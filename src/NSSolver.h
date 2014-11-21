#ifndef NSSolver_H
#define NSSolver_H

#include "Constants.h"
#include "Particle.h"
#include "PolynomialKernel.h"

class NSSolver
{
public:
	bool pressure, viscosity, interactive, gravity;

	NSSolver();

	pVec* solve(pVec* particles, float h);
};

#endif