/** Smoothed Particle Hydrodynamics 3D implementation                         **/
/** Based on: http://www.cs.cornell.edu/~bindel/class/cs5220-f11/code/sph.pdf **/

#include "Update.h"
#include "Grid.h"
#include "Initializer.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <ctime>

#include <omp.h>

using namespace std;

void check_state(sim_state_t* s) {
	for (int i = 0; i < s->n; ++i) {
		float xi = s->x[4 * i + 0];
		float yi = s->x[4 * i + 1];
		float zi = s->x[4 * i + 2];
		assert(xi >= -0.001 && xi <= 1.001);
		assert(yi >= -0.001 && yi <= 1.001);
    assert(zi >= -0.001 && zi <= 1.001);
	}
}

void write_frame_data(ofstream* fp, int n, float* x) {
	for (int i = 0; i < n; i ++) {
	    // Shifted positions are for simple-render
	    float shiftx = x[4*i+0] - 0.5;
	    float shifty = x[4*i+1] - 0.5;
		*fp << "p " << shiftx << " " << shifty << " " << x[4*i+2] << "\n";
	}
	*fp << "t\n";
}

void init_params(sim_param_t* params) {
  // Kevin you will need to fix this
  params->fname = "../outputs/run.txt"; /* File name */
  params->nframes = 300; /* Number of frames */
  params->npframe = 20; /* Steps per frame */
  params->h = 3e-2; /* Particle size */
  params->dt = 1e-3; /* Time step */
  params->rho0 = 1000; /* Reference density */
  params->k = 600; /* Bulk modulus */
  params->mu = 0.5; /* Viscosity */
  params->g = 9.8; /* Gravity strength */
  params->damp = 1; /* Damp */
}

int main(int argc, char* argv[]) {
  
  omp_set_num_threads(16);

	double start = omp_get_wtime(); //benchmarking starts here
	double write_time = 0.0; //time spent on write_frame_data & fp init
	double accel_time = 0.0; //time for compute_accel
	double set_time = 0.0; //time for setParticles
	double frog_time = 0.0; //uh
	double check_time = 0.0; //time for checking state
	double init_time = 0.0;

	double time_start = omp_get_wtime();

	sim_param_t params;
	init_params(&params);
	//sim_state_t* state = place_particles(&params, box_indicator);
	sim_state_t* state = place_particles(&params, sphere_indicator_with_water_plane);
	Grid* grid = new Grid(1.0, 1.0, 1.0, params.h, state);
	grid->setParticles();
	normalize_mass(state, &params, grid);
	printf("mass: %f\n", state->mass);

	init_time += omp_get_wtime() - time_start;
	time_start = omp_get_wtime();

	ofstream* fp = new ofstream();
	fp->open(params.fname);

	write_time += omp_get_wtime() - time_start;


	int nframes = params.nframes;
	int npframe = params.npframe;
	float dt = params.dt;
	int n = state->n;
	printf("Number of Particles: %d\n", n);

	time_start = omp_get_wtime();
	write_frame_data(fp, n, state->x);
	write_time += omp_get_wtime() - time_start;
	time_start = omp_get_wtime();

	compute_accel(state, &params, grid);

	accel_time += omp_get_wtime() - time_start;
	time_start = omp_get_wtime();

	leapfrog_start(state, &params, dt);

	frog_time += omp_get_wtime() - time_start;
	time_start = omp_get_wtime();

	grid->setParticles();

	set_time += omp_get_wtime() - time_start;
	time_start = omp_get_wtime();

	check_state(state);
	
	check_time += omp_get_wtime() - time_start;


	for (int frame = 1; frame < nframes; ++frame) {
    	printf("On iteration %d / %d\n", frame, nframes);
		for (int i = 0; i < npframe; ++i) {

			time_start = omp_get_wtime();
			
			compute_accel(state, &params, grid);

			accel_time += omp_get_wtime() - time_start;
			time_start = omp_get_wtime();

			leapfrog_step(state, &params, dt);

			frog_time += omp_get_wtime() - time_start;
			time_start = omp_get_wtime();

			check_state(state);
			
			check_time += omp_get_wtime() - time_start;
			time_start = omp_get_wtime();

			grid->setParticles();

			set_time += omp_get_wtime() - time_start;
		}

		time_start = omp_get_wtime();

		write_frame_data(fp, n, state->x);

		write_time += omp_get_wtime() - time_start;
	}

	double total_time = omp_get_wtime() - start;

	printf("Total time to run: %f\n",total_time);
	printf("Total time spent writing to disk: %f\n", write_time);
	printf("Total time spent computing accel: %f\n", accel_time);
	printf("Total time spent on init: %f\n", init_time);
	printf("Total time spent checking state: %f\n", check_time);
	printf("Total time spent setting particles: %f\n", set_time);
	printf("Total time spent on leapfrog step: %f\n", frog_time);

	fp->close();
	delete grid;
	free_state(state);
}