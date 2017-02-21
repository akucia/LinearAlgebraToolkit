/*
 * GPUSolver.h
 *
 *      Author: Artur Kucia
 */

#ifndef GPUSOLVER_H_
#define GPUSOLVER_H_

#include "Solver.h"


class GPUSolver: public Solver {
public:
	GPUSolver();
	virtual ~GPUSolver();
	Matrix solveGJ(Matrix A, Vector B);
	Matrix solveLUD(Matrix A, Vector B);
};

#endif /* GPUSOLVER_H_ */
