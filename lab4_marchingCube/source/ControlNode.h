#ifndef CONTROLNODE_H
#define CONTROLNODE_H

#include <glm\glm.hpp>

// control nodes (i.e. cube vertex) with its state (active or not)

class ControlNode
{
    public:
        glm::vec3 pos;
        float value;
        bool active;

        ControlNode();
        ControlNode(glm::vec3& _pos, float _value, bool _active);
        virtual ~ControlNode();

    protected:

    private:
};

#endif // CONTROLNODE_H
