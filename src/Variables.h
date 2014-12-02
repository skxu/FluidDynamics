#ifndef __VARIABLES_H__
#define __VARIABLES_H__

#define PI 3.14159265359

typedef struct sim_param_t {
	char* fname = "run.out"; /* File name */
	int nframes = 5; /* Number of frames */
	int npframe = 100; /* Steps per frame */
	float h = 5e-2; /* Particle size */
	float dt = 1e-4; /* Time step */
	float rho0 = 1000; /* Reference density */
	float k = 1e3; /* Bulk modulus */
	float mu = 0.1; /* Viscosity */
	float g = 9.8; /* Gravity strength */
} sim_param_t;

typedef struct sim_state_t {
	int n; /* Number of particles */
	float mass; /* Particle mass */
	float* rho; /* Densities */
	float* x; /* Positions */
	float* vh; /* Velocities (half step) */
	float* v; /* Velocities (full step) */
	float* a; /* Acceleration */
} sim_state_t;
sim_state_t* alloc_state(int n);
void free_state(sim_state_t* s);

#endif