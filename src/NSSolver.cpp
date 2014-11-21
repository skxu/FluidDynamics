#include "NSSolver.h"


NSSolver::NSSolver()
{
	pressure = viscosity = interactive = gravity = true;
}

pVec* NSSolver::solve(pVec* particles, float h)
{
	for (Particle* p: *particles)
	{
		p->acc = vec3();
		
		// Neighbor accelerations pressure and viscosity

		for (int i = 0; i < p->neighbors->size(); i++)
		{
			Particle* n = (*p->neighbors)[i];
			vec3* ndir = (*p->neighborDirections)[i];

			if (pressure)
			{
				float pij = -1.0f * n->mass * (p->pressure / pow(p->density, 2) + n->pressure / pow(n->density, 2));
				p->acc += PolynomialKernel::Gradient(*ndir, h) * pij;
			}

			if (viscosity)
			{
				float umppWW = MU * n->mass * PolynomialKernel::Lagragian(*ndir, h) / (p->density * n->density);
				p->acc += (n->vel - p->vel) * umppWW;
			}
		}

		if (interactive)
		{
			//duck
		}

		if (gravity)
		{
			p->acc.z += GRAVITY;
		}
	}
	return particles;
}