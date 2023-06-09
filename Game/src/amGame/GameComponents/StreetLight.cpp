#include "StreetLight.hpp"

StreetLight::StreetLight(
	const Shader* const shader,
	CustomCamera* const camera,
	std::shared_ptr<Texture2D>& texture)
	: StreetLight(
		shader,
		camera,
		texture,
		PATH_JOIN(Engine::GetWindow()->props.selfDir, "assets", "models", "primitives"),
		"box.obj")
{
}

StreetLight::StreetLight(
	const Shader* const shader,
	CustomCamera* const camera,
	std::shared_ptr<Texture2D>& texture,
	std::string meshPath,
	std::string meshName)
	: GameComponent()
{
	m_texture = texture;

	m_base = std::make_unique<GeometryObject3d>(shader, camera, meshPath, meshName);
	m_head = std::make_unique<GeometryObject3d>(shader, camera, meshPath, meshName);

	m_scale = glm::vec3{6.f, 6.f, 6.f};

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

void StreetLight::Render(const glm::vec3& eyePosition, const LightSourcesVector& lightingComponents)
{
	m_modelMatrix = glm::mat4(1);

	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);

	glm::mat4 baseModelMatrix = glm::translate(m_modelMatrix, glm::vec3{0.f, 1.5f, 0.f});
	baseModelMatrix = glm::scale(baseModelMatrix, glm::vec3{.1f, 3.f, .1f});
	GeometryRenderer::Render(
		m_base.get(), baseModelMatrix, eyePosition, lightingComponents, m_texture.get());
	m_bbox = m_base->GetTranformedBBox(baseModelMatrix);

	glm::mat4 headModelMatrix = glm::translate(m_modelMatrix, glm::vec3{0., 3.f, 0.f});
	headModelMatrix = glm::scale(headModelMatrix, glm::vec3{2.f, .1f, .25f});
	GeometryRenderer::Render(
		m_head.get(), headModelMatrix, eyePosition, lightingComponents, m_texture.get());
}

void StreetLight::InstantiateLightSources()
{
	 m_leftBulb = std::make_unique<SpotLight>(
		m_position + glm::vec3{1.3f, 18.f, 0.f},
		Colors::White,
		20.f,
		RADIANS(50),
		glm::vec3{0, -1, 0},
		glm::vec3{-1, 0, 0});
	 m_rightBulb = std::make_unique<SpotLight>(
		m_position + glm::vec3{-1.3, 18.f, 0.f},
		Colors::White,
		20.f,
		RADIANS(50),
		glm::vec3{0, -1, 0},
		glm::vec3{-1, 0, 0});
	//m_leftBulb =
	//	std::make_unique<PointLight>(m_position + glm::vec3{1.3f, 12.f, 0.f}, Colors::White, 5.f);
	//m_rightBulb =
	//	std::make_unique<PointLight>(m_position + glm::vec3{-1.3, 12.f, 0.f}, Colors::White, 5.f);
}

std::pair<const LightSourceAdapter*, const LightSourceAdapter*> StreetLight::GetLightSources() const
{
	{
		return std::pair<const LightSourceAdapter*, const LightSourceAdapter*>{
			m_leftBulb.get(), m_rightBulb.get()};
	}
}
