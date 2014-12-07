#include "Update.h"
#include "Initializer.h"

#include <xmmintrin.h>

using namespace std;

void compute_density(sim_state_t* s, sim_param_t* params, Grid* grid) 
{
  int n = s->n;
  float* rho = s->rho;
  const float* x = s->x;

  float h  = params->h;
  float h2 = h * h;
  float h4 = h2 * h2;
  float h9 = h4 * h4 * h;
  float C  = 315.0 * s->mass / 64.0 / PI / h9;

  memset(rho, 0, n*sizeof(float));
  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < n; i++) {
    vector<int> neighbors;
    grid->getNeighbors(i, neighbors);

    int nidx = 0;
    for (nidx; nidx < neighbors.size(); nidx++) {
      int j = neighbors[nidx];
      __m128 x1 = _mm_loadu_ps(x + 3*i);
      __m128 x2 = _mm_loadu_ps(x + 3*j);
      __m128 r = _mm_sub_ps(x1, x2); 
      __m128 r2 = _mm_mul_ps(r, r);
      float result[4];
      _mm_storeu_ps(result, r2);
      float r2_sum = result[0] + result[1] + result[2];
      float z = h2 - r2_sum;
      if (z > 0) {
        float rho_ij = C*z*z*z;
        rho[i] += rho_ij;
      }
    }

  }
}

void compute_accel(sim_state_t* state, sim_param_t* params, Grid* grid)
{
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
  start_time = omp_get_wtime();
  compute_density(state, params, grid);

  if (DEBUG >= 3) { //Will output ~100 times per iteration
    printf("compute_density took %f\n", omp_get_wtime() - start_time);
  }

  // Gravity
  for (int i = 0; i < n; i++) {
    a[3*i+0] = 0;
    a[3*i+1] = 0;
    a[3*i+2] = -g;
  }

  // Interaction term constants
  float C0 = 45.0 * mass / PI / (h2*h2*h2);

  // Interaction force calculation
  start_time = omp_get_wtime();
  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < n; i++) {
    const float rhoi = rho[i];
    vector<int> neighbors;
    grid->getNeighbors(i, neighbors);
    for (int nidx = 0; nidx < neighbors.size(); nidx++) {
      int j = neighbors[nidx];
      if (i != j) {
        //float dx = x[3*i+0] - x[3*j+0];
        //float dy = x[3*i+1] - x[3*j+1];
        //float dz = x[3*i+2] - x[3*j+2];
        //float r = sqrt(dx*dx + dy*dy + dz*dz);

        __m128 x1 = _mm_loadu_ps(x + 3*i);
        __m128 x2 = _mm_loadu_ps(x + 3*j);
        __m128 dx = _mm_sub_ps(x1, x2); 
        __m128 r2 = _mm_mul_ps(dx, dx);
        float result[4];
        _mm_storeu_ps(result, r2);
        float r = sqrt(result[0] + result[1] + result[2]);

        assert(r > 0); // this shouldn't be 0 do to floating point precision
        float z = h-r;
        const float rhoj = rho[j];
        float w0 = C0/rhoi/rhoj;
        //float wp = w0 * k * (rhoi + rhoj - 2*rho0) * z * z / r / 2.0;
        //float wv = w0 * mu * z;
        //float dvx = v[3*j+0] - v[3*i+0];
        //float dvy = v[3*j+1] - v[3*i+1];
        //float dvz = v[3*j+2] - v[3*i+2];
        
        __m128 wp = _mm_set1_ps(w0 * k * (rhoi + rhoj - 2*rho0) * z * z / r / 2.0);
        __m128 wv = _mm_set1_ps(w0 * mu * z);
        __m128 v1 = _mm_loadu_ps(v + 3*j);
        __m128 v2 = _mm_loadu_ps(v + 3*i);
        __m128 dv = _mm_sub_ps(v1, v2);

        __m128 wvdv = _mm_mul_ps(dv, wv);
        __m128 wpdx = _mm_mul_ps(dx, wp);
        __m128 sum = _mm_add_ps(wvdv, wpdx);

        float final[4]; //the last element is garbage
        _mm_storeu_ps(final, sum);

        a[3*i+0] += final[0]; //(wp*dx + wv*dvx);
        a[3*i+1] += final[1]; //(wp*dy + wv*dvy);
        a[3*i+2] += final[2]; //(wp*dz + wv*dvz);
      }
    }
  }

  #pragma omp barrier
  if (DEBUG >= 3) { //Will output ~100 times per iteration
    printf("interaction force calculation took: %f\n", omp_get_wtime() - start_time);
  }
}

void leapfrog_step(sim_state_t* s, sim_param_t* p, float dt)
{
  const float* a = s->a;
  float* vh      = s->vh;
  float* v       = s->v;
  float* x       = s->x;
  int n          = s->n;

  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < 3*n; i++) {
    vh[i] += a[i] * dt;
    v[i] = vh[i] + a[i] * dt / 2;
    x[i] += vh[i] * dt;
  }

  // Handle reflection across edge and bottom
  reflect_bc(s, p);
}

// Special edge case for first timestep
void leapfrog_start(sim_state_t* s, sim_param_t* p, float dt)
{
  const float* a = s->a;
  float* vh      = s->vh;
  float* v       = s->v;
  float* x       = s->x;
  int n          = s->n;

  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < 3*n; i++) {
    vh[i] = v[i] + a[i] * dt / 2;
    v[i] += a[i] * dt;
    x[i] += vh[i] * dt;
  }

  // Handle reflection across edge and bottom
  reflect_bc(s, p);
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
  if (v[3*i+dim] == 0) return;

  // Scale back distance traveled based on time of collision
  // Alvin thinks it should be vh instead of v...
  float tbounce = (x[3*i+dim] - barrier) / vh[3*i+dim];
  x[3*i+0] -= vh[3*i+0]*(1-damp)*tbounce;
  x[3*i+1] -= vh[3*i+1]*(1-damp)*tbounce;
  x[3*i+2] -= vh[3*i+2]*(1-damp)*tbounce;

  // Reflect position and velocity
  x[3*i+dim]  = 2*barrier - x[3*i+dim];
  v[3*i+dim]  = -v[3*i+dim];
  vh[3*i+dim] = -vh[3*i+dim];

  // Damp the velocities
  v[3*i+0]  *= damp;
  vh[3*i+0] *= damp;
  v[3*i+1]  *= damp;
  vh[3*i+1] *= damp;
  v[3*i+2]  *= damp;
  vh[3*i+2] *= damp;
}

void reflect_bc(sim_state_t* s, sim_param_t* p)
{
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
    if (x[3*i+0] < XMIN) damp_reflect(0, i, XMIN, s, p);
    if (x[3*i+0] >= XMAX) damp_reflect(0, i, XMAX, s, p); // fix this
    if (x[3*i+1] < YMIN) damp_reflect(1, i, YMIN, s, p);
    if (x[3*i+1] >= YMAX) damp_reflect(1, i, YMAX, s, p);
    if (x[3*i+2] < ZMIN) damp_reflect(2, i, ZMIN, s, p);
    if (x[3*i+2] >= ZMAX) damp_reflect(2, i, ZMAX, s, p);
  }
}

void normalize_mass(sim_state_t* s, sim_param_t* param, Grid* grid)
{
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
}