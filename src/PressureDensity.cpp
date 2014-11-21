#include "PressureDensitySolver.h"

float PDSolver::calcDensity(Particle* p)
{
	float rho = 0.0f;
	fVec distances;
	first = WeightSolver::CalculateWeights(distances);
	for (int i = 0; i < masses->size(); i++) {
		rho += (*first)[i] * (*masses)[i];
	}

	return rho;
}

float PDSolver::calcPressure(Particle* p)
{
	fVec* p = new fVec();
	float pressure = 0.0f;
	for each (float density in (*densities)) {
		pressure = k * (density - rho);
		p->push_back(pressure);
	}

	return p;
}

pVec* PDSolver::UpdateDensities(pVec* particle)
{
}

pVec* PDSolver::UpdatePressures(pVec* particle)
{
}