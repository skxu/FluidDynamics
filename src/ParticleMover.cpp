#include "ParticleMover.h"

pVec* ParticleMover::updateVelocities(pVec* particles, float ts)
{
	for (int i = 0; i < particles->size(); i++)
	{
		Particle* p = (*particles)[i];
		p->vel += p->acc * ts;
	}
	return particles;
}

pVec* ParticleMover::updatePositions(pVec* particles, float ts)
{

	for (int i = 0; i < particles->size(); i++)
	{
		Particle* p = (*particles)[i];
		p->pos += p->vel * ts;
	}
	return particles;
}