#include "solver.h"


Solver::Solver(
	pVec particleList,
	float hValue)
{
	cutoff = hValue;
	currentParticles = particleList;
    grid = new Grid(10,10,10,1); //What are our parameters?!
    grid->setParticles(currentParticles);

    //initialize solvers
    nsSolver = new NSSolver();
    pdSolver = new PDSolver();

}

void Solver::UpdateAll()
{

	//Calculate neighbors
	grid->setNeighbors(currentParticles);

	//Update densities
	pdSolver->UpdateDensities(&currentParticles, cutoff);

	//Update pressures
	pdSolver->UpdatePressures(&currentParticles);

	//Solve navier-stokes
	currentParticles = * nsSolver->solve(&currentParticles, cutoff);

	ParticleMover::updateVelocities(&currentParticles, 0.001);

	ParticleMover::updatePositions(&currentParticles, 0.001);

	grid->cleanGrid();

	grid->setParticles(currentParticles);

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