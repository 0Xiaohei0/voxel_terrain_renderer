#pragma once
#include <glm/glm.hpp>

class Transform
{
public:
	Transform() : position(glm::vec3(0.0f, 0.0f, 0.0f)), rotation(glm::vec3(0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f, 1.0f, 1.0f)) {};

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void setPosition(glm::vec3 newPosition) {
		position = newPosition;
	}

	void setRotation(glm::vec3 newRotation) {
		rotation = newRotation;
	}

	void setScale(glm::vec3 newScale) {
		scale = newScale;
	}
private:

};