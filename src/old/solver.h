#ifndef SOLVER_H
#define SOLVER_H

#include "Constants.h"
#include "Particle.h"
#include <vector>
#include <map>
#include "Grid.h"
#include "PressureDensitySolver.h"
#include "NSSolver.h"
#include "ParticleMover.h"

#include <iostream>

class Solver
{
private:
	float cutoff;

	pVec currentParticles;
	Grid* grid;
	PDSolver* pdSolver;
	NSSolver* nsSolver;


	//do we need this?
	void SetParticles(pVec* particleList);



public:
	Solver(pVec initialParticles, float hValue);

	pVec GetParticles();

	void UpdateAll();
};


#endif