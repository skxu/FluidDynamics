#include "solver.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#ifndef _WIN64
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>

#define pVec std::vector<Particle*>

#define DEFAULT_DENSITY 10.0
#define DEFAULT_PRESSURE 11.0
#define DEFAULT_TIMESTEPS 300
#define DEFAULT_CUTOFF 1

float particle_density = DEFAULT_DENSITY;
float particle_pressure = DEFAULT_PRESSURE;
float cutoff = DEFAULT_CUTOFF;
int timesteps = DEFAULT_TIMESTEPS;
char* input_file = NULL;
char* output_file = NULL;

int Initialize(pVec* particles);

int main(int argc, char* argv[])
{
	int c;
#ifndef _WIN64
	while ((c = getopt(argc, argv, "d:p:t:o:i:h:")) != -1) 
	{
		switch (c)
		{
			case 'd':
				particle_density = atof(optarg);
				break;
			case 'p':
				particle_pressure = atof(optarg);
				break;
			case 't':
				timesteps = atoi(optarg);
				break;
			case 'i':
				input_file = optarg;
				break;
			case 'o':
				output_file = optarg;
				break;
			case 'h':
				cutoff = atof(optarg);
				break;

			default:
				break;

		}
	}
#endif

	if (input_file == NULL || output_file == NULL)
	{
		std::cout << "[Error]: Must specify input & output file" <<std::endl;
	}

	pVec particles;
	if (Initialize(&particles) == -1)
	{
		//failed initialize
		return -1;
	}
	
	Solver solver = Solver(particles, cutoff);
	std::ofstream output;
	output.open(output_file);
	for (int timestep=0; timestep<timesteps; timestep++) 
	{
		pVec currentParticles = solver.GetParticles();
		for (int idx=0; idx<currentParticles.size(); idx++)
		{
			Particle* p = currentParticles[idx];
			output << "particle ";
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
	std::ifstream inFile(input_file);
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
		p->density = particle_density;
		p->pressure = particle_pressure;
		particles->push_back(p);
	} 
	return 0;
}