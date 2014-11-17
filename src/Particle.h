#include "vec3.h"
#include <vector>

#ifndef PARTICLE_INCLUDE
#define PARTICLE_INCLUDE
class Grid;
struct Particle {
	vec3 pos;
	vec3 vel;
	vec3 acc;
    vec3 gridPos;
    
	float density, pressure, mass;
	std::vector<Particle*>* neighbors;
    Grid *grid;
	Particle();

	Particle(const Particle &other);
};
#endif