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


// temporary: reflect on normal to floor's plane nvm just zero out
		if (p->pos.z < 0)
		{
			p->pos.z = 0;
			p->vel.z = 0;
		}

		if (p->pos.y < 0)
		{
			p->pos.y = 0;
			p->vel.y = 0;
		}

		if (p->pos.y > 10)
		{
			p->pos.y = 10;
			p->vel.y = 0;
		}

		if (p->pos.x < 0)
		{
			p->pos.x = 0;
			p->vel.x = 0;
		}

		if (p->pos.x > 10)
		{
			p->pos.x = 10;
			p->vel.x = 0;
		}
	}
	return particles;
}