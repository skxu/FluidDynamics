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


void display() {
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  // First include the camera transform
  mat4 mv ;
  mv = glm::lookAt(eye,center,up) ;
  glLoadMatrixf(&mv[0][0]) ;
  /** BEGIN DRAW **/


  /** END DRAW **/
  glutSwapBuffers();
}



