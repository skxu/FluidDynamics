#ifndef SOLVER_H
#define SOLVER_H

#include "Particle.h"
#include <vector>
#include <map>
class Solver 
{

private:
	std::vector<Particle*> currentParticles;

	Particle* UpdateParticle(Particle* particle, std::vector<Particle*>* neighborList);

	std::vector<Particle*>* CalcNeighbors(Particle* particle);

	//do we need this?
	void SetParticles(std::vector<Particle*>* particleList);



public: 
	Solver(std::vector<Particle*> initialParticles);

	std::vector<Particle*> GetParticles();

	void UpdateAll();

};


#endif