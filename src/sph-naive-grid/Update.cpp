#include "Update.h"
#include "Initializer.h"

#include <xmmintrin.h>
#include <pmmintrin.h>
#include <iostream>

using namespace std;

void compute_density(sim_state_t* s, sim_param_t* params, Grid* grid) 
{
  if (DEBUG >= 4) {
    std::cout << "Entering compute_density" << std::endl << std::flush;
  }
  int n = s->n;
  float* rho = s->rho;
  const float* x = s->x;

  float h  = params->h;
  float h2 = h * h;
  float h4 = h2 * h2;
  float h9 = h4 * h4 * h;
  float C  = 315.0 * s->mass / 64.0 / PI / h9;

  for (int i = 0; i < n; i++) {
    
    float xi_f = x[4*i+0];
    float yi_f = x[4*i+1];
    float zi_f = x[4*i+2];

    __m128 xi = _mm_load_ps(x+4*i);
    float rhoi = 0.0;
    vector<int>* neighbors = grid->getNeighbors(i);
    int nidx = 0;
	/*
    for (nidx; nidx + 4 < neighbors->size(); nidx+= 4) {

      int j = (*neighbors)[nidx];
      int j2 = (*neighbors)[nidx+1];
      int j3 = (*neighbors)[nidx+2];
      int j4 = (*neighbors)[nidx+3];

      __m128 xj = _mm_load_ps(x+4*j);
      __m128 dx = _mm_sub_ps(xi,xj);
      __m128 r = _mm_mul_ps(dx, dx);
      
      __m128 xj2 = _mm_load_ps(x+4*j2);
      __m128 dx2 = _mm_sub_ps(xi, xj2);
      __m128 r2 = _mm_mul_ps(dx2, dx2);

      __m128 xj3 = _mm_load_ps(x+4*j3);
      __m128 dx3 = _mm_sub_ps(xi, xj3);
      __m128 r3 = _mm_mul_ps(dx3, dx3);

      __m128 xj4 = _mm_load_ps(x+4*j4);
      __m128 dx4 = _mm_sub_ps(xi, xj4);
      __m128 r4 = _mm_mul_ps(dx4, dx4);


      //horizontal adds
      __m128 sum1 = _mm_hadd_ps(r, r2);
      __m128 sum2 = _mm_hadd_ps(r3, r4);
      __m128 sum3 = _mm_hadd_ps(sum1, sum2);


      __m128 hh2 = _mm_set1_ps(h2);

      __m128 z = _mm_sub_ps(hh2, sum3); 
      
      __m128 CC = _mm_set1_ps(C);

      __m128 final = _mm_mul_ps(CC, z);
      final = _mm_mul_ps(final, z);
      final = _mm_mul_ps(final, z);

      float results[4];
      _mm_storeu_ps(results, final);

      for (int i = 0; i < 4; i++) {
        rhoi += results[i];
      }
      /*
      float dx = xi - x[4*j+0];
      float dy = yi - x[4*j+1];
      float dz = zi - x[4*j+2];
      float r2 = dx*dx + dy*dy + dz*dz;
      float z = h2-r2;
      float rho_ij = C*z*z*z;
      rhoi += rho_ij;
      

    }*/

    for (nidx; nidx < neighbors->size(); nidx++) {
      int j = (*neighbors)[nidx];
      float dx = xi_f - x[4*j+0];
      float dy = yi_f - x[4*j+1];
      float dz = zi_f - x[4*j+2];
      float r2 = dx*dx + dy*dy + dz*dz;
      float z = h2-r2;
      float rho_ij = C*z*z*z;
      rhoi += rho_ij;
    }
    rho[i] = rhoi;
  }

  if (DEBUG >= 4) {
    std::cout << "Exiting compute_density" << std::endl << std::flush;
  }
}

void compute_accel(sim_state_t* state, sim_param_t* params, Grid* grid)
{
  if (DEBUG >= 4) {
    std::cout << "Entering compute_accel" << std::endl << std::flush;
  }

  // Get parameters
  const float h    = params->h;
  const float rho0 = params->rho0;
  const float k    = params->k;
  const float mu   = params->mu;
  const float g    = params->g;
  const float mass = state->mass;
  const float h2   = h*h;
  const float* rho = state->rho;
  const float* x   = state->x;
  const float* v   = state->v;
  float* a         = state->a;
  int n            = state->n;

  //for benchmarking
  double start_time;

  // Compute Densities
  if (DEBUG >= 3) {
    start_time = omp_get_wtime();
  }
  compute_density(state, params, grid);

  if (DEBUG >= 3) {
    //~100 outputs per iteration
    printf("compute_density took: %fs\n",omp_get_wtime() - start_time);
  }

  // Interaction term constants
  float C0 = 45.0 * mass / PI / (h2*h2*h2);

  // Interaction force calculation
  if (DEBUG >= 3) {
    start_time = omp_get_wtime();
  }
  for (int i = 0; i < n; i++) {
    float xi = x[4*i+0];
    float yi = x[4*i+1];
    float zi = x[4*i+2];
    float vxi = v[4*i+0];
    float vyi = v[4*i+1];
    float vzi = v[4*i+2];
    float ax = 0;
    float ay = 0;
    float az = -g;
    const float rhoi = rho[i];
    vector<int>* neighbors = grid->getNeighbors(i);
	for (int nidx = 0; nidx < neighbors->size(); nidx++) {
      int j = (*neighbors)[nidx];
      if (i != j) {
        float dx = xi - x[4*j+0];
        float dy = yi - x[4*j+1];
        float dz = zi - x[4*j+2];
        float r = sqrt(dx*dx + dy*dy + dz*dz);
        assert(r > 0); // this shouldn't be 0 do to floating point precision
        float z = h-r;
        const float rhoj = rho[j];
        float w0 = C0/rhoi/rhoj;
        float wp = w0 * k * (rhoi + rhoj - 2*rho0) * z * z / r / 2.0;
        float wv = w0 * mu * z;
        float dvx = v[4*j+0] - vxi;
        float dvy = v[4*j+1] - vyi;
        float dvz = v[4*j+2] - vzi;
        ax += (wp*dx + wv*dvx);
        ay += (wp*dy + wv*dvy);
        az += (wp*dz + wv*dvz);
      }
    }
    a[4*i+0] = ax;
    a[4*i+1] = ay;
    a[4*i+2] = az;
  }

  
  if (DEBUG >= 3) {
    printf("interaction force calculation took: %fs\n", omp_get_wtime() - start_time);
  }
  if (DEBUG >= 4) {
    std::cout << "Exiting compute_accel" << std::endl << std::flush;
  }
}

