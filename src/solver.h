#ifndef SOLVER_H
#define SOLVER_H

#include "Particle.h"
#include <vector>
#include <map>
class Solver 
{

private:
	std::vector<Particle> currentParticles;

	Particle UpdateParticle(Particle particle, std::vector<Particle>* neighborList);

	std::vector<Particle> *CalcNeighbors(Particle particle);



public: 
	Solver(std::vector<Particle> initialParticles);

	void SetParticles(std::vector<Particle> particleList);

	void UpdateAll();

};


#endif