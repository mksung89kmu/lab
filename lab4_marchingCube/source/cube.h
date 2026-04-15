#ifndef CUBE_H
#define CUBE_H

#include <glm\glm.hpp>
#include <ControlNode.h>
#include <vector>
#include <utility>
#include <Vertex.h>

using namespace std;


// cube class to generate and store a cube
// with its edges' nodes

class Cube
{
    public:
        ControlNode **controlNodes;
        int *edgeNodes;
		//what does configuration do? it is an 8 bit number representing the state of the 8 control nodes of the cube, which is used to determine which vertices to draw according to the triangulation table
        int configuration;
        int bordering;

        Cube();
        Cube(ControlNode **_controlNodes, int _bordering);

        // create the necessary edge vertices of the cube
        void createVertices(vector<Vertex>& vertices, float surfaceLevel);

        virtual ~Cube();

    protected:

    private:
        glm::vec3 interpolate(int a, int b, float t);
};

#endif // CUBE_H
