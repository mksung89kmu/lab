
#define GLM_ENABLE_EXPERIMENTAL 
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


glm::mat4 lookAt(glm::vec3 pos, glm::vec3 look, glm::vec3 up)
{

	glm::vec3 zaxis = glm::normalize(pos - look);
	glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

	glm::mat4 R;

	R[0] = glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0.0f);
	R[1] = glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0.0f);
	R[2] = glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0.0f);
	R[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 T;

	T[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	T[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	T[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	T[3] = glm::vec4(-pos.x, -pos.y, -pos.z, 1.0f);


	return R * T;
}


glm::mat4 perspective(float fov, float aspect, float n, float f)
{

	glm::mat4 P(0.0f);

	const float tanHalfFOV = tan(glm::radians(fov) / 2.0f);

	float A = (-f - n) / (f - n);
	float B = (2 * (n * f)) / (n - f);

	P[0] = glm::vec4(1.0f / (aspect * tanHalfFOV), 0, 0, 0);
	P[1] = glm::vec4(0, 1.0 / tanHalfFOV, 0, 0);
	P[2] = glm::vec4(0, 0, A, -1.0f);
	P[3] = glm::vec4(0, 0, B, 0.0f);

	return P;
}



int main()
{

	glm::vec3 vecA(1.0f, 2.0f, 3.0f);
	glm::vec3 vecB(1.0f, 2.0f, 3.0f);

	vecA += vecB;

//	glm::vec4 vecB(4.0f, 5.0f, 6.0f, 7.0f);
	glm::vec4 point(4, 5, 3, 1);

	////////아래 실습은 view matrix와 model matrix 실습에 대한 문제임


	/*
	//S-R-T
	glm::mat4 model = glm::scale(glm::mat4(1.0), glm::vec3(1.3f, 1.3f, 1.3f));
	
	model = model * glm::translate(glm::mat4(1.0f), glm::vec3(0, 5.0f, 0));


	model = glm::rotate(glm::mat4(1.0f),  glm::radians(40.0f), glm::vec3(1.0, 0, 0));

//	model = glm::rotate(glm::mat4(1.0f), glm::radians(56.0f), glm::vec3(0, 1.0, 0));//
//	model = model * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0.0f,1.0));
	
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 0, 1.0));

	
	
	glm::vec4  world_point = model * point;
	std::cout << glm::to_string(world_point) << std::endl;
	
	glm::mat4 view = lookAt(glm::vec3(0, 0, 48.82), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	*/

	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 48.82), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	

	view[0] = glm::vec4(0.83, -0.30, 0.47, 0);
	view[1] = glm::vec4(0.0, 0.84, 0.54, 0);
	view[2] = glm::vec4(-0.56, -0.45, 0.7, 0);
	view[3] = glm::vec4(0.0, 0.0, -44.47, 1);

	std::cout << glm::to_string(view) << std::endl;

	glm::mat3 R = glm::mat3(view);
	glm::vec3 t = glm::vec3(view[3]);  // translation column

	glm::vec3 cameraPos = -glm::transpose(R) * t;

	glm::vec3 right = glm::vec3(view[0][0], view[1][0], view[2][0]);
	glm::vec3 up = glm::vec3(view[0][1], view[1][1], view[2][1]);
	glm::vec3 forward = -glm::vec3(view[0][2], view[1][2], view[2][2]);

		

	glm::vec3 lookAtPoint = cameraPos + forward;

	glm::mat4 view2 = lookAt(cameraPos, lookAtPoint, glm::vec3(0,1,0));

	std::cout << glm::to_string(view2) << std::endl;

	
	


	return 0;
}