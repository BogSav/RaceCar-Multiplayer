#pragma once

#include "amGame/GameSettings.hpp"
#include "amGame/Lighting/LightSource.hpp"
#include "amGeometry/3dGeometries/3dGeometriesIncludes.hpp"

class GameComponent
{
protected:
	GameComponent() = delete;
	GameComponent(const GameSettings* gameSettings) : m_gameSettings(gameSettings) {}

public:
	virtual void Update(float) { return; }

	virtual void Render() { return; };
	virtual void Render(
		const glm::vec3& eyePosition,
		const std::vector<const class LightSourceAdapter*>& lightingComponents) const
	{
		return;
	};

	BoundingBox GetBBox() const { return m_bbox; }
	glm::vec3 GetPosition() const { return m_position; }

protected:
	glm::mat4 m_modelMatrix = glm::mat4(1);
	glm::vec3 m_position = glm::vec3{0, 0, 0};
	glm::vec3 m_scale = {1.f, 1.f, 1.f};

	BBox m_bbox;

	const GameSettings* m_gameSettings;
};