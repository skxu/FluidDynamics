/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

typedef glm::mat3 mat3 ;
typedef glm::mat4 mat4 ; 
typedef glm::vec3 vec3 ; 
typedef glm::vec4 vec4 ; 
const float pi = 3.14159265 ; // For portability across platforms

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif 

EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up 
EXTERN vec3 center ; 
EXTERN int w, h ; 
EXTERN float fovy ; 

EXTERN GLuint vertexshader, fragmentshader, shaderprogram ; // shaders
enum shape {cube, sphere} ; 

EXTERN GLfloat ambient[4] ; 
EXTERN GLfloat diffuse[4] ; 
EXTERN GLfloat specular[4] ; 
EXTERN GLfloat emission[4] ; 
EXTERN GLfloat shininess ; 

struct object {
  shape type ; 
  GLfloat size ;
  GLfloat ambient[4] ; 
  GLfloat diffuse[4] ; 
  GLfloat specular[4] ;
  GLfloat emission[4] ; 
  GLfloat shininess ;
  mat4 transform ; 
};

struct light {
  GLfloat lightcolor[4] ;
  GLfloat lightposn[4] ;
};

EXTERN std::vector<object> objects;
EXTERN std::vector<light> lights;

// Variables to set uniform params for lighting fragment shader 
EXTERN GLuint lightcol ; 
EXTERN GLuint lightpos ; 
EXTERN GLuint numusedcol ; 
EXTERN GLuint enablelighting ; 
EXTERN GLuint ambientcol ; 
EXTERN GLuint diffusecol ; 
EXTERN GLuint specularcol ; 
EXTERN GLuint emissioncol ; 
EXTERN GLuint shininesscol ;
