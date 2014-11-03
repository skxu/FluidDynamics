/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>

using namespace std ;
#include "variables.h"

void transformvec(const GLfloat input[4], GLfloat output[4]);
void addLight(GLfloat r, GLfloat b, GLfloat g, GLfloat a, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void renderLights();
void drawSphere(mat4 &mv); // temporary

void display() {
  glClearColor(0.1,0.1,0.1,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  // First include the camera transform
  mat4 mv ;
  mv = glm::lookAt(eye,center,up) ;
  glLoadMatrixf(&mv[0][0]) ;

  /** BEGIN DRAW **/

  // First add lights
  addLight(1000.0, 0.0, 0.0, 1.0, 0.0, 0.1, 0.3, 1.0);
  renderLights();

  drawSphere(mv);

  /** END DRAW **/
  glutSwapBuffers();
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

void addLight(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
  if (numLights < maxNumLights) {
    lightposn[numLights*4] = x;
    lightposn[numLights*4+1] = y;
    lightposn[numLights*4+2] = z;
    lightposn[numLights*4+3] = w;
    lightcolor[numLights*4] = r;
    lightcolor[numLights*4+1] = g;
    lightcolor[numLights*4+2] = b;
    lightcolor[numLights*4+3] = a;
    numLights++;
  } 
}

void renderLights() {
  GLfloat in[4];
  GLfloat out[4];
  glUniform1i(enablelighting, false);
  for (int i = 0; i < numLights; i++) {
    glUniform1i(enablelighting, true);
    in[0] = lightposn[i*4];
    in[1] = lightposn[i*4+1];
    in[2] = lightposn[i*4+2];
    in[3] = lightposn[i*4+3];
    transformvec(in, out);
    lighttransf[i*4] = out[0];
    lighttransf[i*4+1] = out[1];
    lighttransf[i*4+2] = out[2];
    lighttransf[i*4+3] = out[3];
  }
  glUniform4fv(lightpos, numLights, lighttransf);
  glUniform4fv(lightcol, numLights, lightcolor);
  glUniform1i(numused, numLights);
}

void drawSphere(mat4 &mv) {
  GLfloat ambient[4] = {0.2, 0.2, 0.2, 1};
  GLfloat diffuse[4] = {0.2, 0.2, 0.2, 1};
  GLfloat specular[4] = {0.2, 0.2, 0.2, 1};
  GLfloat shininess = 1.0;
  glUniform4fv(ambientcol, 1, ambient);
  glUniform4fv(diffusecol, 1, diffuse);
  glUniform4fv(specularcol, 1, specular);
  glUniform1f(shininesscol, shininess);
  glLoadMatrixf(&(mv[0][0]));
  glutSolidSphere(0.2, 40, 40);
}

