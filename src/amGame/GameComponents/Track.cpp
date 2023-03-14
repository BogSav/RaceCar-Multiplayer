#include "Track.hpp"

#include <fstream>

Track::Track(
	const Shader* const shader,
	CustomCamera* const camera,
	const float& width,
	Color roadColor,
	Color linesColor,
	const float& scaleFactor,
	std::string trackName)
	: GameComponent(shader, camera),
	  m_width(width),
	  m_roadColor(roadColor),
	  m_linesColor(linesColor),
	  m_trackName(trackName),
	  m_scaleFactor(scaleFactor)
{
	this->InitTrack();
}

void Track::InitTrack()
{
	this->ReadTrackPointsFromFile();

	this->GenerateExteriorPoints();

	this->GenerateGeometries();
	this->GenerateLines();
}

void Track::ReadTrackPointsFromFile()
{
	std::string fileName = m_trackName + ".txt";
	std::string path = PATH_JOIN(SOURCE_PATH::MainMenu, "Piste", fileName);

	std::ifstream inputf;

	try
	{
		inputf.open(path);

		if (inputf.fail())
			throw std::exception("Dfk??!!");

		size_t size;
		inputf >> size;

		m_trackPoints.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			float x, y, z;
			inputf >> x >> y >> z;

			m_trackPoints.emplace_back(m_scaleFactor * x, 0, m_scaleFactor * -y);
		}

		inputf.close();
	}
	catch (std::exception e)
	{
		throw e;
	}
}

void Track::GenerateExteriorPoints()
{
	m_exteriorPoints.reserve(m_trackPoints.size());

	{
		const glm::vec3 segmentDirection1 =
			*m_trackPoints.cbegin() - *std::prev(m_trackPoints.cend());
		const glm::vec3 exteriorDirection1 = glm::cross(segmentDirection1, m_zAxisNormal);

		const glm::vec3 segmentDirection2 =
			*m_trackPoints.cbegin() - *std::next(m_trackPoints.cbegin());
		const glm::vec3 exteriorDirection2 = glm::cross(m_zAxisNormal, segmentDirection2);

		const glm::vec3 finalExteriorDirection =
			glm::normalize(exteriorDirection1 + exteriorDirection2);

		const glm::vec3 exteriorPoint =
			*m_trackPoints.cbegin() + finalExteriorDirection * m_width;

		m_exteriorPoints.push_back(exteriorPoint);
	}

	for (auto it = m_trackPoints.cbegin(); it != std::prev(m_trackPoints.cend(), 2); ++it)
	{
		const glm::vec3 segmentDirection1 = *std::next(it) - *it;
		const glm::vec3 exteriorDirection1 = glm::cross(segmentDirection1, m_zAxisNormal);

		const glm::vec3 segmentDirection2 = *std::next(it) - *std::next(it, 2);
		const glm::vec3 exteriorDirection2 = glm::cross(m_zAxisNormal, segmentDirection2);

		const glm::vec3 finalExteriorDirection =
			glm::normalize((exteriorDirection1 + exteriorDirection2) / 2.f);

		const glm::vec3 exteriorPoint = *std::next(it) + finalExteriorDirection * m_width;

		m_exteriorPoints.push_back(exteriorPoint);
	}

	{
		const glm::vec3 segmentDirection1 =
			*std::prev(m_trackPoints.end()) - *std::prev(m_trackPoints.end(), 2);
		const glm::vec3 exteriorDirection1 = glm::cross(segmentDirection1, m_zAxisNormal);

		const glm::vec3 segmentDirection2 =
			*std::prev(m_trackPoints.end()) - *m_trackPoints.begin();
		const glm::vec3 exteriorDirection2 = glm::cross(m_zAxisNormal, segmentDirection2);

		const glm::vec3 finalExteriorDirection =
			glm::normalize(exteriorDirection1 + exteriorDirection2);

		const glm::vec3 exteriorPoint =
			*std::prev(m_trackPoints.end()) + finalExteriorDirection * m_width;

		m_exteriorPoints.push_back(exteriorPoint);
	}
}

void Track::GenerateGeometries()
{
	m_geometries.reserve(m_trackPoints.size() * 2);

	for (size_t it = 0; it < m_trackPoints.size() - 1; it++)
	{
		m_geometries.emplace_back(new Triangle3d(
			m_shader,
			m_camera,
			m_exteriorPoints[it],
			m_trackPoints[it],
			m_trackPoints[it + 1],
			m_roadColor));
		m_geometries.emplace_back(new Triangle3d(
			m_shader,
			m_camera,
			m_exteriorPoints[it],
			m_trackPoints[it + 1],
			m_exteriorPoints[it + 1],
			m_roadColor));
	}
	{
		m_geometries.emplace_back(new Triangle3d(
			m_shader,
			m_camera,
			*std::prev(m_exteriorPoints.end()),
			*std::prev(m_trackPoints.end()),
			*m_trackPoints.begin(),
			m_roadColor));
		m_geometries.emplace_back(new Triangle3d(
			m_shader,
			m_camera,
			*std::prev(m_exteriorPoints.end()),
			*m_trackPoints.begin(),
			*m_exteriorPoints.begin(),
			m_roadColor));
	}
}

void Track::GenerateLines()
{
	for (size_t i = 0; i < m_trackPoints.size() - 1; i += 2)
	{
		m_geometries.emplace_back(new Polygon3d(
			m_shader,
			m_camera,
			utils::GetInterpolatedPoint(m_trackPoints[i], m_exteriorPoints[i], 0.48f)
				+ glm::vec3{0, 0.001, 0},
			utils::GetInterpolatedPoint(m_trackPoints[i], m_exteriorPoints[i], 0.52f)
				+ glm::vec3{0, 0.001, 0},
			utils::GetInterpolatedPoint(m_trackPoints[i + 1], m_exteriorPoints[i + 1], 0.52f)
				+ glm::vec3{0, 0.001, 0},
			utils::GetInterpolatedPoint(m_trackPoints[i + 1], m_exteriorPoints[i + 1], 0.48f)
				+ glm::vec3{0, 0.001, 0},
			m_linesColor));
	}
}
