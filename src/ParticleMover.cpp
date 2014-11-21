#include "ParticleMover.h"

pVec* ParticleMover::updateVelocities(pVec* particles, float ts)
{
	pVec* newParticles = new pVec();
	for (Particle* p: *particles)
	{
		Particle* newP = new Particle(*p);
		newP->vel += newP->acc * ts;
		newParticles->push_back(newP);
	}
	return newParticles;
}

pVec* ParticleMover::updatePositions(pVec* particles, float ts)
{
	pVec* newParticles = new pVec();
	for (Particle* p: *particles)
	{
		Particle* newP = new Particle(*p);
		newP->pos += newP->vel * ts;
		newParticles->push_back(newP);
	}
	return newParticles;
}