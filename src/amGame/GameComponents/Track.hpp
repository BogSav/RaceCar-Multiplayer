#pragma once

#include "GameComponent.hpp"

class Track : public GameComponent
{
public:
	Track() = delete;
	Track(
		const GameSettings* gameSettings,
		const Shader* const shader,
		CustomCamera* const camera,
		const float& width,
		Color roadColor = Colors::Asfalt,
		Color linesColor = Colors::White,
		const float& scaleFactor = 6.0f,
		std::string trackName = "MonacoF1");

	const std::vector<glm::vec3>& GetInteriorPoints() const { return m_trackPoints; };

	friend class CollisionEngine;

private:
	void InitTrack();
	void ReadTrackPointsFromFile();

	void GenerateExteriorPoints();
	void GenerateGeometries();
	void GenerateLines();

private:
	std::vector<glm::vec3> m_trackPoints;
	std::vector<glm::vec3> m_exteriorPoints;

	std::vector<std::unique_ptr<GeometryObject3d>> m_geometries; 

	const Color m_roadColor = Colors::Gray;
	const Color m_linesColor;

	const float m_width;
	const float m_scaleFactor;
	const glm::vec3 m_zAxisNormal = glm::vec3{0, 1, 0};
	const std::string m_trackName;

	const Shader* m_shader = nullptr;
	CustomCamera* m_camera = nullptr;
};