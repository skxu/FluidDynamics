#include "Particle.h"

Particle::Particle()
{
	memset(this, 0, sizeof(Particle));
}

Particle::Particle(const Particle &other)
{
	memcpy(this, &other, sizeof(Particle));
}
