#ifndef GRID_H
#define GRID_H

#include "Constants.h"
#include "Particle.h"
#include "vec3.h"
#include <vector>
#include <math.h>

class Grid{
public:
    Grid(int x, int y, int z, int h);
    void cleanGrid();
    pVec getParticles(int index);
    void setParticle(Particle* p);
    void setParticles(pVec particles);
    //void updateParticle(Particle* p);
    void setNeighbors(Particle* p);
    void setNeighbors(pVec pVector);
    pVec* getNeighbors(Particle* p);
    //TODO: figure out how to update neighbors after they move
    float getDistance(Particle* p1, Particle* p2);
    bool isValidPos(vec3 v);
    vec3 calcGridPos(Particle* p);
    int calcIndex(vec3 v);
    int getCutoff();
private:
    std::vector< pVec > grid;
    int cutoff;
    int xDim;
    int yDim;
    int zDim;
};

#endif