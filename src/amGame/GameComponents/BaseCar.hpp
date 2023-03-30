#pragma once

#include "GameComponent.hpp"

class BaseCar : public GameComponent
{
protected:
	BaseCar(const Shader* const shader, std::shared_ptr<CustomCamera> camera)
		: GameComponent(), m_shader(shader), m_camera(camera){};

	void InitMesh()
	{
		m_mesh = std::make_unique<Mesh>();
		m_mesh->LoadMesh(
			PATH_JOIN(Engine::GetWindow()->props.selfDir, RESOURCE_PATH::MODELS, "mele"), "F1.obj");
	}

protected:
	const Shader* const m_shader;
	std::shared_ptr<CustomCamera> m_camera;

	std::unique_ptr<Mesh> m_mesh;

	std::unique_ptr<SpotLight> m_headLightLeft;
	std::unique_ptr<SpotLight> m_headLightRight;

	glm::vec3 m_direction = glm::vec3{0.f, 0.f, 0.f};
	float m_angleOrientation = 0.f;
};