#include "3dGeometryObject.hpp"

#include <iostream>
#include <string>

GeometryObject3d::GeometryObject3d(
	const Shader* const shader, CustomCamera* const camera, Color color)
	: m_shader(shader), m_camera(camera), m_color(color)
{
}

GeometryObject3d::GeometryObject3d(
	const Shader* const shader,
	CustomCamera* const camera,
	std::string meshPath,
	std::string meshName)
	: m_shader(shader), m_camera(camera)
{
	m_mesh = std::make_unique<Mesh>();
	m_mesh->LoadMesh(meshPath, meshName);

	m_bbox.InitBBoxBasedOnMesh(m_mesh.get());
}

void GeometryObject3d::ComputeInitialBBox()
{
	m_bbox.InitBBoxBasedOnMesh(m_mesh.get());
}

BBox GeometryObject3d::GetTranformedBBox(const glm::mat4& transformationMatrix)
{
	return m_bbox.GetTranformedBBox(transformationMatrix);
}

void GeometryObject3d::CalculateVerticesNormals(
	std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
	for (size_t i = 0; i < indices.size() - 2; i += 3)
	{
		int vertexA = indices[i];
		int vertexB = indices[i + 1];
		int vertexC = indices[i + 2];

		glm::vec3 edgeAB = vertices[vertexB].position - vertices[vertexA].position;
		glm::vec3 edgeAC = vertices[vertexC].position - vertices[vertexA].position;

		glm::vec3 areaWeightedNormal = glm::cross(edgeAB, edgeAC);

		vertices[vertexA].normal += areaWeightedNormal;
		vertices[vertexB].normal += areaWeightedNormal;
		vertices[vertexC].normal += areaWeightedNormal;
	}

	std::for_each(
		vertices.begin(),
		vertices.end(),
		[](VertexFormat& current) { current.normal = glm::normalize(current.normal); });
}