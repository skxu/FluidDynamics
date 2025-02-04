#include "vec3.h"
#include <vector>
#include "Constants.h"

#ifndef PARTICLE_INCLUDE
#define PARTICLE_INCLUDE
struct Particle {
	vec3 pos;
	vec3 vel;
	vec3 acc;
  vec3 gridPos;
    
	float density, pressure, mass;
	std::vector<Particle*>* neighbors;
  std::vector<vec3*>* neighborDirections;
	Particle();

	Particle(const Particle &other);
};
#endif