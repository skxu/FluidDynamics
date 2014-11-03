#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "Transform.h"
#include "display.h"

using namespace std ;
#include "variables.h"

void transformvec (const GLfloat input[4], GLfloat output[4]) {
    GLfloat modelview[16] ; // in column major order
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview) ;
    
    for (int i = 0 ; i < 4 ; i++) {
        output[i] = 0 ;
        for (int j = 0 ; j < 4 ; j++)
            output[i] += modelview[4*j+i] * input[j] ;
    }
}



void display() {
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glMatrixMode(GL_MODELVIEW);
	mat4 mv ;
    
    mv = glm::lookAt(eye,center,up) ;
    glLoadMatrixf(&mv[0][0]) ;

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
    else glUniform1i(enablelighting,false) ;
    
    GLfloat ambient[4] = {0.2, 0.2, 0.2, 0.2};
    GLfloat diffuse[4] = {0.2, 0.2, 0.2, 0.2};
    GLfloat specular[4] = {0.2, 0.2, 0.2, 0.2};
    GLfloat shininess = 15;

    glUniform4fv(ambientcol, 1, ambient);
    glUniform4fv(diffusecol, 1, diffuse);
    glUniform4fv(specularcol, 1, specular);
    glUniform1f(shininesscol, shininess);
    glutSolidSphere(0.2, 20, 20);
    
    glutSwapBuffers();
}