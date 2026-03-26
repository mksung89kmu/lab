
#define GLM_ENABLE_EXPERIMENTAL 

#include <iostream>
#include "cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

////////////////////////////////////////////////
float floorColor1[3] = { .7f, .7f, .7f }; // Light color
float floorColor2[3] = { .2f, .2f, .2f }; // Dark color

checkeredFloor::checkeredFloor()
{

	setup(50, 16);

//	setup(50, 1);

}

int nvert;

void checkeredFloor::setup(float size, int nSquares)
{
	std::vector <glm::vec4> vlists;
	std::vector <glm::vec3> clists;


	// parameters:
	float maxX = size / 2, maxY = size / 2;
	float minX = -size / 2, minY = -size / 2;

	int x, y, v[3], i;
	float xp, yp, xd, yd;
	v[2] = 0;
	xd = (maxX - minX) / ((float)nSquares);
	yd = (maxY - minY) / ((float)nSquares);


	
	for (x = 0, xp = minX; x < nSquares; x++, xp += xd) {
		for (y = 0, yp = minY, i = x; y < nSquares; y++, i++, yp += yd) {
			if (i % 2 == 1) {
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
				clists.push_back(glm::vec3(floorColor1[0], floorColor1[1], floorColor1[2]));
			}
			else {
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));
				clists.push_back(glm::vec3(floorColor2[0], floorColor2[1], floorColor2[2]));

			}
			vlists.push_back(glm::vec4(xp, 0, yp, 1));
			vlists.push_back(glm::vec4(xp, 0, yp + yd, 1));
			vlists.push_back(glm::vec4(xp + xd, 0, yp + yd, 1));

			vlists.push_back(glm::vec4(xp, 0, yp, 1));
			vlists.push_back(glm::vec4(xp + xd, 0, yp + yd, 1));
			vlists.push_back(glm::vec4(xp + xd, 0, yp, 1));

		} // end of for j
	}// end of for i
	




	nvert = vlists.size();

	
	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//create vbo for vertices
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*vlists.size(), vlists.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // attribute
		4,                 // number of elements per vertex, here (x,y,z,1)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
		);
	glEnableVertexAttribArray(0);


	//create vbo for colors
	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*clists.size(), clists.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // attribute
		3,                 // number of elements per vertex, here (R,G,B)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
		);
	glEnableVertexAttribArray(1);

	vlists.clear();
	clists.clear();

	glBindVertexArray(0);
}

void checkeredFloor::draw()
{

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, nvert*3);
	glBindVertexArray(0);
}


/////////////

