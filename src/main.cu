/*
 * main.cu
 *
 *      Author: Artur Kucia
 */


#include <iostream>
#include <string>
#include <sys/time.h>
#include "Matrix.h"
#include "Vector.h"
#include "Solver.h"
#include "CPUSolver.h"
#include "GPUSolver.cuh"
#include "benchmark.h"


int main(int argc, char *argv[]){
	using namespace std;


	int inputMatrixSize = 0;
	string inputMatrixPath;
	string inputVectorPath;
	string outputVectorPath;
	bool useRandom = false;
	bool useGPU = false;
	bool printResults = false;
	bool useGJ = true;
	bool saveResults = false;

	if (argc < 3){
			 fprintf(stderr, "usage: %s input not specified \n", argv[0]);
		     exit(1);
	}

	for (int i = 1; i < argc; i++){

		if((strcmp(argv[i], "-random") == 0 )|| (strcmp(argv[i], "-r") == 0)){
			if (i + 1 < argc ){
				useRandom = true;
				i++;
				inputMatrixSize = atoi(argv[i]);
				cout << "using random matrix of size: " << inputMatrixSize << endl;
			}
			else{
				fprintf(stderr, "usage: %s random_matrix_size  \n", argv[0]);
				exit(1);
			}
		}

		else if ((strcmp(argv[i], "-input") == 0 )|| (strcmp(argv[i], "-i") == 0)){
			useRandom = false;
			if (i + 2 < argc ){
				i++;
				inputMatrixPath = (string) argv[i];
				i++;
				inputVectorPath = (string) argv[i];
			}
			else{
				fprintf(stderr, "usage: %s path to input_matrix_path input_vector_path  \n", argv[0]);
				exit(1);
			}
		}
		else if ((strcmp(argv[i], "-output") == 0 )|| (strcmp(argv[i], "-o")== 0)){
			if (i + 1 < argc ){
				saveResults = true;
				i++;
				outputVectorPath = (string) argv[i];
			}
			else{
				fprintf(stderr, "usage: %s output_file_path  \n", argv[0]);
				exit(1);
			}
		}
		else if (strcmp(argv[i], "-gpu") == 0){
			useGPU = true;
		}
		else if (strcmp(argv[i], "-scr") == 0){
			printResults = true;
		}
		else if(strcmp(argv[i], "-gj") == 0){
			useGJ = true;
		}
		else if(strcmp(argv[i], "-lud") == 0){
			useGJ = false;
		}
	}

	Matrix A(1);
	Vector b(1);


	if (useRandom){
		cout << "Using randomized input..." << endl;
		A = Matrix(inputMatrixSize);
		A.randomize();
		b = Vector(inputMatrixSize);
		b.randomize();

	}
	else{
		cout << "Loading matrix A..." << endl;
		A = Matrix(inputMatrixPath);
		cout << "Loading vector b..." << endl;
		b = Vector(inputVectorPath);
	}



	double time = 0.0;
	Matrix results(1);
	if(useGJ){
		if(useGPU){
			GPUSolver sGPU = GPUSolver();
			time = cpuTimer();
			results = sGPU.solveGJ(A,b);
			time = cpuTimer() - time;
		}
		else{
			CPUSolver sCPU = CPUSolver();
			time = cpuTimer();
			results = sCPU.solveGJ(A,b);
			time = cpuTimer() - time;

		}

	}
	else{
		if(useGPU){
			GPUSolver sGPU = GPUSolver();
			time = cpuTimer();
			results = sGPU.solveLUD(A,b);
			time = cpuTimer() - time;
		}
		else{
			CPUSolver sCPU = CPUSolver();
			time = cpuTimer();
			results = sCPU.solveLUD(A,b);
			time = cpuTimer() - time;

		}


	}

	cout << "Time: " << time << endl;

	if(printResults){
		cout << results << endl;
	}
	if(saveResults){
		results.save(outputVectorPath);
	}

	cout << "done." << endl;

	return 0;
}
