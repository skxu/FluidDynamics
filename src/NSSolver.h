
class NSSolver
{
public:
	// takes two particle's displacement vector and the distances
	// of the neighbors around the particle of interest to calc
	// the pressure kernel gradient
	// implements (4) in http://cs.brown.edu/courses/cs195v/asgn/fluids.pdf
	static vec3* calcPressure(vec3 vec, fVec distances, float p, fVec pressures, fVec masses, float rho);

	// uses relative velocity vector (vec is u_neighbor - u_particle) and laplacian of viscosity kernel
	static vec3* calcViscosity(vec3 vec, fVec distances, float mu, vec3 vel, float rho, float m);

};