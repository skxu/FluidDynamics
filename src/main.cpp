#include "Update.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <ctime>

using namespace std;

void check_state(sim_state_t* s)
{
	for (int i = 0; i < s->n; ++i) {
		float xi = s->x[3 * i + 0];
		float yi = s->x[3 * i + 1];
    float zi = s->x[3 * i + 2];
		assert(xi >= 0 || xi <= 1);
		assert(yi >= 0 || yi <= 1);
    assert(zi >= 0);
	}
}
void write_frame_data(ofstream* fp, int n, float* x)
{
	for (int i = 0; i < n; i ++)
	{
		*fp << "particle " << x[3 * i] << " " << x[3 * i + 1] << " " << x[3 * i + 2] << "\n";
	}
	*fp << "time\n";
}

void init_params(sim_param_t* params) {
  // Kevin you will need to fix this
  params->fname = "../outputs/run.txt"; /* File name */
  params->nframes = 300; /* Number of frames */
  params->npframe = 4; /* Steps per frame */
  params->h = 5e-2; /* Particle size */
  params->dt = 1e-3; /* Time step */
  params->rho0 = 1000; /* Reference density */
  params->k = 1e3; /* Bulk modulus */
  params->mu = 0.1; /* Viscosity */
  params->g = 9.8; /* Gravity strength */
}

int main(int argc, char* argv[])
{
	sim_param_t params;
  init_params(&params);
	/*if (get_params(argc, argv, &params) != 0)
		exit(-1);*/
	sim_state_t* state = init_particles(&params);
	ofstream* fp = new ofstream();
	fp->open(params.fname);
	int nframes = params.nframes;
	int npframe = params.npframe;
	float dt = params.dt;
	int n = state->n;
	write_frame_data(fp, n, state->x);
	compute_accel(state, &params);
	leapfrog_start(state, dt);
	check_state(state);
	for (int frame = 1; frame < nframes; ++frame) {
		for (int i = 0; i < npframe; ++i) {
			compute_accel(state, &params);
			leapfrog_step(state, dt);
			check_state(state);
		}
		write_frame_data(fp, n, state->x);
	}
	fp->close();
	free_state(state);
}