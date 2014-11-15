#include "WeightSolver.h"

/* B-Spline Kernel is:
 * W(q) = {   8/pi * (1 - 6q^2 + 6q^3)  IF 0.0 <= q < 0.5
 *            8/pi * (2 * (1-q)^3)      IF 0.5 <= q < 1.0
 *            0                         IF 1.0 < q }
 *
 * First derivative is:
 * W'(q) = {  8/pi * (-12q + 18q^2)     IF 0.0 <= q < 0.5
 *            8/pi * (-6 * (1-q)^2)     IF 0.5 <= q < 1.0
 *            0                         IF 1.0 < q }
 *
 * Second derivative is:
 * W''(q) = { 8/pi * (-12 + 36q)        IF 0.0 <= q < 0.5
 *            8/pi * (12 * (1-q))       IF 0.5 <= q < 1.0
 *            0                         IF 1.0 < q }
 */

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

/* Give the first derivative of distances of particle's neighbors
   Return the weights based on the B-Spline Kernel
*/
fVec* WeightSolver::CalculateFirstDerivativesWeights(fVec distances)
{
	fVec* weights = new fVec();
	for each(float d in distances)
	{
		float w = 0.0f;
		float q = d; //TODO: CHANGE ME PLEASE!
		if (q < 0.5f)
		{
			w = -12 * q + 18 * pow(q, 2);
		}
		else if (q < 1)
		{
			w = -6 * pow(1-q, 2);
		}
		w *= 8 / pi;
		weights->push_back(w);
	}
	return weights;
}

/* Give the second derivative of distances of particle's neighbors
   Return the weights based on the B-Spline Kernel
*/
fVec* WeightSolver::CalculateSecondDerivativesWeights(fVec distances)
{
	fVec* weights = new fVec();
	for each(float d in distances)
	{
		float w = 0.0f;
		float q = d; //TODO: CHANGE ME PLEASE!
		if (q < 0.5f)
		{
			w = -12 + 36 * q;
		}
		else if (q < 1)
		{
			w = 12 * (1 - q);
		}
		w *= 8 / pi;
		weights->push_back(w);
	}
	return weights;
}