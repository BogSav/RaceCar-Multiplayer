#pragma once

#include "GameComponent.hpp"

class Track : public GameComponent
{
public:
	Track() = delete;
	Track(const GameSettings* gameSettings);

	const std::vector<glm::vec3>& GetInteriorPoints() const { return m_trackPoints; };
	const std::vector<glm::vec3>& GetExteriorPoints() const { return m_exteriorPoints; };

	void Render() override;
	void Render(
		const glm::vec3& eyePosition, const LightSourcesVector& lightingComponents) override;

	void Update(float deltaTime) override;

	friend class TrackBuilder;

private:
	std::vector<glm::vec3> m_trackPoints;
	std::vector<glm::vec3> m_exteriorPoints;
	std::vector<std::unique_ptr<GeometryObject3d>> m_geometries;
};