#include "solver.h"

#ifndef WEIGHTSOLVER_H
#define WEIGHTSOLVER_H

class WeightSolver
{
public:
	static fVec* CalculateWeights(fVec distances);
	static fVec* CalculateFirstDerivativesWeights(fVec distances);
	static fVec* CalculateSecondDerivativesWeights(fVec distances);
};

#endif