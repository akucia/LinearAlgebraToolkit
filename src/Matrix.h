/*
 * Matrix.h
 *
 *      Author: Artur Kucia
 */

#ifndef MATRIX_H_
#define MATRIX_H_
#include <iostream>
#include <string>


class Matrix {
protected:
	float* table;
	int n;
	int m;

public:
	Matrix();
	Matrix(int x, int y);
	Matrix(int x);
	Matrix(const Matrix &other);
	Matrix(float* tab, int x, int y);
	Matrix(std::string path);
	virtual ~Matrix();
	Matrix matMul(const Matrix &other);
	bool operator==(const Matrix &other);
	void operator=(const Matrix &other);
	friend std::ostream &operator<<(std::ostream &out, const Matrix &M);
	void save(std::string path);
	int getN();
	int getM();
	float* getArrayPointer();
	void randomize();
};

#endif /* MATRIX_H_ */
