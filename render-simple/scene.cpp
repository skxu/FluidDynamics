#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include "scene.h"
#include "Transform.h"
#include "variables.h"

Scene::Scene(const char * filename) {
  numused = 0;
  timetopartlist = new std::map<int, std::vector<float>*>;
  initializeScene();
  readFromFile(filename);
}

Scene::~Scene() {
  for (int i = 0; i < timetopartlist->size(); i++) {
    delete (*timetopartlist)[i];
  }
  delete timetopartlist;
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

bool Scene::readvals(std::stringstream &s, const int numvals, float * values, int lineCount) {
  for (int i = 0 ; i < numvals ; i++) {
    s >> values[i] ;
    if (s.fail()) {
      std::cout << "Line " << lineCount << ": Failed reading value " << i << " will skip." << std::endl;
      return false ;
    }
  }
  return true ;
}

void Scene::readFromFile(const char * filename) {
  int timestep = 0;
  int linecount = 0;
  std::vector<float>* particles = new std::vector<float>();
  std::string str, cmd;
  std::ifstream in ;
  in.open(filename);
  if (in.is_open()) {
    getline (in, str);
    while (in) {
      linecount++;
      // Rule out comments and blank lines first
      if ((str.find_first_not_of(" \t\r\n") != std::string::npos) && (str[0] != '#')) {
        std:: stringstream s(str);
        s >> cmd;
        float values[3];
        if (cmd == "p") {
          readvals(s, 3, values, linecount);
          particles->push_back(values[0]);
          particles->push_back(values[1]);
          particles->push_back(values[2]);
        }
        else if (cmd == "t") {
          if (linecount != 1 || timestep != 0) {
            (*timetopartlist)[timestep] = particles;
            timestep++;
            particles = new std::vector<float>();
          }
        }
      }
      getline (in, str);
    }
  } else {
    std::cerr << "Unable to Open Data File " << filename << std::endl; 
  }
}

glm::mat4 Scene::translateMtx(float x, float y, float z) {
  return glm::transpose(Transform::translate(x, y, z));
}

glm::mat4 Scene::scaleMtx(float x, float y, float z) {
  return glm::transpose(Transform::scale(x, y, z));
}

void Scene::draw(glm::mat4& mv, int timeidx) {
  renderLights();
  renderParticles(mv, timeidx);
  renderTable(mv);
}

void Scene::renderParticles(glm::mat4 &mv, int timeidx) {
  GLfloat ambient [4] = {0.1, 0.1, 0.1, 1};
  GLfloat shininess = 4;
  std::vector<float>* partlist = (*(timetopartlist))[timeidx];
  for (std::vector<float>::iterator obj = partlist->begin(); obj != partlist->end(); obj+=3) {
    GLfloat diffuse [4] = {0.1, 0.6 * *(obj+2), 0.4, 1};
    GLfloat specular[4] = {0.1, 0.7 * *(obj+2), 0.5, 1};
    glUniform4fv(ambientcol, 1, ambient);
    glUniform4fv(diffusecol, 1, diffuse);
    glUniform4fv(specularcol, 1, specular);
    glUniform1i(shininesscol, shininess);
    glm::mat4 translate = translateMtx(*obj, *(obj+1), *(obj+2));
    glLoadMatrixf(&(mv * translate)[0][0]);
    glutSolidSphere(PARTICLE_RADIUS, 3, 3);
  }
}

void Scene::renderTable(glm::mat4 &mv) {
  GLfloat ambient [4] = {0.2, 0.2, 0.2, 1};
  GLfloat diffuse [4] = {0.2, 0.2, 0.2, 1};
  GLfloat specular[4] = {0.3, 0.3, 0.3, 1};
  GLfloat shininess = 4;
  glUniform4fv(ambientcol, 1, ambient);
  glUniform4fv(diffusecol, 1, diffuse);
  glUniform4fv(specularcol, 1, specular);
  glUniform1i(shininesscol, shininess);
  float scalevals[3] = {3, 1, 0.05};
  glm::mat4 translate = translateMtx(0, 0, -scalevals[2]-PARTICLE_RADIUS); // small offset so particles near table don't go into the table
  glm::mat4 scale = scaleMtx(scalevals[0], scalevals[1], scalevals[2]);
  glLoadMatrixf(&(mv * translate * scale)[0][0]);
  glutSolidCube(2); // (Cube with (1,1,1), (1,1,-1), (-1,1,1), (1,-1,1), etc...)
}

int Scene::numTimeSteps() {
  return timetopartlist->size();
}
