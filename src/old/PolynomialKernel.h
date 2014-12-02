#ifndef POLYNOMIALKERNEL_H
#define POLYNOMIALKERNEL_H

#include "Constants.h"
#include "vec3.h"
#include <math.h>

class PolynomialKernel 
{
public:
  static float Value     (vec3 r, float h);
  static vec3  Gradient  (vec3 r, float h);
  static float Lagragian (vec3 r, float h);
};

#endif // POLYNOMIALKERNEL_H
