#pragma once

#include "GameComponent.hpp"

class Tree : public GameComponent
{
public:
	Tree(
		const GameSettings* gameSettings,
		const Shader* const shader,
		CustomCamera* const camera,
		const WindowObject* window,
		std::shared_ptr<Texture2D>& crownTexture,
		std::shared_ptr<Texture2D>& trunkTexture);

	Tree(
		const GameSettings* gameSettings,
		const Shader* const shader,
		CustomCamera* const camera,
		std::shared_ptr<Texture2D>& crownTexture,
		std::shared_ptr<Texture2D>& trunkTexture,
		std::string meshPath,
		std::string trunkMeshName,
		std::string crownMeshName);

	void Render() override;
	void Render(
		const glm::vec3& eyePosition,
		const std::vector<const class LightSourceAdapter*>& lightingComponents);

	void InstantiateLightSource();
	const LightSourceAdapter* GetLightSoruce() const { return m_lightSource.get(); }

private:
	Tree(
		const Shader* const shader,
		CustomCamera* const camera,
		std::shared_ptr<Texture2D>& crownTexture,
		std::shared_ptr<Texture2D>& trunkTexture,
		std::string meshPath,
		std::string trunkMeshName,
		std::string crownMeshName);

private:
	std::shared_ptr<Texture2D> m_trunkTexture;
	std::shared_ptr<Texture2D> m_crownTexture;
	std::vector<std::unique_ptr<GeometryObject3d>> m_objects;

	std::unique_ptr<LightSourceAdapter> m_lightSource;
};