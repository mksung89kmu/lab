#ifndef __CUBE
#define __CUBE

#include "GL/gl3w.h"
#include <glm/mat4x4.hpp>

#include <vector>
#include<cstddef>

class HalfEdgeMesh;

class bunny
{
public:
	bunny();
	void setup();
	void draw();

	GLuint vaoHandle;
	GLuint vbo_bunny_vertices, ibo_bunny_elements;
	GLuint vbo_bunny_normals, vbo_bunny_texcoords;

};

#endif