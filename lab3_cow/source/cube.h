#ifndef __CUBE
#define __CUBE

#include "GL/gl3w.h"
#include <glm/mat4x4.hpp>


#include <vector>
#include<cstddef>


class HalfEdgeMesh;


class cow
{
public:
	cow();
	void setup();
	void draw();
	void calculateNormal();
	void laplacianSmoothing(HalfEdgeMesh& mesh);
	void findingBoundaryEdge(HalfEdgeMesh& mesh);


	GLuint vaoHandle;
	GLuint vbo_cow_vertices, ibo_cow_elements;
	GLuint vbo_cow_normals;
	std::vector<glm::vec3> normals;
};

#endif