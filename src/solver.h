#ifndef SOLVER_H
#define SOLVER_H

#include "Particle.h"
#include <vector>
#include <map>

#define pVec std::vector<Particle*>
#define fVec std::vector<float>
class Solver 
{

private:
    pVec currentParticles;





    Particle* UpdateDensityPressure(Particle* particle, pVec* neighborList,                
        fVec weights);

    pVec* CalcNeighbors(Particle* particle);

    //do we need this?
    void SetParticles(pVec* particleList);



public: 
    Solver(pVec initialParticles);

    pVec GetParticles();

    void UpdateAll();

};


#endif