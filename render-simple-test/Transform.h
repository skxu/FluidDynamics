#ifndef __INCLUDETRANSFORM
#define __INCLUDETRANSFORM

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
  public:
	  Transform();
	  virtual ~Transform();
	  static void left(float degrees, glm::vec3& eye, glm::vec3& up);
	  static void up(float degrees, glm::vec3& eye, glm::vec3& up);
	  static glm::mat4 lookAt(const glm::vec3& eye, const glm::vec3 &center, const glm::vec3& up);
	  static glm::mat4 perspective(float fovy, float aspect, float zNear, float zFar);
    static glm::mat3 rotate(const float degrees, const glm::vec3& axis) ;
    static glm::mat4 scale(const float &sx, const float &sy, const float &sz) ; 
    static glm::mat4 translate(const float &tx, const float &ty, const float &tz);
    static glm::vec3 upvector(const glm::vec3 &up, const glm::vec3 &zvec) ; 
};

#endif // __INCLUDETRANSFORM