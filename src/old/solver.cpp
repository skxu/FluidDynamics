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

  std::cout << "Neighbors" << std::endl;
  for (int i = 0; i < currentParticles.size(); i++) {
    std::cout << currentParticles[i]->neighbors->size() << std::endl;
  }

	//Update densities
	pdSolver->UpdateDensities(&currentParticles, cutoff);

  std::cout << "Density" << std::endl;
  for (int i = 0; i < currentParticles.size(); i++) {
    std::cout << currentParticles[i]->density << std::endl;
  }

  std::cout << "Mass" << std::endl;
  for (int i = 0; i < currentParticles.size(); i++) {
    std::cout << currentParticles[i]->mass << std::endl;
  }

	//Update pressures
	pdSolver->UpdatePressures(&currentParticles);

  std::cout << "Pressure" << std::endl;
  for (int i = 0; i < currentParticles.size(); i++) {
    std::cout << currentParticles[i]->pressure << std::endl;
  }

	//Solve navier-stokes
	currentParticles = * nsSolver->solve(&currentParticles, cutoff);

	ParticleMover::updateVelocities(&currentParticles, 0.001);

	ParticleMover::updatePositions(&currentParticles, 0.001);

	grid->setParticles(currentParticles);

  grid->cleanGrid();

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