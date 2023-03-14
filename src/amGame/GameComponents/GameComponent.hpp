#pragma once

#include "Game/3dGeometries/3dPolygon.hpp"
#include "Game/3dGeometries/3dPolyhedron.hpp"
#include "Game/3dGeometries/3dTriangle.hpp"
#include "Game/Physics/Collision/BoundingBox.hpp"
#include "Game/Lighting/LightSource.hpp"

class GameComponent
{
protected:
	GameComponent() = default;

public:
	virtual void Update(float) { return; }

	virtual void Render() { return; };
	virtual void Render(
		const glm::vec3& eyePosition,
		const std::vector<const class LightSourceAdapter*>& lightingComponents)
	{
		return;
	};

	BoundingBox GetBBox() const { return m_bbox; }
	glm::vec3 GetPosition() const { return m_position; }

protected:
	glm::mat4 m_modelMatrix = glm::mat4(1);
	glm::vec3 m_position = glm::vec3{0, 0, 0};
	float m_scale = 1.f;

	BoundingBox m_bbox;
};