/*
 * Vector.cpp
 *
 *      Author: Artur Kucia
 */


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "Matrix.h"
#include "Vector.h"

Vector::Vector(int n): Matrix(n,1){


}
Vector::Vector(float *tab, int n): Matrix(tab,n,1){


}

Vector::Vector(std::string path) : Matrix(1) {
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
		std::cout << "Vector loaded successfully!" << std::endl;
		file.close();

	}
	else{
		std::cout << "Error occurred while opening the file: " << path << std::endl;
		std::cout << "Vector not loaded!";
	}


}
