#include "solver.h"

Solver::Solver(std::vector<Particle> particleList)
{
	currentParticles = particleList;
}

Particle Solver::UpdateParticle(Particle particle, std::vector<Particle> neighborList)
{
   //TODO
	Particle p = Particle();
	p.neighbors = neighborList;
	return p;
}

void Solver::UpdateAll()
{
	//TODO
	std::vector<Particle> updatedParticles;

	for (std::vector<int>::size_type i = 0; i != currentParticles.size(); i++) 
	{
		Particle p = currentParticles[i];
		//TODO: Get neighbors
		std::vector<Particle> neighbors  = CalcNeighbors(p);
		
		//UPDATE the particle
		Particle updatedParticle = UpdateParticle(p, neighbors);
		updatedParticles.push_back(updatedParticle);

	}

	currentParticles = updatedParticles;
}

void Solver::SetParticles(std::vector<Particle> particleList)
{
	currentParticles = particleList;
}


std::vector<Particle> Solver::CalcNeighbors(Particle particle)
{

	std::vector<Particle> neighbors;
	//TODO SOMEHOW GET NEIGHBORS YEAH

	return neighbors;
}
