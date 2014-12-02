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

vec3 vec3::operator*(float scalar)
{
	return vec3(x*scalar, y*scalar, z*scalar);
}

vec3 vec3::operator+(vec3 other)
{
	return vec3(x+other.x, y+other.y, z+other.z);
}

vec3 vec3::operator+=(vec3 other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

vec3 vec3::operator-(vec3 other)
{
	return vec3(x-other.x, y-other.y, z-other.z);
}

vec3 vec3::operator-=(vec3 other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

float vec3::SumSquares()
{
	return x*x + y*y + z*z;
}

float vec3::L2Distance()
{
	return sqrt(SumSquares());
}