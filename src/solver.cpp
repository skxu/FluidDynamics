#include "solver.h"


Solver::Solver(
	pVec particleList,
	float hValue)
{
	cutoff = hValue;
	currentParticles = particleList;
    grid = new Grid(420,420,420,4); //What are our parameters?!
    grid->setParticles(currentParticles);

    //initialize solvers
    nsSolver = new NSSolver();
    pdSolver = new PDSolver();

}

void Solver::UpdateAll()
{
	

	pVec* updatedParticles;	

	//Calculate neighbors
	grid->setNeighbors(currentParticles);

	//Update densities
	updatedParticles = pdSolver->UpdateDensities(currentParticles);

	//Update pressures
	updatedParticles = pdSolver->UpdatePressures(updatedParticles);

	//Solve navier-stokes
	updatedParticles = nsSolver->solve(updatedParticles, cutoff);

	currentParticles = *updatedParticles;

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