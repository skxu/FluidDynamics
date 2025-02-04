#include "Initializer.h"

using namespace std;

int box_indicator(float x, float y, float z)
{
	return (x < 0.4) && (z < 0.4) || (z < 0.1);
}

int sphere_indicator_with_water_plane(float x, float y, float z)
{
	float dx = (x - 0.5);
	float dy = (y - 0.5);
	float dz = (z - 0.75);
	float r2 = dx*dx + dy*dy + dz*dz;
	return (r2 < 0.2*0.2) || z < 0.25;
}

sim_state_t* place_particles(sim_param_t* param, domain_fun_t indicatef)
{
	float h = param->h;
	float hh = h / param->spacing;
	// Count mesh points that fall in indicated region.
	int count = 0;

	float range = (ceil(1.0 / hh) - 1) * hh;
	float start = (1.0 - range) / 2;
	for (float z = start; z < 1; z += hh) {
		for (float x = start; x < 1; x += hh) {
			for (float y = start; y < 1; y += hh) {
				count += indicatef(x, y, z);
			}
		}
	}

	// Populate the particle data structure
	sim_state_t* s = alloc_state(count);
	int p = 0;

	for (float z = start; z < 1; z += hh) {
		for (float x = start; x < 1; x += hh) {
			for (float y = start; y < 1; y += hh) {
				count += indicatef(x, y, z);
				if (indicatef(x, y, z)) {
					s->x[4 * p + 0] = x;
					s->x[4 * p + 1] = y;
					s->x[4 * p + 2] = z;
					s->v[4 * p + 0] = MAX_INIT_VEL * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					s->v[4 * p + 1] = MAX_INIT_VEL * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					s->v[4 * p + 2] = MAX_INIT_VEL * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					++p;
				}
			}
		}
	}
	return s;
}