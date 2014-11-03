#ifndef __INCLUDEENTITY
#define __INCLUDEENTITY

#include <vector>
#include <queue>
#include <stack>
#include <glm/glm.hpp>

class Entity {
  protected:
    enum shape {cube, sphere};
    struct object {
      shape type;
      GLfloat ambient[4] ; 
      GLfloat diffuse[4] ; 
      GLfloat specular[4] ;
      GLfloat shininess ;
      glm::mat4 transform ; 
    };
    std::vector<object> elements;
    std::stack<glm::mat4> s;
    void pushTransform();
    void popTransform();
    void rightmultiply(const glm::mat4 & M);
    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    void rotate(float ax, float ay, float az, float angle_deg);
    void addObject(Entity::shape type, GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4], GLfloat shininess);
    void instantiateObjects(); // overwrite me!
  public:
    Entity();
    void draw(glm::mat4 &mv);
    void update();
};

#endif // __INCLUDEENTITY