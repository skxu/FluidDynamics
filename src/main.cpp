#include "solver.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int Initialize(std::vector<Particle*>* particles);
float PARTICLE_DENSITY = 10.0;
float PARTICLE_PRESSURE = 11.0;
int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		PARTICLE_PRESSURE = atof(argv[1]);
		PARTICLE_DENSITY = atof(argv[2]);
	}
	

	std::vector<Particle*> particles;
	if (Initialize(&particles) == -1)
	{
		//failed initialize
		return -1;
	}

	
	Solver solver = Solver(particles);
	
	
	return 0;

}

int Initialize(std::vector<Particle*>* particles)
{
	std::ifstream inFile("../inputs/init.txt");
	if (!inFile.is_open()) { return -1; }
	
	

	std::string line;
	while(std::getline(inFile,line))
	{
		std::istringstream iss(line);
		float x, y, z;
		if (!(iss >> x >> y >> z))
		{
			std::cout<<"Bad input format"<<std::endl;
			break;
		}
		Particle* p = new Particle();
		p->pos = vec3(x, y, z);
		p->vel = vec3(0.0, 0.0, 0.0);
		p->acc = vec3(0.0, 0.0, 0.0);
		p->density = PARTICLE_DENSITY;
		p->pressure = PARTICLE_PRESSURE;
		particles->push_back(p);
	} 
	return 0;
}