#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm\glm.hpp>


struct Triangle
{
    unsigned int a, b, c;
    glm::vec3 normal;
    Triangle(unsigned int _a, unsigned int _b, unsigned int _c);
};

#endif // TRIANGLE_H
