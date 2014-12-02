#include "Variables.h"
typedef int(*domain_fun_t)(float, float, float);
int box_indicator(float x, float y, float z);
sim_state_t* place_particles(sim_param_t* param, domain_fun_t indicatef);