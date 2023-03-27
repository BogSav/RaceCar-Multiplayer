#pragma once

#include "GameComponentPositionGenerator.hpp"

class StreetLight : public GameComponent
{
public:
	StreetLight(
		const Shader* const shader,
		CustomCamera* const camera,
		std::shared_ptr<Texture2D>& texture);

	StreetLight(
		const Shader* const shader,
		CustomCamera* const camera,
		std::shared_ptr<Texture2D>& texture,
		std::string meshPath,
		std::string meshName);

	void Render() override;
	void Render(
		const glm::vec3& eyePosition, const LightSourcesVector& lightingComponents) override;

	void InstantiateLightSources();

	std::pair<const LightSourceAdapter*, const LightSourceAdapter*> GetLightSources() const;

private:
	std::shared_ptr<Texture2D> m_texture;
	std::unique_ptr<GeometryObject3d> m_base;
	std::unique_ptr<GeometryObject3d> m_head;

	std::unique_ptr<LightSourceAdapter> m_leftBulb;
	std::unique_ptr<LightSourceAdapter> m_rightBulb;
};