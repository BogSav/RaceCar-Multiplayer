#pragma once

#include "2dGeometryObject.hpp"

class SemiCircle2d : public GeometryObject2d
{
public:
	SemiCircle2d() = delete;
	SemiCircle2d(const glm::vec2& origin, const float& radius, const int& nrOfSteps, Color color)
	{
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;

		vertices.emplace_back(glm::vec3{origin.x, origin.y, 1}, color());
		vertices.emplace_back(glm::vec3{origin.x, origin.y, 1} + glm::vec3{radius, 0, 0}, color());

		int nrOfTriangles = nrOfSteps + 1;
		float angularStep = std::numbers::pi_v<float> / nrOfTriangles;
		int trNr = 0;

		for (float currentAngle = angularStep; trNr < nrOfTriangles;
			 currentAngle += angularStep, trNr++)
		{
			vertices.emplace_back(
				glm::vec3{origin.x, origin.y, 1}
					+ glm::vec3{cosf(currentAngle), sinf(currentAngle), 0} * radius,
				color());

			indices.push_back(trNr + 1);
			indices.push_back(0);
			indices.push_back(trNr + 2);
		}

		m_mesh = std::make_unique<Mesh>();

		m_mesh->InitFromData(vertices, indices);
	}
};