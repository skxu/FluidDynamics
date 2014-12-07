#ifndef __VARIABLES_H__
#define __VARIABLES_H__

#define PI 3.14159265359
#define DEBUG 1 //debug output level (0:None 1:Minimal 2:Detailed)
#define MAX_INIT_VEL 0.01

typedef struct sim_param_t {
  const char* fname; /* File name */
  int nframes;       /* Number of frames   */
  int npframe;       /* Steps per frame    */
  float h;           /* Particle size      */
  float dt;          /* Time step          */
  float rho0;        /* Reference density  */
  float k;           /* Bulk modulus       */
  float mu;          /* Viscosity          */
  float g;           /* Gravity strength   */
  float damp;        /* Damp on collisions */
} sim_param_t;

typedef struct sim_state_t {
  int n;         /* Number of particles    */
  float mass;    /* Particle mass          */
  float* rho;    /* Densities              */
  float* x;      /* Positions              */
  float* vh;     /* Velocities (half step) */
  float* v;      /* Velocities (full step) */
  float* a;      /* Acceleration           */
} sim_state_t;

sim_state_t* alloc_state(int n);
void free_state(sim_state_t* s);

#endif