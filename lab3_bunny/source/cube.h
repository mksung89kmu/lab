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
	
    void laplacianSmoothing(HalfEdgeMesh& mesh);
    void findingBoundaryEdge(HalfEdgeMesh& mesh);

	GLuint vaoHandle;
	GLuint vbo_cow_vertices, ibo_cow_elements;
	GLuint vbo_cow_normals;

};

#endif