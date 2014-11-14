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

using namespace std;

class Scene {
  public:
    Scene(const char * filename);
    void initializeScene();
    void draw(glm::mat4 &mv, int timeIndex);
    void destroy();
    int numTimeSteps();
  private:
    void addLight(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    void renderLights();
    void readFromFile(const char * filename);
    bool readvals(stringstream &s, const int numvals, float* values, int linecount);
    glm::mat4 translateMtx(float x, float y, float z);

    static const int numLights = 10;
    GLfloat lightposn [4*numLights] ; // Light Positions
    GLfloat lightcolor[4*numLights] ; // Light Colors
    GLfloat lightransf[4*numLights] ; // Lights transformed by modelview
    int numused ;                     // How many lights are used

    map<int, vector<float>*>* timetopartlist;
};

#endif // __INCLUDESCENE