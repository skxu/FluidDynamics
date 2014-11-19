#ifndef SOLVER_H
#define SOLVER_H

#include "Constants.h"
#include "Particle.h"
#include <vector>
#include <map>
#include "WeightSolver.h"

class Solver
{
private:
	pVec currentParticles;





	Particle* UpdateDensityPressure(Particle* particle, pVec* neighborList,
		fVec weights);

	pVec* CalcNeighbors(Particle* particle);

	//do we need this?
	void SetParticles(pVec* particleList);



public:
	Solver(pVec initialParticles);

	pVec GetParticles();

	void UpdateAll();
	float calcDensityField(fVec* first, fVec* masses);
	fVec* calcPressure(float k, float rho, fVec* densities);
	vec3* calcPressure(vec3 vec, fVec distances, float p, fVec pressures, fVec masses, float rho);
	vec3* calcViscosity(vec3 vec, fVec distances, float mu, vec3 vel, float rho, float m);
	float l2Norm(vec3 vec);
};


#endif