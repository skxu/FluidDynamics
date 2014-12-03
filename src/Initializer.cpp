#include "Initializer.h"

using namespace std;

int box_indicator(float x, float y, float z)
{
	return (x < 0.5) && (y < 0.5) && (z < 0.5);
}

int box_indicator_long(float x, float y, float z)
{
	return (x < 0.25) && (y < 0.25) && (z < 1);
}

int sphere_indicator(float x, float y, float z)
{
	float dx = (x - 0.5);
	float dy = (y - 0.5);
	float dz = (z - 0.5);
	float r2 = dx*dx + dy*dy + dz*dz;
	return (r2 < 0.25*0.25);
}

sim_state_t* place_particles(sim_param_t* param, domain_fun_t indicatef)
{
	float h = param->h;
	float hh = h / 1.3;
	// Count mesh points that fall in indicated region.
	int count = 0;
	for (float x = 0; x < 1; x += hh)
		for (float y = 0; y < 1; y += hh)
			for (float z = 0; z < 1; z += hh)
				count += indicatef(x, y, z);
	// Populate the particle data structure
	sim_state_t* s = alloc_state(count);
	int p = 0;
	for (float x = 0; x < 1; x += hh) {
		for (float y = 0; y < 1; y += hh) {
			for (float z = 0; z < 1; z += hh) {
				if (indicatef(x, y, z)) {
					s->x[3 * p + 0] = x;
					s->x[3 * p + 1] = y;
					s->x[3 * p + 2] = z;
					s->v[3 * p + 0] = 0;
					s->v[3 * p + 1] = 0;
					s->v[3 * p + 2] = 0;
					++p;
				}
			}
		}
	}
	return s;
}

sim_state_t* rain_particles(sim_param_t* param)
{
	srand (static_cast <unsigned> (time(0)));

	int count = 3000;
	// Populate the particle data structure
	sim_state_t* s = alloc_state(count);
	for (int i = 0; i < count; i++) {
		float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		z = 5 * z + 3; // range from z = 3 to z = 8
		s->x[3 * i + 0] = x;
		s->x[3 * i + 1] = y;
		s->x[3 * i + 2] = z;
		s->v[3 * i + 0] = 0;
		s->v[3 * i + 1] = 0;
		s->v[3 * i + 2] = 0;
	}
	return s;
}