/*
 * Matrix.cpp
 *
 *	Author: Artur Kucia
 */


#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <random>
#include <iomanip>
#include <ctime>
#include "Matrix.h"


Matrix::Matrix(int x, int y): n(x), m(y), table(new float[x*y])  {

	for(int i=0; i<x; i++)
			for(int j=0; j<y; j++)
				table[y*i+j] = 0.0;

}

Matrix::Matrix(int x): n(x), m(x), table(new float[x*x])  {

	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			if(i==j)
				table[n*i+j] = 1.0;
			else
				table[n*i+j] = 0.0;

}

Matrix::Matrix(float* tab, int x, int y): n(x), m(y), table(new float[x*y])  {

	for(int i=0; i<n; i++)
		for(int j=0; j<m; j++)
			table[m*i+j] = tab[m*i+j];

}
Matrix::Matrix(const Matrix &other): n(other.n), m(other.m), table(new float[other.n*other.m]){

	for(int i=0; i<n; i++)
		for(int j=0; j<m; j++)
			table[m*i+j] = other.table[m*i+j];

}

Matrix::Matrix(std::string path){
	std::string line;
	std::string unused1;
	std::string unused2;
	std::string unused3;
	std::string N;
	std::string M;
	std::string temp;
	std::ifstream file;

	file.open(path);
	if (file.is_open()){
		file >> unused1 >>unused2 >> N >> unused3 >> M;
		n = std::stoi(N);
		m = std::stoi(M);
		table = new float[n*m];
		int i=0;
		while(file >> temp){
			table[i] = std::stof(temp);
			++i;
		}
		std::cout << "Matrix loaded successfully!" << std::endl;
		file.close();

	}
	else{
		std::cout << "Error occurred while opening the file: " << path << std::endl;
		std::cout << "Matrix not loaded!";
	}


}

Matrix::~Matrix() {
	delete  [] table;

}

float* Matrix::getArrayPointer(){
	return table;
}

Matrix Matrix::matMul(const Matrix &other){
	if(m == other.n){
		Matrix result(n,other.m);

		for (int i=0; i<result.n; i++){

			for(int j=0; j<result.m; j++){

				for(int k=0; k<m; k++){

					result.table[i*result.m + j] += table[i*m + k] * other.table[k*other.m + j];

				}
			}
		}
		return result;
	}
	else{
		std::cout << "ERROR - the dimensions do not match: " << m << "!=" << other.n << std::endl;
		return Matrix(1,1);

	}


}
bool Matrix::operator==(const Matrix &other){
	float epsilon = 5.0;
	if (n != other.n)
		return false;
	else if(m != other.m)
		return false;
	else{
		for(int i=0; i<n; i++){

			for(int j=0; j<m; j++){

				if (fabs(table[i*m+j] - other.table[i*m+j]) > epsilon){
					std::cout << "Different elements: " << std::endl;
					std::cout << table[i*m+j] << " != " << other.table[i*m+j] << std::endl;
					return false;
				}
			}
		}
	}
	return true;

}
void Matrix::operator=(const Matrix &other){
	delete [] table;
	n = other.n;
	m = other.m;
	table = new float[n*m];
	for (int i=0; i<n;i++)
		for (int j=0; j<m; j++)
			table[i*m+j] = other.table[i*m+j];
}

int Matrix::getN(){
	return n;
}

int Matrix::getM(){
	return m;
}

void Matrix::randomize(){
	srand(time(NULL));
	for (int i=0; i<n; i++){

		for(int j=0; j<m; j++){

			table[i*m+j] = rand()%10;
		}
	}
}

void Matrix::save(std::string path){
	std::ofstream file;
	file.open(path);

		if (file.is_open()){
			file << "Matrix N: " << n << ", M: " << m << std::endl;
			for(int i=0;i<n;++i){
				for(int j=0;j<m;++j){
					file << table[i*m+j] << " ";

				}
				file << std::endl;
			}

			std::cout << "Matrix saved successfully!" << std::endl;
			file.close();

		}
		else{
			std::cout << "Error occurred while opening the file: " << path << std::endl;
			std::cout << "Matrix not saved!";
		}



}

std::ostream &operator<<(std::ostream &out, const Matrix &M){
	for(int i=0; i<M.n; i++){
		out << "[  ";
		for(int j=0; j<M.m; j++)
			out << std::setw(5) << std::setprecision(3) << M.table[i*M.m +j] << "  ";
		out << "]" << std::endl;
	}
    return out;
}

