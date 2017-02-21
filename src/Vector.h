/*
 * Vector.h
 *
 *      Author: Artur Kucia
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include "Matrix.h"

class Vector: public Matrix{
public:
	Vector();
	Vector(int n);
	Vector(float *tab, int n);
	Vector(std::string path);

};

#endif /* VECTOR_H_ */
