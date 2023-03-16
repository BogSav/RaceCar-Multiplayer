#pragma once

#include "amGeometry/GeometryRenderer.hpp"
#include "amGame/Physics/Collision/BoundingBox.hpp"

class GeometryObject3d
{
public:
	GeometryObject3d() = default;
	GeometryObject3d(const Shader* const shader, CustomCamera* const camera, Color color);
	GeometryObject3d(
		const Shader* const shader,
		CustomCamera* const camera,
		std::string meshPath,
		std::string meshName);

	void ComputeInitialBBox();
	BBox GetTranformedBBox(const glm::mat4& transformationMatrix);

	friend GeometryRenderer;

protected:
	void CalculateVerticesNormals(
		std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);

protected:
	std::unique_ptr<Mesh> m_mesh = nullptr;

	const Shader* m_shader = nullptr;
	CustomCamera* m_camera = nullptr;

	Color m_color;

	BBox m_bbox;
};