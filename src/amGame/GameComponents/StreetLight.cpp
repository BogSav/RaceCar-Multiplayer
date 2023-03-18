#include "StreetLight.hpp"

StreetLight::StreetLight(
	const GameSettings* gameSettings,
	const Shader* const shader,
	CustomCamera* const camera,
	const WindowObject* window,
	std::shared_ptr<Texture2D>& texture)
	: StreetLight(
		gameSettings,
		shader,
		camera,
		texture,
		PATH_JOIN(window->props.selfDir, "assets", "models", "primitives"),
		"box.obj")
{
}

StreetLight::StreetLight(
	const GameSettings* gameSettings,
	const Shader* const shader,
	CustomCamera* const camera,
	std::shared_ptr<Texture2D>& texture,
	std::string meshPath,
	std::string meshName)
	: GameComponent(gameSettings)
{
	m_texture = texture;

	m_base = std::make_unique<GeometryObject3d>(shader, camera, meshPath, meshName);
	m_head = std::make_unique<GeometryObject3d>(shader, camera, meshPath, meshName);

	m_base->ComputeInitialBBox();
	m_head->ComputeInitialBBox();
}

void StreetLight::Render()
{
	m_modelMatrix = glm::mat4(1);

	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);

	glm::mat4 baseModelMatrix = glm::translate(m_modelMatrix, glm::vec3{0.f, 1.5f, 0.f});
	baseModelMatrix = glm::scale(baseModelMatrix, glm::vec3{.1f, 3.f, .1f});
	GeometryRenderer::Render(m_base.get(), baseModelMatrix, m_texture.get());
	m_bbox = m_base->GetTranformedBBox(baseModelMatrix);

	glm::mat4 headModelMatrix = glm::translate(m_modelMatrix, glm::vec3{0., 3.f, 0.f});
	headModelMatrix = glm::scale(headModelMatrix, glm::vec3{2.f, .1f, .25f});
	GeometryRenderer::Render(m_head.get(), headModelMatrix, m_texture.get());
}

void StreetLight::InstantiateLightSources()
{
	m_leftBulb = std::make_unique<SpotLight>(
		m_position + glm::vec3{1.3f, 3.f, 0.f},
		Colors::White,
		1.f,
		RADIANS(30),
		glm::vec3{0, -1, 0},
		glm::vec3{-1, 0, 0});
	m_rightBulb = std::make_unique<SpotLight>(
		m_position + glm::vec3{-1.3, 3.f, 0.f},
		Colors::White,
		1.f,
		RADIANS(30),
		glm::vec3{0, -1, 0},
		glm::vec3{-1, 0, 0});
}
