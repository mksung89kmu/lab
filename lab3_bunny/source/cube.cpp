


#include "cube.h"
#include "bunny.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

bunny::bunny()
{

	setup();
}


void bunny::setup()
{
		
	//struct ModelVertex const modelVertices[3161]
	std::vector<glm::vec3> vertices(3161);
	std::vector<glm::vec3> normals(3161);
	std::vector<glm::vec2> texcoords(3161);

	std::vector<uint32_t> nvertices;
	for (int i = 0; i < 3161; i++) {
		vertices[i] = modelVertices[i].position;
		normals[i] = modelVertices[i].normal;

	}

	
	std::vector<std::vector<int>> triangles;
	for (int i = 0; i < 17088; i+=3) { //is it correct?
	
		std::vector<int> tri;
		tri.push_back(modelIndices[i]);
		tri.push_back(modelIndices[i+1]);
		tri.push_back(modelIndices[i+2]);

		triangles.push_back(tri);
	}

	HalfEdgeMesh mesh(triangles);

	/*
	for (auto he : mesh.halfEdges) {
		std::cout << "HalfEdge from v" << he->vertex->id;
		if (he->twin)
			std::cout << " twin->v" << he->twin->vertex->id;
		else
			std::cout << " twin->None";
		std::cout << std::endl;
	}
	*/

	//const unsigned int modelIndices[17088] 


	/*
	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

		

	//create vbo for vertices
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 3 * 8, &cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // attribute
		3,                 // number of elements per vertex, here (x,y,z,1)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
		);
	glEnableVertexAttribArray(0);


	//create vbo for colors
	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 3 * 8, &cube_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // attribute
		3,                 // number of elements per vertex, here (R,G,B)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
		);
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	

	glBindVertexArray(0);

	*/


	

	glCreateVertexArrays(1, &vaoHandle);
	glCreateBuffers(1, &vbo_cow_vertices); // VBO 두 개 생성
	glCreateBuffers(1, &vbo_cow_normals); // VBO 두 개 생성
	glCreateBuffers(1, &ibo_cow_elements); // VBO 두 개 생성


	// 정점 데이터 VBO 설정
	glNamedBufferData(vbo_cow_vertices, sizeof(glm::vec3)* 3161, vertices.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 0, vbo_cow_vertices, 0, sizeof(glm::vec3));


	glNamedBufferData(vbo_cow_normals, sizeof(glm::vec3) * 3161, normals.data(), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 1, vbo_cow_normals, 0, sizeof(glm::vec3));


	// 인덱스 데이터 IBO 설정
	glNamedBufferData(ibo_cow_elements, sizeof(modelIndices), modelIndices, GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vaoHandle, ibo_cow_elements);


	// 위치 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 0, 0);
	glEnableVertexArrayAttrib(vaoHandle, 0);
	

	glVertexArrayAttribFormat(vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 1, 1);
	glEnableVertexArrayAttrib(vaoHandle, 1);



}

void bunny::draw()
{

	glBindVertexArray(vaoHandle);
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	size_t tt = size / sizeof(unsigned int);
	glDrawElements(GL_TRIANGLES, size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

}

/////////////

