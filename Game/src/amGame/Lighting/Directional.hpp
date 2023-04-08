#pragma once

#ifdef LIGHT

class Directional LIGHT
{
protected:
	Directional() = delete;
	Directional(glm::vec3 direction) : m_direction(direction) {}

public:
	glm::vec3 GetDirection() const { return m_direction; };

private:
	glm::vec3 m_direction;
};
#endif