/*
 * GPUSolver.cpp
 *
 *      Author: Artur Kucia
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cuda.h>
#include <cstring>
#include <cfloat>
#include "GPUSolver.cuh"
#include "Matrix.h"
#include "Vector.h"



GPUSolver::GPUSolver() {

}

GPUSolver::~GPUSolver() {

}


__global__ void kernel_jj(float *d_a, float *d_i, int n, int j){
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if( i < n){
		if ( d_a[j*n+j] == 0.0)
			for(int k=j+1; k<n; k++)
				if ( d_a[k*n+j] != 0.0){
					d_a[j*n+i] += d_a[k*n+i];
					d_i[j*n+i] += d_i[k*n+i];
					break;
				}

	}

}


__global__ void normalize_row(float *d_a, float *d_i, int n, int j){
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < n){
		if (i !=j){
			d_i[j*n+i] /= d_a[j*n+j];
			d_a[j*n+i] /= d_a[j*n+j];
		}
		else{
			d_i[j*n+i] /= d_a[j*n+j];
		}
	}

}

__global__ void normalize_diagonal(float *d_a, float *d_i, int n, int j){

		d_a[j*n+j] /= d_a[j*n+j];


}

__global__ void reduce_row_i(float *d_a, float *d_i, int n, int j){
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	if(x<n && y< n)
	if (y != j){
		d_i[y*n+x] -= d_i[j*n+x]*d_a[y*n+j];
	}

}
__global__ void reduce_row_a(float *d_a, float *d_i, int n, int j){
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	if(x<n && y< n)
	if (y != j){
		if( x != j)
		d_a[y*n+x] -= d_a[j*n+x]*d_a[y*n+j];


	}

}


Matrix GPUSolver::solveGJ(Matrix A, Vector B) {

	Matrix A_copy(A);

	int n = A.getN();
	int m = B.getM();

	Matrix I(n);
	float *h_i = I.getArrayPointer();
	float *h_a = A.getArrayPointer();


	float *d_a;
	float *d_i;

	cudaMalloc((void **)&d_a, n*n*sizeof(float));
	cudaMalloc((void **)&d_i, n*n*sizeof(float));

	cudaMemcpy(d_a, h_a, n*n*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_i, h_i, n*n*sizeof(float), cudaMemcpyHostToDevice);


	for(int i=0; i<n; i++){

		dim3 block(256,1);
		dim3 grid((n + block.x - 1) / block.x, 1);
		kernel_jj <<<grid, block>>> (d_a, d_i, n, i);


		dim3 block1(256,1);
		dim3 grid1((n + block1.x - 1) / block1.x,1);
		normalize_row <<< grid1,block1 >>> (d_a, d_i, n, i);

		dim3 block2(16,16);
		dim3 grid2((n + block2.x - 1) / block2.x, (n + block2.y - 1) / block2.y);
		reduce_row_i <<< grid2, block2 >>> (d_a, d_i, n, i);

		reduce_row_a <<< grid2, block2 >>> (d_a, d_i, n, i);

	}


	cudaMemcpy(h_a, d_a, n*n*sizeof(float), cudaMemcpyDeviceToHost);
	cudaMemcpy(h_i, d_i, n*n*sizeof(float), cudaMemcpyDeviceToHost);

	cudaFree(d_a);
	cudaFree(d_i);

	return I.matMul(B);
}

__global__ void kernel_update_u(float *d_a, int n, int k){

	int i = blockIdx.x * blockDim.x + threadIdx.x;

	float sum = 0.0;
	if(i>k-1 && i<n){
		for(int p=0;p<k;++p){

			sum += d_a[i*n+p]*d_a[p*n+k];
		}

	d_a[i*n+k] -= sum;

	}




}
__global__ void kernel_update_l(float *d_a, int n, int k){

	int j = blockIdx.x * blockDim.x + threadIdx.x;

	float sum=0.0;
	if(j>k && j<n){
		for(int p=0;p<k;++p){

			sum+=d_a[k*n+p]*d_a[p*n+j];
		}
		d_a[k*n+j]=(d_a[k*n+j]-sum)/d_a[k*n+k];
	}

}

__global__ void kerneljj(float *d_a, float *d_b, int n, int j){

	int i = blockIdx.x * blockDim.x + threadIdx.x;

	if(i < n){
		if ( d_a[j*n+j] == 0.0){
			for(int k=j+1; k<n; k++){

				if (d_a[k*n+j] != 0.0){
					d_a[j*n+i] += d_a[k*n+i];
					if(i==0){
						d_b[j] += d_b[k];
					}
					break;
				}
			}
		}
	}

}



Matrix GPUSolver::solveLUD(Matrix A, Vector B){

	int n = A.getN();
	int m = B.getM();
	Vector X(n);

	float *h_a = A.getArrayPointer();
	float *h_b = B.getArrayPointer();
	float *h_x = X.getArrayPointer();
	float *h_y = new float[n];

	for(int i=0; i<n; i++)
		h_y[i] = 0.0;

	float *d_a;
	float *d_b;


	cudaMalloc((void **)&d_a, n*n*sizeof(float));
	cudaMalloc((void **)&d_b, n*sizeof(float));


	cudaMemcpy(d_a, h_a, n*n*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, n*sizeof(float), cudaMemcpyHostToDevice);


	for(int k=0;k<n;++k){

		dim3 block2(256,1);
		dim3 grid2((n + block2.x - 1) / block2.x,1);

		kerneljj <<< grid2, block2 >>> (d_a, d_b, n, k);
		kernel_update_u <<< grid2, block2 >>> (d_a, n, k);
		kernel_update_l <<< grid2, block2 >>> (d_a, n, k);

	}


	cudaMemcpy(h_a, d_a, n*n*sizeof(float), cudaMemcpyDeviceToHost);
	cudaMemcpy(h_b, d_b, n*sizeof(float), cudaMemcpyDeviceToHost);

	cudaFree(d_a);
	cudaFree(d_b);

	for(int i=0;i<n;++i){

		float sum = 0.0;
		for(int k=0;k<i;++k){

			sum += h_a[i*n+k]*h_y[k];
		}
		h_y[i]=(h_b[i]-sum)/h_a[i*n+i];
	}
	for(int i=n-1; i>=0 ;--i){

		float sum = 0.0;
		for(int k=i+1; k<n; ++k){

			sum += h_a[i*n+k]*h_x[k];
		}
		h_x[i] = (h_y[i]-sum);
	}

	delete [] h_y;
	return X;
}
