#include "PressureDensitySolver.h"
#include "Constants.h"
#include "PolynomialKernel.h"

float PDSolver::calcDensity(Particle* p, float h)
{

	float rho = 0.0f;

	for (int i=0; i< p->neighbors->size(); i++){
		float mass = p->neighbors[i]->mass;
		vec3 direction = p->neighborsDirections[i];
		rho += p->neighbors[i]->mass * PolynomialKernel::Value(direction, h);
	}
	return rho;
}

float PDSolver::calcPressure(Particle* p)
{
	float pressure = 0.0f;
	float c_s = SPEED_SOUND * SPEED_SOUND;
	pressure += c_s * (p->density - DENSITY_WATER);
	return pressure;
}

pVec* PDSolver::UpdateDensities(pVec* particle, float h)
{
	for (int i=0; i<particle->size(); i++){
		float d = PDSolver::calcDensity((*particle)[i], h)
		(*particle)[i]->density = d;
	}
}

pVec* PDSolver::UpdatePressures(pVec* particle)
{
	for (int i=0; i<particle->size(); i++){
		float p = PDSolver((*particle)[i]);
		(*particle)[i]->pressure = p;
	}
}