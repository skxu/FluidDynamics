#include <stdio.h>
#include <string.h>
struct vec3 {
	float x, y, z;

	vec3()
	{
		memset(this, 0, sizeof(vec3));
	}
	
	vec3(float x, float y, float z):
		x(x), y(y), z(z){}

	vec3(const vec3 &other)
	{
		memcpy(this, &other, sizeof(vec3));
	}
};

union Particle {
	struct {
		vec3 pos;
		vec3 vel;
		vec3 acc;
		float density, pressure;
	};
	/* we need this?
	struct {
		float pos_x, pos_y, pos_z;
		float vel_x, vel_y, vel_z;
		float acc_x, acc_y, acc_z;
		float density, pressure;
	}*/

	Particle()
	{
		memset(this, 0, sizeof(Particle));
	}

	Particle(const Particle &other)
	{
		memcpy(this, &other, sizeof(Particle));
	}
};