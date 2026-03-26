#ifndef __CUBE
#define __CUBE

#include "GL/gl3w.h"
#include <glm/mat4x4.hpp>


#include <vector>
#include<cstddef>

class checkeredFloor
{
public:
	checkeredFloor();
	void setup(float size, int nSquares);
	void draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection);
	void draw();


	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_colors;
	
};
#endif