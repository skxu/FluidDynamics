#include "NSSolver.h"

// takes two particle's displacement vector and the distances
// of the neighbors around the particle of interest to calc
// the pressure kernel gradient
// implements (4) in http://cs.brown.edu/courses/cs195v/asgn/fluids.pdf
vec3* NSSolver::calcPressure(vec3 vec, fVec distances, float p, fVec pressures, fVec masses, float rho) {
	vec3* gradient = new vec3();
	float h = 5; //constant particle size?
	for (int i = 0; i < distances.size(); i++) {
		float x = vec.x;
		float y = vec.y;
		float z = vec.z;
		float d = distances[i];
		float m = masses[i];

		float c = (pressures[i] + p) / 2 * m / rho;

		x *= -45 / (pi * pow(h, 6) * d) * pow(h - d, 2);

		y *= -45 / (pi * pow(h, 6) * d) * pow(h - d, 2);

		z *= -45 / (pi * pow(h, 6) * d) * pow(h - d, 2);

		gradient->x += c * x;
		gradient->y += c * y;
		gradient->z += c * z;

		//can probably bring some terms out of the for loop to optimize
	}
	return gradient;
}
// uses relative velocity vector (vec is u_neighbor - u_particle) and laplacian of viscosity kernel
vec3* NSSolver::calcViscosity(vec3 vec, fVec distances, float mu, vec3 vel, float rho, float m) {
	vec3* visc = new vec3();
	fVec masses;
	float h = 5; //constant particle size?
	for (int i = 0; i < distances.size(); i++) {
		float x = vec.x;
		float y = vec.y;
		float z = vec.z;
		float d = distances[i];
		float m = masses[i];

		x *= m * 45 / (pi * pow(h, 6)) * (h - d);

		y *= m * 45 / (pi * pow(h, 6)) * (h - d);

		z *= m * 45 / (pi * pow(h, 6)) * (h - d);

		visc->x += x;
		visc->y += y;
		visc->z += z;
	}
	visc->x *= mu / rho;
	visc->y += mu / rho;
	visc->z += mu / rho;

	return visc;
}