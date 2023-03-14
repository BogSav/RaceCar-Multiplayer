#pragma once

#include "3dGeometryObject.hpp"

class Polyhedron3d : public GeometryObject3d
{
public:
	Polyhedron3d(
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& v3,
		const glm::vec3& v4,
		const glm::vec3& v5,
		const glm::vec3& v6,
		const glm::vec3& v7,
		const glm::vec3& v8,
		const Color color)
	{
		InitMesh(v1, v2, v3, v4, v5, v6, v7, v8, color);
	}

	Polyhedron3d(
		const Shader* const shader,
		CustomCamera* const camera,
		const glm::vec3& v1,
		const float& width,
		const float& length,
		const float& height,
		const Color color)
		: GeometryObject3d(shader, camera, color)
	{
		InitMesh(
			v1 + glm::vec3{0, 0, length},
			v1 + glm::vec3{width, 0, length},
			v1 + glm::vec3{0, height, length},
			v1 + glm::vec3{width, height, length},
			v1,
			v1 + glm::vec3{width, 0, 0},
			v1 + glm::vec3{0, height, 0},
			v1 + glm::vec3{width, height, 0},
			color);
	}

private:
	void InitMesh(
		const glm::vec3& v1,
		const glm::vec3& v2,
		const glm::vec3& v3,
		const glm::vec3& v4,
		const glm::vec3& v5,
		const glm::vec3& v6,
		const glm::vec3& v7,
		const glm::vec3& v8,
		const Color color)
	{
		std::vector<VertexFormat> vertices;
		vertices.reserve(8);
		vertices.emplace_back(v1, color(), glm::vec3{0, 0, 0});
		vertices.emplace_back(v2, color(), glm::vec3{0, 0, 0});
		vertices.emplace_back(v3, color(), glm::vec3{0, 0, 0});
		vertices.emplace_back(v4, color(), glm::vec3{0, 0, 0});
		vertices.emplace_back(v5, color(), glm::vec3{0, 0, 0});
		vertices.emplace_back(v6, color(), glm::vec3{0, 0, 0});
		vertices.emplace_back(v7, color(), glm::vec3{0, 0, 0});
		vertices.emplace_back(v8, color(), glm::vec3{0, 0, 0});

		std::vector<unsigned int> indices = {0, 1, 2, 1, 3, 2, 2, 3, 7, 2, 7, 6, 1, 7, 3, 1, 5, 7,
											 6, 7, 4, 7, 5, 4, 0, 4, 1, 1, 4, 5, 2, 6, 4, 0, 2, 4};

		m_mesh = std::make_unique<Mesh>();

		this->CalculateVerticesNormals(vertices, indices);

		m_mesh->InitFromData(vertices, indices);
	}
};