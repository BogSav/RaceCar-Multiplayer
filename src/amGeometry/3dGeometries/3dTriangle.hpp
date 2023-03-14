#pragma once

#include "3dGeometryObject.hpp"

class Triangle3d : public GeometryObject3d
{
public:
	Triangle3d(
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& v3,
		const Color color,
		const bool wireframe = false)
	{
		InitMesh(v1, v2, v3, color, wireframe);
	}

	Triangle3d(
		const Shader* const shader,
		CustomCamera* const camera,
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& v3,
		const Color color,
		const bool wireframe = false)
		: GeometryObject3d(shader, camera, color)
	{
		InitMesh(v1, v2, v3, color, wireframe);
	}

private:
	void InitMesh(
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& v3,
		const Color color,
		const bool wireframe)
	{
		std::vector<VertexFormat> vertices;
		vertices.reserve(3);
		vertices.emplace_back(v1, color(), glm::vec3{0, 0, 0});
		vertices.emplace_back(v2, color(), glm::vec3{0, 0, 0});
		vertices.emplace_back(v3, color(), glm::vec3{0, 0, 0});

		std::vector<unsigned int> indices;
		indices.reserve(3);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(0);

		m_mesh = std::make_unique<Mesh>();

		if (wireframe)
		{
			m_mesh->SetDrawMode(GL_LINE_LOOP);
		}

		this->CalculateVerticesNormals(vertices, indices);

		m_mesh->InitFromData(vertices, indices);
	}
};