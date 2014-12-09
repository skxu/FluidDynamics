#ifndef __INCLUDESVARIABLES
#define __INCLUDESVARIABLES

#include <glm/glm.hpp>

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif 

EXTERN glm::vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN glm::vec3 up;  // The (regularly updated) vector coordinates of the up 
EXTERN glm::vec3 center; // The center vector where the eye is looking at.

EXTERN GLuint vertexshader, fragmentshader, shaderprogram ; // shaders

// Variables to set uniform params for lighting fragment shader 
EXTERN GLuint lightcol ; 
EXTERN GLuint lightpos ; 
EXTERN GLuint numusedcol ; 
EXTERN GLuint enablelighting ; 
EXTERN GLuint ambientcol ; 
EXTERN GLuint diffusecol ; 
EXTERN GLuint specularcol ; 
EXTERN GLuint shininesscol ;

#endif // __INCLUDESVARIABLES