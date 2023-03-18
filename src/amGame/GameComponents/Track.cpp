#include "Track.hpp"

#include <fstream>

Track::Track(
	const GameSettings* gameSettings)
	: GameComponent(gameSettings)
{
}

void Track::Render()
{
	for (auto& geometry : m_geometries)
	{
		GeometryRenderer::Render(geometry.get(), glm::mat4(1));
	}
}

void Track::Render(
	const glm::vec3& eyePosition,
	const std::vector<const class LightSourceAdapter*>& lightingComponents)
{
}

void Track::Update(float deltaTime)
{
}
