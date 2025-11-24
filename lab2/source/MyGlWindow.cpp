
#define GLM_ENABLE_EXPERIMENTAL 
#include "MyGlWindow.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

MyGlWindow::MyGlWindow(int w, int h)

//==========================================================================
{
	m_width = w;
	m_height = h;

	setupBuffer();
	
}

void MyGlWindow::setupBuffer()
{
	/*
	const float vertexPositions[] = {
		-0.1f, -0.1f, 0.0f, 1.0f,
	 	 0.1f, -0.1f, 0.0f, 1.0f,
		-0.1f, 0.1f, 0.0f, 1.0f,
				
		0.1f, -0.1f, 0.0f, 1.0f,
		0.1f,  0.1f, 0.0f, 1.0f,
		- 0.1f, 0.1f, 0.0f, 1.0f

				
	};

	const float vertexColors[] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,

		0, 1, 0,
		1, 1, 0,
		0, 0, 1
	};
	*/

	/*  rectangle : two triangles
	const float vertexPositions[] = {
		-0.2f, 0.1, 0.0f, 1.0f,
		-0.2f, -0.1f, 0.0f, 1.0f,
		0.2f, -0.1f, 0.0f, 1.0f,

		0.2f, -0.1f, 0.0f, 1.0f,
		-0.2f, 0.1, 0.0f, 1.0f,
		0.2f, 0.1, 0.0f, 1.0f,

	
	};

	const float vertexColors[] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
		0, 0, 1,
		1, 0, 0,
		0, 1, 0
	};
	*/

	const float vertexPositions[] = {
		-0.2f, -0.1f, 0.0f, 1.0f,
		0.2f, -0.1f, 0.0f, 1.0f,			
		-0.2f, 0.1, 0.0f, 1.0f,
		0.2f, 0.1, 0.0f, 1.0f
	};

	const float vertexColors[] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
		0, 1, 0
	};


	shaderProgram = std::make_unique<ShaderProgram>();
	
	//load shaders
	shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");

	

	//without DSA
	
	
	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);
	
	//create vbo for vertices
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), &vertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, 
		4,                 
		GL_FLOAT,          
		GL_FALSE,          
		0,             //  stride : distance from the previous to next data
		0              //  offset  
		);
    glEnableVertexAttribArray(0);


	//create vbo for colors
	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), &vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // attribute
		3,                 // number of elements per vertex, here (R,G,B)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
		);
	glEnableVertexAttribArray(1);

	//unbound the vao
	glBindVertexArray(0);
	
	

	//with DSA
	/*
	
	glCreateVertexArrays(1, &vaoHandle);
	glCreateBuffers(1, &vbo_cube_vertices); // VBO 두 개 생성
	glCreateBuffers(1, &vbo_cube_colors); // VBO 두 개 생성

	// 정점 데이터 VBO 설정
	glNamedBufferData(vbo_cube_vertices, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

	//0번 index에 sizeof(GLfloat)*4 간격으로 데이터를 가져오도록 설정
	glVertexArrayVertexBuffer(vaoHandle, 0, vbo_cube_vertices, 0, sizeof(GLfloat) * 4);  //0 : binding index, 0 : offset, sizeof(GLfloat)*4 : stride (VBO에도 번호를 매긴다)

	// 색상 데이터 VBO 설정
	glNamedBufferData(vbo_cube_colors, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 1, vbo_cube_colors, 0, sizeof(GLfloat) * 3);   //1 : binding index, 0 : offset, sizeof(GLfloat)*3 : stride

	// 위치 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 
		0, // attribute index
		4, 
		GL_FLOAT, 
		GL_FALSE, 
		0);  //상대적인 offset
	glVertexArrayAttribBinding(vaoHandle, 0, 0);  //attribute index, binding index을 연결  (attribute 0은 binding 0번 VBO에서 데이터를 가져온다)
	glEnableVertexArrayAttrib(vaoHandle, 0);

	// 색상 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 
		1, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		0);
	glVertexArrayAttribBinding(vaoHandle, 1, 1); //attribute index, binding index을 연결 
	glEnableVertexArrayAttrib(vaoHandle, 1);

	*/


	/*
	- Vertex Buffer Binding Index라고도 하며, DSA 스타일에서 glVertexArrayVertexBuffer 또는 glBindVertexBuffer 등에 사용됩니다.
	- 하나의 VAO에서 여러 버퍼를 바인딩할 수 있는데, 이때 각 버퍼를 구분하기 위한 슬롯 번호입니다.
	- 예: glVertexArrayVertexBuffer(vao, 2, vbo, 0, stride); → binding index는 2입니다.
		
	 예전방식에는 번호로 구분하지 않은 이유는 대부분 단일 버퍼 였고, 순차적으로 설정했기 때문에 따로 번호를 지정할 필요가 없었습니다.
	*/

	//interleaved way : DSA Higher than 4.5
	GLfloat vertices[] = {
		// 위치          // 색상
		-0.2f, 0, 0.0f, 1.0f,     1.0f, 0.0f, 0.0f, // Red
		0,  0.4f, 0.0f, 1.0f,     0.0f, 1.0f, 0.0f, // Green
		0.2f, 0, 0.0f, 1.0f,      0.0f, 0.0f, 1.0f  // Blue
	};

	/*
	//no dsa : interleave
	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//create vbo for vertices
	glGenBuffers(1, &vbo_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(
		0,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 7,
		(void*)0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1, // attribute
		3,                 // number of elements per vertex, here (R,G,B)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		sizeof(float) * 7,                 // no extra data between each position
		(void*)(sizeof(float) * 4)                  // offset of first element
	);
	glEnableVertexAttribArray(1);

	//unbound the vao
	glBindVertexArray(0);
	
	*/
	
	//interleave : dsa

	/*
	GLuint VBO;
	glCreateVertexArrays(1, &vaoHandle);
	glCreateBuffers(1, &VBO);

	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vaoHandle, 0, VBO, 0, sizeof(float) * 7); // 7는 위치와 색상 포함

	// 위치 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 0, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoHandle, 0, 0);
	glEnableVertexArrayAttrib(vaoHandle, 0);

	// 색상 속성 설정
	glVertexArrayAttribFormat(vaoHandle, 1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4);
	glVertexArrayAttribBinding(vaoHandle, 1, 0);
	glEnableVertexArrayAttrib(vaoHandle, 1);

	*/

}

void MyGlWindow::draw(void)
{
	
	glClearColor(0.2f, 0.2f, .2f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, m_width, m_height);

	
	shaderProgram->use();
//	glUniformMatrix4fv(shaderProgram->uniform("Model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(vaoHandle);
//		glDrawArrays(GL_TRIANGLES, 0, 4);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	shaderProgram->disable();


}
