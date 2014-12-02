#include "Particle.h"

Particle::Particle()
{
	memset(this, 0, sizeof(Particle));
  this->mass = MASS_WATER;
}

Particle::Particle(const Particle &other)
{
	memcpy(this, &other, sizeof(Particle));
}
