#include "solver.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define pVec std::vector<Particle*>

#define DEFAULT_DENSITY 10.0
#define DEFAULT_PRESSURE 11.0
#define DEFAULT_TIMESTEPS 300

float partilce_density = DEFAULT_DENSITY;
float particle_pressure = DEFAULT_PRESSURE;
int timesteps = DEFAULT_TIMESTEPS;

int Initialize(pVec* particles);

int main(int argc, char* argv[])
{


	if (argc == 3)
	{
		particle_pressure = atof(argv[1]);
		partilce_density = atof(argv[2]);
		timesteps = atoi(argv[3]);
	}

	pVec particles;
	if (Initialize(&particles) == -1)
	{
		//failed initialize
		return -1;
	}
	
	Solver solver = Solver(particles);
	std::ofstream output;
	output.open("../outputs/output.txt");
	for (int timestep=0; timestep<timesteps; timestep++) 
	{
		pVec currentParticles = solver.GetParticles();
		for (int idx=0; idx<currentParticles.size(); idx++)
		{
			Particle* p = currentParticles[idx];
			output << "Particle ";
			output << p->pos.x;
			output << " ";
			output << p->pos.y;
			output << " ";
			output << p->pos.z;
			output <<"\n";
		}
		output << "time\n";
	}

	return 0;

}

int Initialize(pVec* particles)
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
		p->density = partilce_density;
		p->pressure = particle_pressure;
		particles->push_back(p);
	} 
	return 0;
}