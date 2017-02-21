/*
 * CPUSolver.cpp
 *
 *      Author: Artur Kucia
 */

#include "CPUSolver.h"
#include "Matrix.h"
#include "Vector.h"
#include <cmath>
#include <cstdio>

CPUSolver::CPUSolver() {


}

CPUSolver::~CPUSolver() {

}

Matrix CPUSolver::solveGJ(Matrix A, Vector B){

	int n = A.getN();
		int m = B.getM();

		Matrix I(n);
		float *i_p = I.getArrayPointer();

		float *a = A.getArrayPointer();
		float *b = B.getArrayPointer();

	for(int j=0; j<n; j++){

		if ( a[j*n+j] == 0.0){
			for(int k=j+1; k<n; k++){

				if ( a[k*n+j] != 0.0){
					for(int i=0; i<n; i++){

						a[j*n+i] += a[k*n+i];
						i_p[j*n+i] += i_p[k*n+i];
					}
					break;
				}
			}

		}

		for(int i=0; i<n; i++){

			if (i != j){
				i_p[j*n+i] /= a[j*n+j];
				a[j*n+i] /= a[j*n+j];
			}

		}

		i_p[j*n+j] /= a[j*n+j];
		a[j*n+j] /= a[j*n+j];

		for(int i=0; i<n; i++){
			for(int k=0; k<n; k++){
				if (k != j){
					i_p[k*n+i] -= i_p[j*n+i]*a[k*n+j];
					if( i != j)
						a[k*n+i] -= a[j*n+i]*a[k*n+j];
				}
			}
		}

	}

	return I.matMul(B);
}




Matrix CPUSolver::solveLUD(Matrix A, Vector B){

	int n = A.getN();
	int m = B.getM();
	Vector X(n);

	float *a = A.getArrayPointer();
	float *b = B.getArrayPointer();
	float *x = X.getArrayPointer();



	for(int k=0;k<n;++k){

		if ( a[k*n+k] == 0.0){
			for(int p=k+1; p<n;p++){

				if ( a[p*n+k] != 0.0){
					for(int i=0; i<n; i++){

						a[k*n+i] += a[p*n+i];
					}
					b[k] += b[p];
					break;
					}
				}

			}

		for(int i=k;i<n;++i){

			 float sum = 0.0;
			 for(int p=0;p<k;++p){

				 sum += a[i*n+p]*a[p*n+k];
			 }

			 a[i*n+k] -= sum;
		  }
		  for(int j=k+1;j<n;++j){

			 float sum=0.0;
			 for(int p=0;p<k;++p){

				 sum += a[k*n+p]*a[p*n+j];
			 }
			 a[k*n+j]=(a[k*n+j]-sum)/a[k*n+k];
		  }
	}


	float y[n];
	for(int i=0;i<n;++i){

		float sum=0.0;
	    for(int k=0;k<i;++k){

	    	sum += a[i*n+k]*y[k];
	    }
	    y[i]=(b[i]-sum)/a[i*n+i];
	}
	for(int i=n-1;i>=0;--i){

		float sum = 0.0;
		for(int k=i+1;k<n;++k){

			sum+=a[i*n+k]*x[k];
		}
		x[i]=(y[i]-sum);
	}
	return X;
}
