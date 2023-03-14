#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

class BoundingBox
{
public:
	BoundingBox() = default;
	BoundingBox(const BoundingBox&) = default;
	BoundingBox& operator=(const BoundingBox&) = default;

	void InitBBoxBasedOnMesh(const Mesh* mesh);
	void ResetCornersToOrigin();

	BoundingBox& operator+=(const BoundingBox&);
	friend BoundingBox operator+(const BoundingBox&, const BoundingBox&);
	friend std::ostream& operator<<(std::ostream&, BoundingBox&);

	glm::vec3 GetMinCorner() const { return m_minCorner; }
	glm::vec3 GetMaxCorner() const { return m_maxCorner; }
	glm::vec3 GetCenterPoint() const;

	BoundingBox GetTranformedBBox(const glm::mat4& modelMatrix) const;
	void TransformBBox(const glm::mat4& modelMatrix);

private:
	void RearrangeCorners();

private:
	glm::vec3 m_minCorner = glm::vec3{0, 0, 0};
	glm::vec3 m_maxCorner = glm::vec3{0, 0, 0};
};