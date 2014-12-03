#ifndef __INITIALIZER_H__
#define __INITIALIZER_H__

#include "Variables.h"
#include <cstdlib>
#include <ctime>

typedef int(*domain_fun_t)(float, float, float);
int box_indicator(float x, float y, float z);
int sphere_indicator(float x, float y, float z);
int box_indicator_long(float x, float y, float z);
sim_state_t* place_particles(sim_param_t* param, domain_fun_t indicatef);
sim_state_t* rain_particles(sim_param_t* param);

#endif