#pragma once

#ifdef LIGHT

#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

class Spot LIGHT
{
protected:
	Spot() = delete;
	Spot(const float& cutOff, const glm::vec3& direction, const glm::vec3& right)
		: m_cutOffAngle(cutOff), m_direction(direction), m_right(right)
	{
	}

public:
	void RotateOx(const float& angle)
	{
		m_direction = glm::normalize(
			glm::rotate(glm::mat4(1.0f), angle, m_right) * glm::vec4(m_direction, 0));
	}
	void RotateOy(const float& angle)
	{
		m_right =
			glm::normalize(glm::rotate(glm::mat4(1.0f), angle, {0, 1, 0}) * glm::vec4(m_right, 0));
		m_direction = glm::normalize(
			glm::rotate(glm::mat4(1.0f), angle, {0, 1, 0}) * glm::vec4(m_direction, 0));
	}

	const float& GetCutOffAngle() const { return m_cutOffAngle; }
	const glm::vec3& GetDirection() const { return m_direction; }
	const glm::vec3& GetRight() const { return m_right; }

	void SetDirection(const glm::vec3& dir) { m_direction = dir; }

private:
	float m_cutOffAngle = 0.f;
	glm::vec3 m_direction = glm::vec3(0);
	glm::vec3 m_right = glm::vec3(0);
};
#endif