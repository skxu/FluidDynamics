#include "solver.h"

#define pi 3.14159265359

class WeightSolver
{
	static fVec* CalculateWeights(fVec distances);
  static fVec* CalculateFirstDerivativesWeights(fVec distances);
  static fVec* CalculateSecondDerivativesWeights(fVec distances);
};