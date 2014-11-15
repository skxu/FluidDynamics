#include "solver.h"

Solver::Solver(
	std::vector<Particle*> particleList)
{
	currentParticles = particleList;
}

/* Calculate density and pressure of a particle based on neighbors
*/
Particle* Solver::UpdateDensityPressure(
	Particle* particle, 
	std::vector<Particle*>* neighborList, 
	std::vector<float> weights)
{
   //TODO
	Particle* p = new Particle(*particle);
	p->neighbors = neighborList;

	p->pressure = 0;
	p->density = 0;
	for (int i = 0; i < neighborList->size(); i++)
	{
		Particle* n = (*neighborList)[i];
		float w = weights[i];
		p->pressure += n->pressure * w;
		p->density += n->density * w;
	}
	return p;
}

void Solver::UpdateAll()
{
	//TODO
	std::vector<Particle*>* updatedParticles = new std::vector<Particle*>();

	for (std::vector<int>::size_type i = 0; i != currentParticles.size(); i++) 
	{
		Particle* p = currentParticles[i];
		//TODO: Get neighbors
		std::vector<Particle*>* neighbors = CalcNeighbors(p);
		std::vector<float> weights = std::vector<float>();
		
		//UPDATE the particle
		Particle* updatedParticle = UpdateDensityPressure(p, neighbors, weights);
		updatedParticles->push_back(updatedParticle);

	}

	currentParticles = *updatedParticles;
}

void Solver::SetParticles(
	std::vector<Particle*>* particleList)
{
	currentParticles = *particleList;
}

std::vector<Particle*> Solver::GetParticles()
{
	return currentParticles;
}


std::vector<Particle*>* Solver::CalcNeighbors(
	Particle* particle)
{

	std::vector<Particle*> * neighbors = new std::vector<Particle*>();
	//TODO SOMEHOW GET NEIGHBORS YEAH

	return neighbors;
}
