#include "ControlNode.h"

#include <glm\glm.hpp>

ControlNode::ControlNode(glm::vec3& _pos, float _value, bool _active) : pos(_pos), value(_value), active(_active)
{

}

ControlNode::ControlNode()
{

}

ControlNode::~ControlNode()
{

}
