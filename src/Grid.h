#ifndef GRID_H
#define GRID_H

#include "Particle.h"
#include <vector>
class Grid{
public:
    Grid(int x, int y, int z, int h);
    std::vector<Particle*> getParticles(int index);
    void setParticle(Particle* p);
    void updateParticle(Particle* p);
    void setNeighbors(Particle* p);
    //TODO: figure out how to update neighbors after they move
    float getDistance(Particle* p1, Particle* p2);
    bool isValidPos(vec3 v);
    vec3 calcGridPos(Particle* p);
    int calcIndex(vec3 v);
    int getCutoff();
private:
    std::vector< std::vector<Particle*> > grid;
    int cutoff;
    int xDim;
    int yDim;
    int zDim;
};

#endif