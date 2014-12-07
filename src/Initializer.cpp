#include "Initializer.h"

using namespace std;

int box_indicator(float x, float y, float z)
{
	return (x < 0.25) && (y < 0.25) && (z < 0.9);
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
	return (r2 < 0.2*0.2);
}

int sphere_indicator_with_water_plane(float x, float y, float z)
{
	float dx = (x - 0.5);
	float dy = (y - 0.5);
	float dz = (z - 0.75);
	float r2 = dx*dx + dy*dy + dz*dz;
	return (r2 < 0.2*0.2) || z < 0.25;
	//return z < 0.05;
}

sim_state_t* place_particles(sim_param_t* param, domain_fun_t indicatef)
{
	float h = param->h;
	float hh = h / 1.1;
	// Count mesh points that fall in indicated region.
	int count = 0;
	for (float z = 0; z < 1; z += hh) {
		float range = (ceil(1.0 / hh) - 1) * hh;
		float start = (1.0 - range) / 2;
		for (float x = start; x < 1; x += hh) {
			for (float y = start; y < 1; y += hh) {
				count += indicatef(x, y, z);
			}
		}
	}
	// Populate the particle data structure
	sim_state_t* s = alloc_state(count);
	int p = 0;
	for (float z = 0; z < 1; z += hh) {
		float range = (ceil(1.0 / hh) - 1) * hh;
		float start = (1.0 - range) / 2;
		for (float x = start; x < 1; x += hh) {
			for (float y = start; y < 1; y += hh) {
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