void leapfrog_step(sim_state_t* s, sim_param_t* p, float dt)
{
  if (DEBUG >= 4) {
    std::cout << "Entering leapfrog_step" << std::endl << std::flush;
  }

  const float* a = s->a;
  float* vh      = s->vh;
  float* v       = s->v;
  float* x       = s->x;
  int n          = s->n;

  for (int i = 0; i < 4*n; i++) {
    vh[i] += a[i] * dt;
    v[i] = vh[i] + a[i] * dt / 2;
    x[i] += vh[i] * dt;
  }

  // Handle reflection across edge and bottom
  reflect_bc(s, p);

  if (DEBUG >= 4) {
    std::cout << "Exiting leapfrog_step" << std::endl << std::flush;
  }
}

// Special edge case for first timestep
void leapfrog_start(sim_state_t* s, sim_param_t* p, float dt)
{
  if (DEBUG >= 4) {
    std::cout << "Entering leapfrog_start" << std::endl << std::flush;
  }

  const float* a = s->a;
  float* vh      = s->vh;
  float* v       = s->v;
  float* x       = s->x;
  int n          = s->n;

  for (int i = 0; i < 4*n; i++) {
    vh[i] = v[i] + a[i] * dt / 2;
    v[i] += a[i] * dt;
    x[i] += vh[i] * dt;
  }

  // Handle reflection across edge and bottom
  reflect_bc(s, p);

  if (DEBUG >= 4) {
    std::cout << "Exiting leapfrog_start" << std::endl << std::flush;
  }
}

// Handles boundary collisions
// dim = 0 (x), dim = 1 (y), dim = 2 (z)
void damp_reflect(int dim, int i, float barrier,
                  sim_state_t* s, sim_param_t* p)
{

  float* vh  = s->vh;
  float* v   = s->v;
  float* x   = s->x;
  float damp = p->damp;

  // Particle doesn't move -- no update
  if (v[4*i+dim] == 0) return;

  // Scale back distance traveled based on time of collision
  // Alvin thinks it should be vh instead of v...
  float tbounce = (x[4*i+dim] - barrier) / vh[4*i+dim];
  x[4*i+0] -= vh[4*i+0]*(1-damp)*tbounce;
  x[4*i+1] -= vh[4*i+1]*(1-damp)*tbounce;
  x[4*i+2] -= vh[4*i+2]*(1-damp)*tbounce;

  // Reflect position and velocity
  x[4*i+dim]  = 2*barrier - x[4*i+dim];
  v[4*i+dim]  = -v[4*i+dim];
  vh[4*i+dim] = -vh[4*i+dim];

  // Damp the velocities
  v[4*i+0]  *= damp;
  vh[4*i+0] *= damp;
  v[4*i+1]  *= damp;
  vh[4*i+1] *= damp;
  v[4*i+2]  *= damp;
  vh[4*i+2] *= damp;

}

void reflect_bc(sim_state_t* s, sim_param_t* p)
{
  if (DEBUG >= 4) {
    std::cout << "Entering reflect_bc" << std::endl << std::flush;
  }

  const float XMIN = 0.0;
  const float XMAX = 1.0;
  const float YMIN = 0.0;
  const float YMAX = 1.0;
  const float ZMIN = 0.0;
  const float ZMAX = 1.0;

  float* vh = s->vh;
  float* v  = s->v;
  float* x  = s->x;
  int n     = s->n;
  for (int i = 0; i < n; i++) {
    if (x[4*i+0] < XMIN) damp_reflect(0, i, XMIN, s, p);
    if (x[4*i+0] >= XMAX) damp_reflect(0, i, XMAX, s, p); // fix this
    if (x[4*i+1] < YMIN) damp_reflect(1, i, YMIN, s, p);
    if (x[4*i+1] >= YMAX) damp_reflect(1, i, YMAX, s, p);
    if (x[4*i+2] < ZMIN) damp_reflect(2, i, ZMIN, s, p);
    if (x[4*i+2] >= ZMAX) damp_reflect(2, i, ZMAX, s, p);
  }

  if (DEBUG >= 4) {
    std::cout << "Exiting reflect_bc" << std::endl << std::flush;
  }
}

void normalize_mass(sim_state_t* s, sim_param_t* param, Grid* grid)
{
  //if (DEBUG >= 4) {
  //  std::cout << "Entering normalize_mass" << std::endl << std::flush;
  //}

  s->mass = 1;
  compute_density(s, param, grid);
  float rho0 = param->rho0;
  float rho2s = 0;
  float rhos = 0;
  for (int i = 0; i < s->n; i++) {
    rho2s += (s->rho[i])*(s->rho[i]);
    rhos += s->rho[i];
  }
  s->mass = ( rho0*rhos / rho2s );

  if (DEBUG >= 4) {
    std::cout << "Exiting normalize_mass" << std::endl << std::flush;
  }
}