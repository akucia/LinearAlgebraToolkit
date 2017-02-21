/*
 * CPUSolver.h
 *
 *      Author: Artur Kucia
 */

#ifndef CPUSOLVER_H_
#define CPUSOLVER_H_

#include "Solver.h"


class CPUSolver: public Solver {
public:
	explicit CPUSolver();
	virtual ~CPUSolver();
	Matrix solveGJ(Matrix A, Vector B);
	Matrix solveLUD(Matrix A, Vector B);
};

#endif /* CPUSOLVER_H_ */
