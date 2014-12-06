/** Smoothed Particle Hydrodynamics 3D implementation                         **/
/** Based on: http://www.cs.cornell.edu/~bindel/class/cs5220-f11/code/sph.pdf **/

#include "Update.h"
#include "Grid.h"
#include "Initializer.h"
#include "omp.h"

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
		assert(xi >= -0.001 && xi <= 1.001);
		assert(yi >= -0.001 && yi <= 1.001);
    assert(zi >= -0.001 && zi <= 1.001);
	}
}
void write_frame_data(ofstream* fp, int n, float* x)
{
	for (int i = 0; i < n; i ++)
	{
    // Shifted positions are for simple-render
    float shiftx = x[3*i+0] - 0.5;
    float shifty = x[3*i+1] - 0.5;
		*fp << "p " << shiftx << " " << shifty << " " << x[3*i+2] << "\n";
	}
	*fp << "t\n";
}

void init_params(sim_param_t* params) {
  // Kevin you will need to fix this
  params->fname = "../outputs/run.txt"; /* File name */
  params->nframes = 200; /* Number of frames */
  params->npframe = 100; /* Steps per frame */
  params->h = 5e-2; /* Particle size */
  params->dt = 1e-4; /* Time step */
  params->rho0 = 1000; /* Reference density */
  params->k = 500; /* Bulk modulus */
  params->mu = 0.1; /* Viscosity */
  params->g = 9.8; /* Gravity strength */
  params->damp = 0.75; /* Damp */
}

int main(int argc, char* argv[])
{
  omp_set_num_threads(16);
	sim_param_t params;
  init_params(&params);
  sim_state_t* state = place_particles(&params, box_indicator);
  //sim_state_t* state = place_particles(&params, sphere_indicator_with_water_plane);
  Grid* grid = new Grid(1.0, 1.0, 1.0, params.h, state);
  grid->setParticles();
  normalize_mass(state, &params, grid);

	ofstream* fp = new ofstream();
	fp->open(params.fname);
	int nframes = params.nframes;
	int npframe = params.npframe;
	float dt = params.dt;
	int n = state->n;
  printf("Number of Particles: %d\n", n);
	write_frame_data(fp, n, state->x);
	compute_accel(state, &params, grid);
	leapfrog_start(state, &params, dt);
  grid->setParticles();
	check_state(state);
	for (int frame = 1; frame < nframes; ++frame) {
    printf("On iteration %d / %d\n", frame, nframes);
		for (int i = 0; i < npframe; ++i) {
			compute_accel(state, &params, grid);
			leapfrog_step(state, &params, dt);
			check_state(state);
      grid->setParticles();
		}
		write_frame_data(fp, n, state->x);
	}
	fp->close();
  delete grid;
	free_state(state);
}