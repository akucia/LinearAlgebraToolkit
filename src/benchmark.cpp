/*
 * benchmark.cpp
 *
 *      Author: Artur Kucia
 */

#include <iostream>
#include <fstream>
#include "Matrix.h"
#include "Vector.h"
#include "CPUSolver.h"
#include "GPUSolver.cuh"

#include <sys/time.h>

double cpuTimer() {
	struct timeval clock;
	gettimeofday(&clock, NULL);
	return ((double)clock.tv_sec + (double)clock.tv_usec *1.e-6);
}


void save_report(double* CPU, double* GPU, int size, std::string filename ){

	using namespace std;
	ofstream myfile;
	cout << "Saving data to: " << filename << endl;
	myfile.open (filename);
	myfile << "#n\tCPU\tGPU\n";
	for (int i=0; i<size;++i){
		myfile << i<< "\t" << CPU[i] << "\t" << GPU[i] << endl;
	}
	myfile.close();
}


void benchmark(int maxSize){
	int seed = time(NULL);
	srand(seed);
	using namespace std;
	CPUSolver sCPU = CPUSolver();
	GPUSolver sGPU = GPUSolver();
	Matrix A(1);
	Vector B(1);

	double *cpu_time = new double[maxSize];
	double *gpu_time = new double[maxSize];

	for(int i=0; i<maxSize; i++){
		cpu_time[i] = 0.0;
		gpu_time[i] = 0.0;
	}
	double g_time = 0.0;
	double c_time = 0.0;
	for(int n=1; n<maxSize; n++){

			cout << "Size: " << n << endl;
			A = Matrix(n,n);
			B = Vector(n);
			A.randomize();
			B.randomize();
			c_time = cpuTimer();
			Matrix result_cpu = sCPU.solveGJ(A,B);
			c_time = cpuTimer() - c_time;

			g_time = cpuTimer();
			Matrix result_gpu = sGPU.solveGJ(A,B);
			g_time = cpuTimer() - g_time;
			if (!(result_cpu == result_gpu)){
				cout << "\nFound a bug, repeating benchmark" << endl;
				n--;
			}
			cout << "cpu time: " << c_time << " gpu time: " << g_time << endl;
			cpu_time[n] = c_time;
			gpu_time[n] = g_time;


		}
	cout << "Benchmark of GJ done\n saving report..." << endl;
	save_report(cpu_time, gpu_time, maxSize, "gauss_jordan_times.txt");

	for(int i=0; i<maxSize; i++){
			cpu_time[i] = 0.0;
			gpu_time[i] = 0.0;
		}

		for(int n=1; n<maxSize; n++){

				cout << "Size: " << n << endl;
				A = Matrix(n,n);
				B = Vector(n);
				A.randomize();
				B.randomize();
				c_time = cpuTimer();
				Matrix result_cpu = sCPU.solveLUD(A,B);
				c_time = cpuTimer() - c_time;

				g_time = cpuTimer();
				Matrix result_gpu = sGPU.solveLUD(A,B);
				g_time = cpuTimer() - g_time;
				if (!(result_cpu == result_gpu)){
					cout << "\nFound a bug, repeating benchmark" << endl;
					n--;
				}
				cout << "cpu time: " << c_time << " gpu time: " << g_time << endl;
				cpu_time[n] = c_time;
				gpu_time[n] = g_time;


			}
		cout << "Benchmark of LUD done\n saving report..." << endl;
		save_report(cpu_time, gpu_time, maxSize, "LUD_times.txt");


}
