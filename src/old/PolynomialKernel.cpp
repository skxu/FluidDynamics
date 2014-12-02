#include "PolynomialKernel.h"

/** 
 * Equations: http://image.diku.dk/projects/media/kelager.06.pdf  *
 * See page 16 of above link.                                     *
 **/

/** 
 * Returns the value W(r,h).  *
 * Assumes that norm(r) <= h. *
 * Return type: float         *
 **/
float PolynomialKernel::Value(vec3 r, float h)
{
  return 315.0 / (64 * pi * pow(h, 9)) * pow(pow(h, 2) - r.SumSquares(), 3);
}

/**
 * Returns the value grad(W(r,h)). *
 * Assumes that norm(r) <= h.      *
 * Return type: vec3               *
 **/
vec3 PolynomialKernel::Gradient(vec3 r, float h)
{
  float multiplier = -945.0 / (32 * pi * pow(h, 9)) * pow(pow(h, 2) - r.SumSquares(), 2);
  return r * multiplier;
}

/**
 * Returns the value grad^2(W(r,h)). *
 * Assumes that norm(r) <= h.        *
 * Return type: float                *
 **/
float PolynomialKernel::Lagragian(vec3 r, float h)
{
  return -945.0 / (32 * pi * pow(h, 9)) * (pow(h, 2) - r.SumSquares()) * (3 * pow(h, 2) - 7 * r.SumSquares());
}
