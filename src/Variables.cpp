#include "Variables.h"

sim_state_t* alloc_state(int n)
{
	sim_state_t* state = new sim_state_t();
	state->n = n;
	state->mass = 0;
	state->rho = new float[n]();
	state->x = new float[n * 3]();
	state->vh = new float[n * 3]();
	state->v = new float[n * 3]();
	state->a = new float[n * 3]();
	return state;
}

void free_state(sim_state_t* s)
{
	delete[] s->a;
	delete[] s->v;
	delete[] s->vh;
	delete[] s->x;
	delete[] s->rho;
	delete s;
}