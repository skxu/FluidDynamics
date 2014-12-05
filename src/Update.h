#ifndef __UPDATE_H__
#define __UPDATE_H__

#include "Grid.h"
#include "Variables.h"
#include <math.h>
#include <string.h>
#include <vector>

void compute_density(sim_state_t* s, sim_param_t* params, Grid* grid);
void compute_accel(sim_state_t* state, sim_param_t* params, Grid* grid);

void leapfrog_step(sim_state_t* s, sim_param_t* p, float dt);
void leapfrog_start(sim_state_t* s, sim_param_t* p, float dt);

void damp_reflect(int which, float barrier,
                  float* x, float* v, float* vh, sim_param_t* p);

void reflect_bc(sim_state_t* s, sim_param_t* p);

void normalize_mass(sim_state_t* s, sim_param_t* param, Grid* grid);

#endif