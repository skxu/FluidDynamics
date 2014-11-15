#include "solver.h"

#define pVec std::vector<Particle*>
#define fVec std::vector<float>

Solver::Solver(
	pVec particleList)
{
	currentParticles = particleList;
}

/* Calculate density and pressure of a particle based on neighbors
*/
Particle* Solver::UpdateDensityPressure(
	Particle* particle, 
	pVec* neighborList, 
	fVec weights)
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


pVec* Solver::CalcNeighbors(
	Particle* particle)
{

	pVec * neighbors = new pVec();
	//TODO SOMEHOW GET NEIGHBORS YEAH

	return neighbors;
}
