#ifndef __INITIALIZER_H__
#define __INITIALIZER_H__

#include "Variables.h"
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <math.h>

typedef int(*domain_fun_t)(float, float, float);
int box_indicator(float x, float y, float z);
int sphere_indicator_with_water_plane(float x, float y, float z);
sim_state_t* place_particles(sim_param_t* param, domain_fun_t indicatef);

#endif