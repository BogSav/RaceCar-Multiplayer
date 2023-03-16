#pragma once

#include "GameComponent.hpp"

class BaseCar : public GameComponent
{
protected:
	BaseCar(
		const GameSettings* gameSettings,
		const Shader* const shader,
		std::shared_ptr<CustomCamera> camera)
		: GameComponent(gameSettings), m_shader(shader)
	{
		m_camera = camera;
	};

protected:
	const Shader* const m_shader;
	std::shared_ptr<CustomCamera> m_camera;

	std::unique_ptr<SpotLight> m_headLightLeft;
	std::unique_ptr<SpotLight> m_headLightRight;

	glm::vec3 m_direction;
	float m_angleOrientation;
};