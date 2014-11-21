#ifndef VARYINGPVKERNEL_H
#define VARYINGPVKERNEL_H

#include "Constants.h"
#include <math.h>
#include "vec3.h"

class VaryingPVKernel
{
public:
  static float Value     (vec3 r, float h);
  static vec3  Gradient  (vec3 r, float h);
  static float Lagragian (vec3 r, float h);
};

#endif // VARYINGPVKERNEL_H
