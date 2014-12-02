#include "Initializer.h"
int box_indicator(float x, float y, float z)
{
	return (x < 0.5) && (y < 0.5) && (z < 0.5);
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