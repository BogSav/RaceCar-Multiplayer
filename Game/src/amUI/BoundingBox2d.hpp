#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include <iostream>

class BoundingBox2d
{
public:
	void InitBBoxBasedOnMesh(const Mesh* mesh)
	{
		if (mesh->vertices.size() != 0)
		{
			m_minCorner.x =
				std::min_element(
					mesh->vertices.begin(),
					mesh->vertices.end(),
					[](const auto& a, const auto& b) { return a.position.x < b.position.x; })
					->position.x;
			m_minCorner.y =
				std::min_element(
					mesh->vertices.begin(),
					mesh->vertices.end(),
					[](const auto& a, const auto& b) { return a.position.y < b.position.y; })
					->position.y;


			m_maxCorner.x =
				std::max_element(
					mesh->vertices.begin(),
					mesh->vertices.end(),
					[](const auto& a, const auto& b) { return a.position.x < b.position.x; })
					->position.x;
			m_maxCorner.y =
				std::max_element(
					mesh->vertices.begin(),
					mesh->vertices.end(),
					[](const auto& a, const auto& b) { return a.position.y < b.position.y; })
					->position.y;

			return;
		}

		m_minCorner.x = std::min_element(
							mesh->positions.begin(),
							mesh->positions.end(),
							[](const auto& a, const auto& b) { return a.x < b.x; })
							->x;
		m_minCorner.y = std::min_element(
							mesh->positions.begin(),
							mesh->positions.end(),
							[](const auto& a, const auto& b) { return a.y < b.y; })
							->y;


		m_maxCorner.x = std::max_element(
							mesh->positions.begin(),
							mesh->positions.end(),
							[](const auto& a, const auto& b) { return a.x < b.x; })
							->x;
		m_maxCorner.y = std::max_element(
							mesh->positions.begin(),
							mesh->positions.end(),
							[](const auto& a, const auto& b) { return a.y < b.y; })
							->y;
	}

	void ResetCornersToOrigin()
	{
		m_maxCorner = glm::vec2{0, 0};
		m_minCorner = glm::vec2{0, 0};
	}

	BoundingBox2d& operator+=(const BoundingBox2d& rhs)
	{
		if (rhs.m_minCorner.x < this->m_minCorner.x)
			this->m_minCorner.x = rhs.m_minCorner.x;
		if (rhs.m_minCorner.y < this->m_minCorner.y)
			this->m_minCorner.y = rhs.m_minCorner.y;

		if (rhs.m_maxCorner.x > this->m_maxCorner.x)
			this->m_maxCorner.x = rhs.m_maxCorner.x;
		if (rhs.m_maxCorner.y > this->m_maxCorner.y)
			this->m_maxCorner.y = rhs.m_maxCorner.y;

		return *this;
	}

	glm::vec2 GetCenterPoint() const
	{
		return glm::vec2{
			(m_minCorner.x + m_maxCorner.x) / 2.f, (m_minCorner.y + m_maxCorner.y) / 2.f};
	}

	BoundingBox2d GetTranformedBBox(const glm::mat3& modelMatrix) const
	{
		BoundingBox2d aux;

		aux.m_maxCorner = glm::vec2(modelMatrix * glm::vec3(m_maxCorner, 1));
		aux.m_minCorner = glm::vec2(modelMatrix * glm::vec3(m_minCorner, 1));

		aux.RearrangeCorners();

		return aux;
	}

	void TransformBBox(const glm::mat4& modelMatrix)
	{
		*this = this->GetTranformedBBox(modelMatrix);
	}

	void RearrangeCorners()
	{
		if (m_maxCorner.x < m_minCorner.x)
			glm::swap(m_maxCorner.x, m_minCorner.x);
		if (m_maxCorner.y < m_minCorner.y)
			glm::swap(m_maxCorner.y, m_minCorner.y);
	}

	bool IsInside(glm::vec2 point) const
	{
		return !(
			point[0] > m_maxCorner[0] || point[0] < m_minCorner[0] || point[1] > m_maxCorner[1]
			|| point[1] < m_minCorner[1]);
	}

private:
	glm::vec2 m_maxCorner;
	glm::vec2 m_minCorner;
};
