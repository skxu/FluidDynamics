#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include "scene.h"
#include "Transform.h"
#include "variables.h"

using namespace std;

Scene::Scene(const char * filename) {
  numused = 0;
  timetopartlist = new map<int, vector<float>*>;
  initializeScene();
  readFromFile(filename);
}

void Scene::initializeScene() {
  addLight(0.0, 0.0, 1.0, 0.0, 0.9, 0.9, 0.9, 1.0);
}

void Scene::addLight(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
  if (numused < numLights) {
    lightposn[numused*4 + 0] = x;
    lightposn[numused*4 + 1] = y;
    lightposn[numused*4 + 2] = z;
    lightposn[numused*4 + 3] = w;
    lightcolor[numused*4 + 0] = r;
    lightcolor[numused*4 + 1] = g;
    lightcolor[numused*4 + 2] = b;
    lightcolor[numused*4 + 3] = a;
    numused++;
  }
}

void transformvec (const GLfloat input[4], GLfloat output[4]) {
    GLfloat modelview[16] ; // in column major order
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview) ;
    
    for (int i = 0 ; i < 4; i++) {
        output[i] = 0 ;
        for (int j = 0 ; j < 4 ; j++)
            output[i] += modelview[4*j+i] * input[j] ;
    }
}

void Scene::renderLights() {
  if (numused) {
    glUniform1i(enablelighting,true) ;
    glUniform1i(numusedcol, numused);
    
    for(int j = 0; j < numused; j++){
        GLfloat inputPosn[4];
        GLfloat outputPosn[4];
        
        inputPosn[0] = lightposn[4*j + 0];
        inputPosn[1] = lightposn[4*j + 1];
        inputPosn[2] = lightposn[4*j + 2];
        inputPosn[3] = lightposn[4*j + 3];
        
        transformvec(inputPosn, outputPosn);
     
        lightransf[4*j + 0] = outputPosn[0];
        lightransf[4*j + 1] = outputPosn[1];
        lightransf[4*j + 2] = outputPosn[2];
        lightransf[4*j + 3] = outputPosn[3];
    }
    glUniform4fv(lightpos, numused, lightransf);
    glUniform4fv(lightcol, numused, lightcolor);
  }
  else glUniform1i(enablelighting, false);
}

bool Scene::readvals(stringstream &s, const int numvals, float * values, int lineCount) {
  for (int i = 0 ; i < numvals ; i++) {
    s >> values[i] ;
    if (s.fail()) {
      cout << "Line " << lineCount << ": Failed reading value " << i << " will skip\n" ;
      return false ;
    }
  }
  return true ;
}

void Scene::readFromFile(const char * filename) {
  int timestep = 0;
  int linecount = 0;
  vector<float>* particles = new vector<float>();
  string str, cmd;
  ifstream in ;
  in.open(filename);
  if (in.is_open()) {
    getline (in, str);
    while (in) {
      linecount++;
      // Rule out comments and blank lines first
      if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
        stringstream s(str);
        s >> cmd;
        float values[3];
        if (cmd == "particle") {
          readvals(s, 3, values, linecount);
          particles->push_back(values[0]);
          particles->push_back(values[1]);
          particles->push_back(values[2]);
        }
        else if (cmd == "time") {
          if (linecount != 1 || timestep != 0) {
            (*timetopartlist)[timestep] = particles;
            timestep++;
            particles = new vector<float>();
          }
        }
      }
      getline (in, str);
    }
  } else {
    cerr << "Unable to Open Data File " << filename << "\n" ; 
  }
}

glm::mat4 Scene::translateMtx(float x, float y, float z) {
  return glm::transpose(Transform::translate(x, y, z));
}

void Scene::draw(glm::mat4& mv, int timeIndex) {
  renderLights();
  GLfloat ambient [4] = {0.1, 0.1, 0.1, 1};
  GLfloat diffuse [4] = {0.1, 0.1, 0.4, 1};
  GLfloat specular[4] = {0.1, 0.1, 0.4, 1};
  GLfloat shininess = 4;
  vector<float>* partlist = (*(timetopartlist))[timeIndex];
  for (vector<float>::iterator obj = partlist->begin(); obj != partlist->end(); obj+=3) {
    glUniform4fv(ambientcol, 1, ambient);
    glUniform4fv(diffusecol, 1, diffuse);
    glUniform4fv(specularcol, 1, specular);
    glUniform1i(shininesscol, shininess);
    glm::mat4 translate = translateMtx(*obj, *(obj+1), *(obj+2));
    glLoadMatrixf(&(mv * translate)[0][0]);
    glutSolidSphere(0.01, 20, 20);
  }
}

void Scene::destroy() {
  for (map<int, vector<float>*>::iterator iter = timetopartlist->begin(); iter != timetopartlist->end(); iter++) {
    delete iter->second;
  }
  delete timetopartlist;
}

int Scene::numTimeSteps() {
  return timetopartlist->size();
}