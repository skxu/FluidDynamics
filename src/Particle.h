#include "vec3.h"
#include <vector>

#ifndef PARTICLE_INCLUDE
#define PARTICLE_INCLUDE
struct Particle {
	vec3 pos;
	vec3 vel;
	vec3 acc;
	float density, pressure;
	std::vector<Particle> neighbors;
	Particle();

	Particle(const Particle &other);
};
#endif