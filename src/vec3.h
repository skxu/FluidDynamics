#include <string.h>
#ifndef VEC3_INCLUDE
#define VEC3_INCLUDE
struct vec3 {
	float x, y, z;

	vec3();

	vec3(float x, float y, float z);

	vec3(const vec3 &other);

	vec3 operator*(float scalar);

	vec3 operator+(vec3 other);

	vec3 operator+=(vec3 other);
};
#endif