#include "solver.h"

#define pVec std::vector<Particle*>
#define fVec std::vector<float>

Solver::Solver(
	pVec particleList)
{
	currentParticles = particleList;
    grid = new Grid(420,420,420,4); //What are our parameters?!
    grid->setParticles(currentParticles);
}

/* Calculate density and pressure of a particle based on neighbors
*/
Particle* Solver::UpdateDensityPressure(
	Particle* particle, 
	pVec* neighborList, 
	fVec weights)
{
   //TODO
	Particle* p = new Particle(*particle);
	p->neighbors = neighborList;

	p->pressure = 0;
	p->density = 0;
	for (int i = 0; i < neighborList->size(); i++)
	{
		Particle* n = (*neighborList)[i];
		float w = weights[i];
		p->pressure += n->pressure * w;
		p->density += n->density * w;
	}
	return p;
}

void Solver::UpdateAll()
{
	//TODO
	pVec* updatedParticles = new pVec();

	for (std::vector<int>::size_type i = 0; i != currentParticles.size(); i++) 
	{
		Particle* p = currentParticles[i];
		//TODO: Get neighbors
		pVec* neighbors = CalcNeighbors(p);
		fVec weights = fVec();
		
		//UPDATE the particle
		Particle* updatedParticle = UpdateDensityPressure(p, neighbors, weights);
		updatedParticles->push_back(updatedParticle);

	}

	currentParticles = *updatedParticles;
}

void Solver::SetParticles(
	pVec* particleList)
{
	currentParticles = *particleList;
}

pVec Solver::GetParticles()
{
	return currentParticles;
}


pVec* Solver::CalcNeighbors(
	Particle* particle)
{

	pVec * neighbors = new pVec();
	//TODO SOMEHOW GET NEIGHBORS YEAH
    grid->setNeighbors(p);
    neighbors = grid->getNeighbors(p);
	return neighbors;
}


// uses distances to calculate the weights and take 
// dot product between neighbors' mass and weights
// implements (9.6) from fluid_notes.pdf
float Solver::calcDensityField(fVec* first, fVec* masses) {
    float rho = 0.0f;
	fVec distances;
    first = WeightSolver::CalculateWeights(distances);
    for (int i = 0; i < masses->size(); i++) {
        rho += (*first)[i]*(*masses)[i];
    }
    
    return rho;
}

// calculates the pressure using the difference in 
// particle density and environmental density
// implements the ideal gas state equation

fVec* Solver::calcPressure(float k, float rho, fVec* densities) {
    fVec* p = new fVec();
    float pressure = 0.0f;
    for each (float density in (*densities)) {
        pressure = k * (density - rho);
        p->push_back(pressure);
    }    
    
    return p;
}

// takes two particle's displacement vector and the distances
// of the neighbors around the particle of interest to calc
// the pressure kernel gradient
// implements (4) in http://cs.brown.edu/courses/cs195v/asgn/fluids.pdf
vec3* Solver::calcPressure(vec3 vec, fVec distances, float p, fVec pressures, fVec masses, float rho) {
    vec3* gradient = new vec3(); 
    float h = 5; //constant particle size?
    for (int i = 0; i < distances.size(); i++) {
        float x = vec.x;
        float y = vec.y;
        float z = vec.z;
        float d = distances[i];
        float m = masses[i];
        
        float c = (pressures[i] + p) / 2 * m / rho;
        
        x *= -45/(pi * pow(h, 6) * d) * pow(h - d, 2);
        
        y *= -45/(pi * pow(h, 6) * d) * pow(h - d, 2);

        z *= -45/(pi * pow(h, 6) * d) * pow(h - d, 2);

        gradient->x += c * x;
        gradient->y += c * y;
        gradient->z += c * z;
        
        //can probably bring some terms out of the for loop to optimize
    }
    return gradient;
}
// uses relative velocity vector (vec is u_neighbor - u_particle) and laplacian of viscosity kernel
vec3* Solver::calcViscosity(vec3 vec, fVec distances, float mu, vec3 vel, float rho, float m) {
    vec3* visc = new vec3(); 
	fVec masses;
    float h = 5; //constant particle size?
    for (int i = 0; i < distances.size(); i++) {
        float x = vec.x;
        float y = vec.y;
        float z = vec.z;
        float d = distances[i];
        float m = masses[i];
        
        x *= m * 45/(pi * pow(h, 6)) * (h - d);
        
        y *= m * 45/(pi * pow(h, 6)) * (h - d);

        z *= m * 45/(pi * pow(h, 6)) * (h - d);

        visc->x += x;
        visc->y += y;
        visc->z += z;        
    }
    visc->x *= mu / rho;
    visc->y += mu / rho;
    visc->z += mu / rho;   
    
    return visc;
}

float Solver::l2Norm(vec3 vec) {
	return 0.0f;
}