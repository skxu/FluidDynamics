#include "vec3.h"

vec3::vec3()
{
	memset(this, 0, sizeof(vec3));
}

vec3::vec3(float x, float y, float z) :
x(x), y(y), z(z){}

vec3::vec3(const vec3 &other)
{
	memcpy(this, &other, sizeof(vec3));
}