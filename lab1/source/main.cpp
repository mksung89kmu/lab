
#define GLM_ENABLE_EXPERIMENTAL 
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

int main()
{

	glm::vec3 vecA(1.0f, 2.0f, 3.0f);
	glm::vec3 vecB(1.0f, 2.0f, 3.0f);

	vecA += vecB;

//	glm::vec4 vecB(4.0f, 5.0f, 6.0f, 7.0f);

	std::cout << glm::to_string(vecA) << std::endl;
	return 0;
}