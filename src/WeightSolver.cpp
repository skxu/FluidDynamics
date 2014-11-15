#include "WeightSolver.h"

/* Give the distances of particle's neighbors
   Return the weights based on the B-Spline Kernel
*/
fVec* WeightSolver::CalculateWeights(fVec distances)
{
	fVec* weights = new fVec();
	for each(float d in distances)
	{
		float w = 0.0f;
		float q = d; //TODO: CHANGE ME PLEASE!
		if (q < 0.5f)
		{
			w = 1 - 6 * pow(q, 2) + 6 * pow(q,3);
		}
		else if (q < 1)
		{
			w = 2 * pow(1.0f - q, 3);
		}
		w *= 8 / pi;
		weights->push_back(w);
	}
	return weights;
}