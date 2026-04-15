#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>


struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    Vertex(glm::vec3 _pos);
};

#endif // VERTEX_H
