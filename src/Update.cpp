#include "Update.h"

void compute_density(sim_state_t* s, sim_param_t* params) 
{
  int n = s->n;
  float* rho = s->rho;
  const float* x = s->x;

  float h  = params->h;
  float h2 = h*h;
  float h8 = (h2*h2)*(h2*h2);
  float C  = 4 * s->mass / PI / h8;

  memset(rho, 0, n*sizeof(float));
  for (int i = 0; i < n; ++i) {
    rho[i] += 4 * s->mass / PI / h2;
    for (int j = i+1; j < n; ++j) {
      float dx = x[3*i+0] - x[3*j+0];
      float dy = x[3*i+1] - x[3*j+1];
      float dz = x[3*i+2] - x[3*j+2];
      float r2 = dx*dx + dy*dy + dz*dz;
      float z = h2-r2;
      if (z > 0) {
        float rho_ij = C*z*z*z;
        rho[i] += rho_ij;
        rho[j] += rho_ij;
      }
    }
  }
}

void compute_accel(sim_state_t* state, sim_param_t* params)
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

  // Compute Densities
  compute_density(state, params);

  // Gravity
  for (int i = 0; i < n; ++i) {
    a[3*i+0] = 0;
    a[3*i+1] = 0;
    a[3*i+2] = -g;
  }

  // Interaction term constants
  float C0 = mass / PI / (h2*h2);
  float Cp = 15*k;
  float Cv = -40*mu;

  // Interaction force calculation
  for (int i = 0; i < n; ++i) {
    const float rhoi = rho[i];
    // To do - grid based to avoid the O(n^2) loop, add back Wesley's and Omer's code
    for (int j = i+1; j < n; ++j) {
      float dx = x[3*i+0] - x[3*j+0];
      float dy = x[3*i+1] - x[3*j+1];
      float dz = x[3*i+2] - x[3*j+2];
      float r2 = dx*dx + dy*dy + dz*dz;
      if (r2 < h2) {
        const float rhoj = rho[j];
        float q = sqrt(r2)/h;
        float u = 1-q;
        float w0 = C0 * u/rhoi/rhoj;
        float wp = w0 * Cp * (rhoi + rhoj - 2*rho0) * u/q;
        float wv = w0 * Cv;
        float dvx = v[3*i+0] - v[3*j+0];
        float dvy = v[3*i+1] - v[3*j+1];
        float dvz = v[3*i+2] - v[3*j+2];
        a[3*i+0] += (wp*dx + wv*dvx);
        a[3*i+1] += (wp*dy + wv*dvy);
        a[3*i+2] += (wp*dz + wv*dvz);
        a[3*j+0] -= (wp*dx + wv*dvx);
        a[3*j+1] -= (wp*dy + wv*dvy);
        a[3*j+2] -= (wp*dz + wv*dvz);
      }
    }
  }
}

void leapfrog_step(sim_state_t* s, double dt)
{
  const float* a = s->a;
  float* vh      = s->vh;
  float* v       = s->v;
  float* x       = s->x;
  int n          = s->n;

  for (int i = 0; i < 3*n; ++i) {
    vh[i] += a[i] * dt;
  }
  for (int i = 0; i < 3*n; ++i) {
    v[i] = vh[i] + a[i] * dt / 2;
  }
  for (int i = 0; i < 3*n; ++i) {
    x[i] += vh[i] * dt;
  }

  // Handle reflection across edge and bottom
  reflect_bc(s);
}

// Special edge case for first timestep
void leapfrog_start(sim_state_t* s, double dt)
{
  const float* a = s->a;
  float* vh      = s->vh;
  float* v       = s->v;
  float* x       = s->x;
  int n          = s->n;
  for (int i = 0; i < 3*n; ++i) {
    vh[i] += v[i] + a[i] * dt / 2;
  }
  for (int i = 0; i < 3*n; ++i) {
    v[i] = a[i] * dt;
  }
  for (int i = 0; i < 3*n; ++i) {
    x[i] += vh[i] * dt;
  }

  // Handle reflection across edge and bottom
  reflect_bc(s);
}

// Handles boundary collissions
// dim = 0 (x), dim = 1 (y), dim = 2 (z)
void damp_reflect(int dim, float barrier,
                  float* x, float* v, float* vh) {
  const float DAMP = 0.75;

  // Particle doesn't move -- no update
  if (v[dim] == 0) return;

  // Scale back distance traveled based on time of collision
  float tbounce = (x[dim] - barrier)  / v[dim];
  x[0] -= v[0]*(1-DAMP)*tbounce;
  x[1] -= v[1]*(1-DAMP)*tbounce;
  x[2] -= v[2]*(1-DAMP)*tbounce;

  // Reflect position and velocity
  x[dim]  = 2*barrier - x[dim];
  v[dim]  = -v[dim];
  vh[dim] = -vh[dim];

  // Damp the velocities
  v[0]  *= DAMP;
  vh[0] *= DAMP;
  v[1]  *= DAMP;
  vh[1] *= DAMP;
  v[2]  *= DAMP;
  vh[2] *= DAMP;
}

void reflect_bc(sim_state_t* s) {
  const float XMIN = 0.0;
  const float XMAX = 1.0;
  const float YMIN = 0.0;
  const float YMAX = 1.0;
  const float ZMIN = 0.0;
  // No ZMAX for open top container

  float* vh = s->vh;
  float* v  = s->v;
  float* x  = s->x;
  int n     = s->n;
  for (int i = 0; i < n; i++, x+=3, v+=3, vh+=3) {
    if (x[0] < XMIN) damp_reflect(0, XMIN, x, v, vh);
    if (x[0] > XMAX) damp_reflect(0, XMAX, x, v, vh);
    if (y[0] < YMIN) damp_reflect(1, YMIN, x, v, vh);
    if (y[0] > YMAX) damp_reflect(1, YMAX, x, v, vh);
    if (z[0] < ZMIN) damp_reflect(2, ZMIN, x, v, vh);
  }
}