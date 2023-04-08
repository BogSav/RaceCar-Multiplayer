#pragma once

#include "2dGeometryObject.hpp"

class Circle2d : public GeometryObject2d
{
public:
	Circle2d() = delete;
	Circle2d(const glm::vec2& origin, const float& radius, const int& nrOfSteps, Color color)
	{
		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;

		vertices.emplace_back(glm::vec3{origin.x, origin.y, 1}, color());
		vertices.emplace_back(glm::vec3{origin.x, origin.y, 1} + glm::vec3{radius, 0, 0}, color());

		float angularStep = std::numbers::pi_v<float> / nrOfSteps;
		int nrOfTriangles = nrOfSteps * 4 - 1;
		int trNr = 0;

		for (float currentAngle = 0; trNr < nrOfTriangles; currentAngle += angularStep, trNr++)
		{
			vertices.emplace_back(
				glm::vec3{origin.x, origin.y, 1}
					+ glm::vec3{sinf(currentAngle), cosf(currentAngle), 0} * radius,
				color());

			indices.push_back(trNr + 1);
			indices.push_back(0);
			indices.push_back(trNr + 2);
		}

		indices.push_back(1);
		indices.push_back(0);
		indices.push_back(nrOfTriangles + 1);

		m_mesh = std::make_unique<Mesh>();

		m_mesh->InitFromData(vertices, indices);
	}
};