/*
 * Solver.h
 *
 *      Author: Artur Kucia
 */

#ifndef SOLVER_H_
#define SOLVER_H_
#include "Matrix.h"
#include "Vector.h"


class Solver {
public:
	virtual Matrix solveGJ(Matrix A, Vector B) = 0;
	virtual Matrix solveLUD(Matrix A, Vector B) = 0;
};

#endif /* SOLVER_H_ */
