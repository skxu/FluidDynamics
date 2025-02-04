#include "Particle.h"
#include "Constants.h"

class PDSolver
{
private:
	// uses distances to calculate the weights and take 
	// dot product between neighbors' mass and weights
	// implements (9.6) from fluid_notes.pdf
	static float calcDensity(Particle* p, float h);


	// calculates the pressure using the difference in 
	// particle density and environmental density
	// implements the ideal gas state equation
	static float calcPressure(Particle* p);

public:
	static void UpdateDensities(pVec* particles, float h);

	static void UpdatePressures(pVec* particles);

};