#ifndef PARTICLE_MOVER_H
#define PARTICLE_MOVER_H
#include "solver.h"

class ParticleMover
{
	public:
	static pVec* updateVelocities(pVec* particles, float ts);
	static pVec* updatePositions(pVec* particles, float ts);

};

#endif