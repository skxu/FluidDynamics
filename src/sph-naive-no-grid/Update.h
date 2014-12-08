#ifndef __UPDATE_H__
#define __UPDATE_H__

#include "Variables.h"
#include "omp.h"
#include <math.h>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <assert.h>

void compute_density(sim_state_t* s, sim_param_t* params);
void compute_accel(sim_state_t* state, sim_param_t* params);

void leapfrog_step(sim_state_t* s, sim_param_t* p, float dt);
void leapfrog_start(sim_state_t* s, sim_param_t* p, float dt);

void damp_reflect(int which, int i, float barrier,
                  sim_state_t* s, sim_param_t* p);

void reflect_bc(sim_state_t* s, sim_param_t* p);

void normalize_mass(sim_state_t* s, sim_param_t* param);

#endif