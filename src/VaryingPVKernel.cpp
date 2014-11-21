#include "VaryingPVKernel.h"
#include "Constants.h"

/** 
 * Equations: http://cs.brown.edu/courses/cs195v/asgn/fluids.pdf  *
 * See page 5 & 6 of above link.                                  *
 **/

/** 
 * Returns the value W(r,h).  *
 * Assumes that norm(r) <= h. *
 * Return type: float         *
 **/
float PolynomialKernel::Value(vec3 r, float h)
{
  return 15.0 / (pi * pow(h, 6)) * pow(h - r.L2Distance(), 3);
}

/**
 * Returns the value grad(W(r,h)). *
 * Assumes that norm(r) <= h.      *
 * Return type: vec3               *
 **/
vec3 PolynomialKernel::Gradient(vec3 r, float h)
{
  float multiplier = -45.0 / (pi * pow(h, 6)) * pow(h - r.L2Distance(), 2);
  return r * (multiplier / r.L2Distance());
}

/**
 * Returns the value grad^2(W(r,h)). *
 * Assumes that norm(r) <= h.        *
 * Return type: float                *
 **/
float PolynomialKernel::Lagragian(vec3 r, float h)
{
  return 45.0 / (pi * pow(h, 6)) * (h - r.L2Distance());
}
