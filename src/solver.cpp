#include "solver.h"


Solver::Solver(
	pVec particleList)
{
	currentParticles = particleList;
    grid = new Grid(420,420,420,4); //What are our parameters?!
    grid->setParticles(currentParticles);
}

void Solver::UpdateAll()
{
	//TODO
	pVec* updatedParticles = new pVec();

	for (std::vector<int>::size_type i = 0; i != currentParticles.size(); i++) 
	{
		Particle* p = currentParticles[i];
		//TODO: Get neighbors
		pVec* neighbors = CalcNeighbors(p);
		fVec weights = fVec();
		
		//UPDATE the particle
		Particle* updatedParticle = UpdateDensityPressure(p, neighbors, weights);
		updatedParticles->push_back(updatedParticle);

	}

	currentParticles = *updatedParticles;
}

void Solver::SetParticles(
	pVec* particleList)
{
	currentParticles = *particleList;
}

pVec Solver::GetParticles()
{
	return currentParticles;
}