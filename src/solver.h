#ifndef SOLVER_H
#define SOLVER_H

#include "Constants.h"
#include "Particle.h"
#include <vector>
#include <map>
#include "Grid.h"

class Solver
{
private:
	pVec currentParticles;
	Grid* grid;



	//do we need this?
	void SetParticles(pVec* particleList);



public:
	Solver(pVec initialParticles);

	pVec GetParticles();

	void UpdateAll();
};


#endif