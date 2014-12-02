#define PI 3.14159265359

typedef struct sim_param_t {
	char* fname = ""; /* File name */
	int nframes = 0; /* Number of frames */
	int npframe = 0; /* Steps per frame */
	float h = 0; /* Particle size */
	float dt = 0; /* Time step */
	float rho0 = 0; /* Reference density */
	float k = 0; /* Bulk modulus */
	float mu = 0; /* Viscosity */
	float g = 0; /* Gravity strength */
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