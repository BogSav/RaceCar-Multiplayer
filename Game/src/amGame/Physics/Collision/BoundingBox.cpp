#include "BoundingBox.hpp"

#include <iostream>

void BoundingBox::InitBBoxBasedOnMesh(const Mesh* mesh)
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
		m_minCorner.z =
			std::min_element(
				mesh->vertices.begin(),
				mesh->vertices.end(),
				[](const auto& a, const auto& b) { return a.position.z < b.position.z; })
				->position.z;


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
		m_maxCorner.z =
			std::max_element(
				mesh->vertices.begin(),
				mesh->vertices.end(),
				[](const auto& a, const auto& b) { return a.position.z < b.position.z; })
				->position.z;

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
	m_minCorner.z = std::min_element(
						mesh->positions.begin(),
						mesh->positions.end(),
						[](const auto& a, const auto& b) { return a.z < b.z; })
						->z;


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
	m_maxCorner.z = std::max_element(
						mesh->positions.begin(),
						mesh->positions.end(),
						[](const auto& a, const auto& b) { return a.z < b.z; })
						->z;
}

void BoundingBox::ResetCornersToOrigin()
{
	m_maxCorner = glm::vec3{0, 0, 0};
	m_minCorner = glm::vec3{0, 0, 0};
}

BoundingBox& BoundingBox::operator+=(const BoundingBox& rhs)
{
	if (rhs.m_minCorner.x < this->m_minCorner.x)
		this->m_minCorner.x = rhs.m_minCorner.x;
	if (rhs.m_minCorner.y < this->m_minCorner.y)
		this->m_minCorner.y = rhs.m_minCorner.y;
	if (rhs.m_minCorner.z < this->m_minCorner.z)
		this->m_minCorner.z = rhs.m_minCorner.z;

	if (rhs.m_maxCorner.x > this->m_maxCorner.x)
		this->m_maxCorner.x = rhs.m_maxCorner.x;
	if (rhs.m_maxCorner.y > this->m_maxCorner.y)
		this->m_maxCorner.y = rhs.m_maxCorner.y;
	if (rhs.m_maxCorner.z > this->m_maxCorner.z)
		this->m_maxCorner.z = rhs.m_maxCorner.z;

	return *this;
}

glm::vec3 BoundingBox::GetCenterPoint() const
{
	return glm::vec3{
		(m_minCorner.x + m_maxCorner.x) / 2.f,
		(m_minCorner.y + m_maxCorner.y) / 2.f,
		(m_minCorner.z + m_maxCorner.z) / 2.f};
}

BoundingBox BoundingBox::GetTranformedBBox(const glm::mat4& modelMatrix) const
{
	BoundingBox aux;

	aux.m_maxCorner = glm::vec3(modelMatrix * glm::vec4(m_maxCorner, 1));
	aux.m_minCorner = glm::vec3(modelMatrix * glm::vec4(m_minCorner, 1));

	aux.RearrangeCorners();

	return aux;
}

void BoundingBox::TransformBBox(const glm::mat4& modelMatrix)
{
	*this = this->GetTranformedBBox(modelMatrix);
}

void BoundingBox::RearrangeCorners()
{
	if (m_maxCorner.x < m_minCorner.x)
		glm::swap(m_maxCorner.x, m_minCorner.x);
	if (m_maxCorner.y < m_minCorner.y)
		glm::swap(m_maxCorner.y, m_minCorner.y);
	if (m_maxCorner.z < m_minCorner.z)
		glm::swap(m_maxCorner.z, m_minCorner.z);
}

BoundingBox operator+(const BoundingBox& lhs, const BoundingBox& rhs)
{
	return BoundingBox();
}

std::ostream& operator<<(std::ostream& out, BoundingBox& rhs)
{
	return out << rhs.m_maxCorner << " " << rhs.m_minCorner << std::endl;
}
