#include "VaryingPVKernel.h"
#include "Constants.h"

/** 
 * Equations: http://cs.brown.edu/courses/cs195v/asgn/fluids.pdf  *
 * See page 5 & 6 of above link.                                  *
 **/

/** 
 * Returns the value W(r,h).        *
 * Assumes that norm(r) <= h.       *
 * This computes term for PRESSURE. *
 * Return type: float               *
 **/
float VaryingPVKernel::Value(vec3 r, float h)
{
  return 15.0 / (pi * pow(h, 6)) * pow(h - r.L2Distance(), 3);
}

/**
 * Returns the value grad(W(r,h)).  *
 * Assumes that norm(r) <= h.       *
 * This computes term for PRESSURE. *
 * Return type: vec3                *
 **/
vec3 VaryingPVKernel::Gradient(vec3 r, float h)
{
  float multiplier = -45.0 / (pi * pow(h, 6)) * pow(h - r.L2Distance(), 2);
  return r * (multiplier / r.L2Distance());
}

/**
 * Returns the value grad^2(W(r,h)). *
 * Assumes that norm(r) <= h.        *
 * This computes term for VISCOSITY. *
 * Return type: float                *
 **/
float VaryingPVKernel::Lagragian(vec3 r, float h)
{
  return 45.0 / (pi * pow(h, 6)) * (h - r.L2Distance());
}
