#ifndef KERNEL_H
#define KERNEL_H

#include "vec3.h"

class Kernel
{
public:
  static float Value     (vec3 r, float h);
  static vec3  Gradient  (vec3 r, float h);
  static float Lagragian (vec3 r, float h);
};

#endif // KERNEL_H