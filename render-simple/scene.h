#ifndef __INCLUDESCENE
#define __INCLUDESCENE

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

class Scene {
  public:
    Scene(const char * filename);
    void destroy();
    void draw(glm::mat4 &mv, int timeidx);
    void initializeScene();
    int numTimeSteps();
  private:
    void addLight(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    void readFromFile(const char * filename);
    bool readvals(std::stringstream &s, const int numvals, float* values, int linecount);
    void renderLights();
    void renderParticles(glm::mat4 &mv, int timeidx);
    void renderTable(glm::mat4 &mv);
    glm::mat4 scaleMtx(float x, float y, float z);
    glm::mat4 translateMtx(float x, float y, float z);

    static const float PARTICLE_RADIUS = 0.015;
    static const int numLights = 10;
    GLfloat lightposn [4*numLights] ; // Light Positions
    GLfloat lightcolor[4*numLights] ; // Light Colors
    GLfloat lightransf[4*numLights] ; // Lights transformed by modelview
    int numused ;                     // How many lights are used

    std::map<int, std::vector<float>*>* timetopartlist;
};

#endif // __INCLUDESCENE
