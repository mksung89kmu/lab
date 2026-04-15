

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "cube.h"
#include "bunny.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "HalfEdge.h"

bunny::bunny()
{

	setup();

	setupTexture();

}

void bunny::setupTexture()
{

	//	int width, height, channel;//
	//	unsigned char* image = stbi_load("baseColor.png", &width, &height, &channel,
	//		0);


		//NON-DSA
		/*
		glGenTextures(1, &name);  //tex_2d
		glBindTexture(GL_TEXTURE_2D, name);  //Activate and set it 2D texture

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);


		int width, height, channel;
		unsigned char* image = stbi_load("baseColor.png", &width, &height, &channel, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0);
		*/


		//DSA

		glCreateTextures(GL_TEXTURE_2D, 1, &name);

		glTextureParameteri(name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(name, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTextureParameteri(name, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		int width, height, channel;
		std::string imagePath = std::string(DATA_DIR) + "/textures/baseColor.png";

		unsigned char* image = stbi_load(imagePath.c_str(), &width, &height, &channel, 0);

		glTextureStorage2D(name, 1, GL_RGB8, width, height);
		glTextureSubImage2D(name, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

		glGenerateTextureMipmap(name);

		stbi_image_free(image);


}




void bunny::setup()
{
		
	//struct ModelVertex const modelVertices[3161]
	
		
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

	//https://github.com/nothings/stb/tree/master

	//https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions
	/*

	glCreateVertexArrays(1, &vaoHandle);
	glCreateBuffers(1, &vbo_bunny_vertices); // VBO 두 개 생성
	glCreateBuffers(1, &vbo_bunny_normals); // VBO 두 개 생성
	glCreateBuffers(1, &vbo_bunny_texcoords); // VBO 두 개 생성
	glCreateBuffers(1, &ibo_bunny_elements); // VBO 두 개 생성

	// 정점 데이터 VBO 설정
	glNamedBufferData(vbo_bunny_vertices, sizeof(glm::vec3)* 3161, vertices.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 0, vbo_bunny_vertices, 0, sizeof(glm::vec3));


	glNamedBufferData(vbo_bunny_normals, sizeof(glm::vec3) * 3161, normals.data(), GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 1, vbo_bunny_normals, 0, sizeof(glm::vec3));

	glNamedBufferData(vbo_bunny_texcoords, sizeof(glm::vec2) * 3161, texcoords.data(), GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 2, vbo_bunny_texcoords, 0, sizeof(glm::vec2));


	// 인덱스 데이터 IBO 설정
	glNamedBufferData(ibo_bunny_elements, sizeof(modelIndices), modelIndices, GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vaoHandle, ibo_bunny_elements);


	// 위치 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 0, 0);
	glEnableVertexArrayAttrib(vaoHandle, 0);
	
	// 법선 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 1, 0);
	glEnableVertexArrayAttrib(vaoHandle, 1);

	// 텍스처 좌표 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 2, 0);
	glEnableVertexArrayAttrib(vaoHandle, 2);
	*/


	glCreateVertexArrays(1, &vaoHandle);
	GLuint handle[2];

	glCreateBuffers(2, handle); // VBO 두 개 생성

	// 정점 데이터 VBO 설정
	glNamedBufferData(handle[0], 3161 * sizeof(ModelVertex), modelVertices, GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 0, handle[0], 0, sizeof(float) * 8);

	// 위치 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 0, 0);
	glEnableVertexArrayAttrib(vaoHandle, 0);
	// 노말 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);
	glVertexArrayAttribBinding(vaoHandle, 1, 0);
	glEnableVertexArrayAttrib(vaoHandle, 1);
	// 텍스처 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6);
	glVertexArrayAttribBinding(vaoHandle, 2, 0);
	glEnableVertexArrayAttrib(vaoHandle, 2);

	//인덱스
	glNamedBufferData(handle[1], 17088 * sizeof(unsigned int), modelIndices, GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vaoHandle, handle[1]);
}

void bunny::draw()
{

	glBindVertexArray(vaoHandle);

	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	
	glBindTextureUnit(0, name);

	glDrawElements(GL_TRIANGLES, size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

/////////////

