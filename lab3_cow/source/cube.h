#ifndef __CUBE
#define __CUBE

#include "GL/gl3w.h"
#include <glm/mat4x4.hpp>

#include <vector>


class cow
{
public:
	cow();
	void setup();
	void draw();
	void calculateNormal();

	GLuint vaoHandle;
	GLuint vbo_cow_vertices, ibo_cow_elements;
	GLuint vbo_cow_normals;
	std::vector<glm::vec3> normals;
};

#endif