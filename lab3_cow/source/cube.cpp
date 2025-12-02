


#include "cube.h"
#include "cow.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

cow::cow()
{

	setup();
}

void cow::calculateNormal()
{
	const uint32_t ntris = 3156;

	normals.resize(1732);

	for (uint32_t i = 0; i < ntris; ++i) {

		const glm::vec3& v0 = vertices[nvertices[i * 3]];
		const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]];
		const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]];


		glm::vec3 vv1 = v1 - v0;
		glm::vec3 vv2 = v2 - v0;

		glm::vec3 n = glm::cross(vv1, vv2);
		n = glm::normalize(n);

		normals[nvertices[i * 3]] = n;
		normals[nvertices[i * 3+1]] = n;
		normals[nvertices[i * 3+2]] = n;
	}

}


void cow::setup()
{
			
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


	calculateNormal();
	
	

	glCreateVertexArrays(1, &vaoHandle);
	glCreateBuffers(1, &vbo_cow_vertices); // VBO 두 개 생성
	glCreateBuffers(1, &vbo_cow_normals); // VBO 두 개 생성
	glCreateBuffers(1, &ibo_cow_elements); // VBO 두 개 생성


	// 정점 데이터 VBO 설정
	glNamedBufferData(vbo_cow_vertices, sizeof(glm::vec3)*1732, vertices, GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 0, vbo_cow_vertices, 0, sizeof(glm::vec3));


	glNamedBufferData(vbo_cow_normals, sizeof(glm::vec3) * 1732, normals.data(), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 1, vbo_cow_normals, 0, sizeof(glm::vec3));


	// 인덱스 데이터 IBO 설정
	glNamedBufferData(ibo_cow_elements, sizeof(nvertices), nvertices, GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vaoHandle, ibo_cow_elements);


	// 위치 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 0, 0);
	glEnableVertexArrayAttrib(vaoHandle, 0);
	

	glVertexArrayAttribFormat(vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 1, 1);
	glEnableVertexArrayAttrib(vaoHandle, 1);



}

void cow::draw()
{

	
	glBindVertexArray(vaoHandle);
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	float tt = size / sizeof(uint32_t);
	glDrawElements(GL_TRIANGLES, size / sizeof(uint32_t), GL_UNSIGNED_INT, 0);



}

/////////////

