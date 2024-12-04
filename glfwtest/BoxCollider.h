#pragma once
#include "glm/glm.hpp"

class BoxCollider
{
public:
	BoxCollider() : size(glm::vec3(0.0f)) {};

	glm::vec3 size;
};
