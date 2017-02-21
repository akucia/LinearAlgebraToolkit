/*
 * tests.cpp
 *
 *      Author: Artur Kucia
 */

#include "Matrix.h"
#include "Vector.h"
#include "CPUSolver.h"
#include "GPUSolver.cuh"
#include <iostream>


int tests(int nTests, int minSize, int maxSize){
	int seed = time(NULL);
	srand(seed);
	using namespace std;
	CPUSolver sCPU = CPUSolver();
	GPUSolver sGPU = GPUSolver();
	Matrix A(1);
	Vector B(1);
	Matrix result_cpu(1);
	Matrix result_gpu(1);
	Matrix result_gj(1);
	Matrix result_lud(1);
	Matrix result_lud2(1);

	cout << "#######################################################" << endl;
	for(int n=minSize; n<maxSize; n++){
		for (int i=0; i<nTests; i++){

			cout << "Size: " << n << " Test #"<< i+1 << endl;
			A = Matrix(n,n);
			B = Vector(n);
			A.randomize();
			B.randomize();


			result_cpu = sCPU.solveGJ(A,B);
			result_gpu = sGPU.solveGJ(A,B);


			if (!(result_cpu == result_gpu)){
				cout << "\nFound a bug!" << endl;
				cout << "seed: " << seed << endl;
				cout << "Matrix A:\n" << A << endl << "Vector B:\n" << B << endl;
				cout << "cpu_results:\n" << result_cpu << endl << "gpu_results:\n" << result_gpu << endl;
				cout << sCPU.solveGJ(A,B);
				return 1;
			}
			//cout << "cpu_results:\n" << result_cpu << endl << "gpu_results:\n" << result_gpu << endl;


		}
	}
	cout << "GJ Tests clear!" << endl;
	/*
	cout << "#######################################################" << endl;
	for(int n=minSize; n<maxSize; n++){
		for (int i=0; i<nTests; i++){

			cout << "Size: " << n << " Test #"<< i+1 << endl;
			A = Matrix(n,n);
			B = Vector(n);
			A.randomize();
			B.randomize();

			if(n>5){
							float *a = A.getArrayPointer();
							a[2*n+2]=0;

						}



			result_gj = sCPU.solveGJ(A,B);
			result_lud = sCPU.solveLUD(A,B);
			Matrix result_lud_gpu = sGPU.solveLUD(A,B);

			if (!(result_gj == result_lud)){
				cout << "\nFound a bug!" << endl;
				cout << "seed: " << seed << endl;
				cout << "Matrix A:\n" << A << endl << "Vector B:\n" << B << endl;
				cout << "gj results:\n" << result_gj << endl << "lud results:\n" << result_lud << endl;
				cout << result_lud_gpu << endl;
				return 1;
			}
			//cout << result_gj << endl << result_lud << endl;


		}
	}
	cout << "LU vs GJ on CPU Tests clear!" << endl;


	cout << "#######################################################" << endl;
	for(int n=minSize; n<maxSize; n++){
		for (int i=0; i<nTests; i++){

			cout << "Size: " << n << " Test #"<< i+1 << endl;
			A = Matrix(n,n);
			B = Vector(n);
			A.randomize();
			B.randomize();


			result_gj = sGPU.solveGJ(A,B);
			result_lud = sGPU.solveLUD(A,B);

			if (!(result_gj == result_lud)){
				cout << "\nFound a bug!" << endl;
				cout << "seed: " << seed << endl;
				cout << "Matrix A:\n" << A << endl << "Vector B:\n" << B << endl;
				cout << "gj results:\n" << result_gj << endl << "lud results:\n" << result_lud << endl;

				return 1;
			}
			//cout << "gj results:\n" << result_gj << endl << "lud results:\n" << result_lud << endl;


		}
	}
	cout << "LU vs GJ on GPU Tests clear!" << endl;
	*/
	cout << "#######################################################" << endl;
	for(int n=minSize; n<maxSize; n++){
		for (int i=0; i<nTests; i++){

			cout << "Size: " << n << " Test #"<< i+1 << endl;
			A = Matrix(n,n);
			B = Vector(n);
			A.randomize();
			B.randomize();



			result_gpu = sGPU.solveLUD(A,B);
			result_cpu = sCPU.solveLUD(A,B);

			if (!(result_cpu == result_gpu)){
				cout << "\nFound a bug!" << endl;
				cout << "seed: " << seed << endl;
				cout << "Matrix A:\n" << A << endl << "Vector B:\n" << B << endl;
				cout << "cpu_results:\n" << result_cpu << endl << "gpu_results:\n" << result_gpu << endl;
				cout << "gj method:\n" << sCPU.solveGJ(A,B) << "\n" << sGPU.solveGJ(A,B) << endl;
				return 1;
			}
			//cout << "gj method:\n" << sCPU.solveGJ(A,B) << "\n" << sGPU.solveGJ(A,B) << endl;


		}
	}
	cout << "LU Tests clear!" << endl;

	return 0;


}
