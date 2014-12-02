#include "Variables.h"
#include <math.h>

void compute_density(sim_state_t* s, sim_param_t* params);
void compute_accel(sim_state_t* state, sim_param_t* params);

void leapfrog_step(sim_state_t* s, double dt);
void leapfrog_start(sim_state_t* s, double dt);

void damp_reflect(int which, float barrier,
                  float* x, float* v, float* vh);

void reflect_bc(sim_state_t* s);

void normalize_mass(sim_state_t* s, sim_param_t* param);