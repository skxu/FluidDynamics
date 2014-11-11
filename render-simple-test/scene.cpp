#include <iostream>
#include "scene.h"
#include "table.h"
#include "container.h"
#include "variables.h"

Scene::Scene() {
  numused = 0;
  static_ents = std::vector<Entity>();
  dynamic_ents = std::vector<Entity>();
  initializeScene();
}

void Scene::initializeScene() {
  addLight(0.0, 0.0, 1.0, 0.0, 0.9, 0.9, 0.9, 1.0);
  Table* t = new Table(3, 3, 1);
  static_ents.push_back(*t);
  Container* c = new Container(2, 1, 0.8);
  static_ents.push_back(*c);
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
    
    for (int i = 0 ; i < 4 ; i++) {
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

void Scene::update() {
  for(std::vector<Entity>::iterator obj = dynamic_ents.begin(); obj != dynamic_ents.end(); obj++) {
    obj->update();
  }
}

void Scene::draw(glm::mat4 &mv) {
  renderLights();
  for(std::vector<Entity>::iterator obj = static_ents.begin(); obj != static_ents.end(); obj++) {
    obj->draw(mv);
  }
  for(std::vector<Entity>::iterator obj = dynamic_ents.begin(); obj != dynamic_ents.end(); obj++) {
    obj->draw(mv);
  }  
}