#pragma once

#include "2dGeometryObject.hpp"

class Polygon2d : public GeometryObject2d
{
public:
	Polygon2d() = delete;
	Polygon2d(
		const glm::vec2& v1,
		const glm::vec2& v2,
		const glm::vec2& v3,
		const glm::vec2& v4,
		Color color,
		const bool wireframe = false)
	{
		std::vector<VertexFormat> vertices;
		vertices.reserve(4);
		vertices.emplace_back(glm::vec3{v1[0], v1[1], 1}, color());
		vertices.emplace_back(glm::vec3{v2[0], v2[1], 1}, color());
		vertices.emplace_back(glm::vec3{v3[0], v3[1], 1}, color());
		vertices.emplace_back(glm::vec3{v4[0], v4[1], 1}, color());

		std::vector<unsigned int> indices;
		indices.reserve(4);
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);

		m_mesh = std::make_unique<Mesh>();

		if (wireframe)
		{
			m_mesh->SetDrawMode(GL_LINE_LOOP);
		}
		else
		{
			indices.push_back(0);
			indices.push_back(2);
		}

		m_mesh->InitFromData(vertices, indices);
	}

	Polygon2d(
		const glm::vec2& v1,
		const float& width,
		const float& height,
		Color color,
		const bool wireframe = false)
		: Polygon2d(
			v1,
			v1 + glm::vec2{0, height},
			v1 + glm::vec2{width, height},
			v1 + glm::vec2{width, 0},
			color,
			wireframe)
	{
	}
};