#include "ParticleMover.h"

pVec* ParticleMover::updateVelocities(pVec* particles, float ts)
{
	pVec* newParticles = new pVec();
	for each (Particle* p in *particles)
	{
		Particle* newP = new Particle(*p);
		newP->vel += newP->acc * ts;
		newParticles->push_back(newP);
	}
}

pVec* ParticleMover::updatePositions(pVec* particles, float ts)
{
	pVec* newParticles = new pVec();
	for each (Particle* p in *particles)
	{
		Particle* newP = new Particle(*p);
		newP->pos += newP->vel * ts;
		newParticles->push_back(newP);
	}
}