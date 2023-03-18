#include "TrackBuilder.hpp"

#include "amUtilities/Utilities.hpp"

#include <fstream>

TrackBuilder::TrackBuilder(const GameSettings* gameSettings, Track* track)
	: m_gameSettings(gameSettings), m_track(track)
{
}

void TrackBuilder::BuildTrack(const Shader* const shader, CustomCamera* camera)
{
	ReadTrackPointsFromFile();

	GenerateExteriorPoints();

	GenerateGeometries(shader, camera);
	GenerateLines(shader, camera);
}

void TrackBuilder::ReadTrackPointsFromFile()
{
	std::string fileName = m_gameSettings->GetTrackParameters().m_trackName + ".txt";
	std::string path = PATH_JOIN(SOURCE_PATH::Game, "Piste", fileName);

	std::ifstream inputf;

	try
	{
		inputf.open(path);

		if (inputf.fail())
			throw std::exception("Dfk??!!");

		size_t size;
		inputf >> size;

		m_track->m_trackPoints.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			float x, y, z;
			inputf >> x >> y >> z;

			m_track->m_trackPoints.emplace_back(
				m_gameSettings->GetTrackParameters().m_scaleFactor * x,
				0,
				m_gameSettings->GetTrackParameters().m_scaleFactor * -y);
		}

		inputf.close();
	}
	catch (std::exception e)
	{
		throw e;
	}
}

void TrackBuilder::GenerateExteriorPoints()
{
	m_track->m_exteriorPoints.reserve(m_track->m_trackPoints.size());
	glm::vec3 zAxisNormal = glm::vec3{0, 1, 0};
	{
		const glm::vec3 segmentDirection1 =
			*m_track->m_trackPoints.cbegin() - *std::prev(m_track->m_trackPoints.cend());
		const glm::vec3 exteriorDirection1 = glm::cross(segmentDirection1, zAxisNormal);

		const glm::vec3 segmentDirection2 =
			*m_track->m_trackPoints.cbegin() - *std::next(m_track->m_trackPoints.cbegin());
		const glm::vec3 exteriorDirection2 = glm::cross(zAxisNormal, segmentDirection2);

		const glm::vec3 finalExteriorDirection =
			glm::normalize(exteriorDirection1 + exteriorDirection2);

		const glm::vec3 exteriorPoint = *m_track->m_trackPoints.cbegin()
			+ finalExteriorDirection * m_gameSettings->GetTrackParameters().m_width;

		m_track->m_exteriorPoints.push_back(exteriorPoint);
	}

	for (auto it = m_track->m_trackPoints.cbegin();
		 it != std::prev(m_track->m_trackPoints.cend(), 2);
		 ++it)
	{
		const glm::vec3 segmentDirection1 = *std::next(it) - *it;
		const glm::vec3 exteriorDirection1 = glm::cross(segmentDirection1, zAxisNormal);

		const glm::vec3 segmentDirection2 = *std::next(it) - *std::next(it, 2);
		const glm::vec3 exteriorDirection2 = glm::cross(zAxisNormal, segmentDirection2);

		const glm::vec3 finalExteriorDirection =
			glm::normalize((exteriorDirection1 + exteriorDirection2) / 2.f);

		const glm::vec3 exteriorPoint =
			*std::next(it) + finalExteriorDirection * m_gameSettings->GetTrackParameters().m_width;

		m_track->m_exteriorPoints.push_back(exteriorPoint);
	}

	{
		const glm::vec3 segmentDirection1 =
			*std::prev(m_track->m_trackPoints.end()) - *std::prev(m_track->m_trackPoints.end(), 2);
		const glm::vec3 exteriorDirection1 = glm::cross(segmentDirection1, zAxisNormal);

		const glm::vec3 segmentDirection2 =
			*std::prev(m_track->m_trackPoints.end()) - *m_track->m_trackPoints.begin();
		const glm::vec3 exteriorDirection2 = glm::cross(zAxisNormal, segmentDirection2);

		const glm::vec3 finalExteriorDirection =
			glm::normalize(exteriorDirection1 + exteriorDirection2);

		const glm::vec3 exteriorPoint = *std::prev(m_track->m_trackPoints.end())
			+ finalExteriorDirection * m_gameSettings->GetTrackParameters().m_width;

		m_track->m_exteriorPoints.push_back(exteriorPoint);
	}
}

void TrackBuilder::GenerateGeometries(const Shader* const shader, CustomCamera* camera)
{
	m_track->m_geometries.reserve(m_track->m_trackPoints.size() * 2);

	for (size_t it = 0; it < m_track->m_trackPoints.size() - 1; it++)
	{
		m_track->m_geometries.emplace_back(new Triangle3d(
			shader,
			camera,
			m_track->m_exteriorPoints[it],
			m_track->m_trackPoints[it],
			m_track->m_trackPoints[it + 1],
			m_gameSettings->GetTrackParameters().m_roadColor));
		m_track->m_geometries.emplace_back(new Triangle3d(
			shader,
			camera,
			m_track->m_exteriorPoints[it],
			m_track->m_trackPoints[it + 1],
			m_track->m_exteriorPoints[it + 1],
			m_gameSettings->GetTrackParameters().m_roadColor));
	}
	{
		m_track->m_geometries.emplace_back(new Triangle3d(
			shader,
			camera,
			*std::prev(m_track->m_exteriorPoints.end()),
			*std::prev(m_track->m_trackPoints.end()),
			*m_track->m_trackPoints.begin(),
			m_gameSettings->GetTrackParameters().m_roadColor));
		m_track->m_geometries.emplace_back(new Triangle3d(
			shader,
			camera,
			*std::prev(m_track->m_exteriorPoints.end()),
			*m_track->m_trackPoints.begin(),
			*m_track->m_exteriorPoints.begin(),
			m_gameSettings->GetTrackParameters().m_roadColor));
	}
}

void TrackBuilder::GenerateLines(const Shader* const shader, CustomCamera* camera)
{
	for (size_t i = 0; i < m_track->m_trackPoints.size() - 1; i += 2)
	{
		m_track->m_geometries.emplace_back(new Polygon3d(
			shader,
			camera,
			utils::GetInterpolatedPoint(
				m_track->m_trackPoints[i], m_track->m_exteriorPoints[i], 0.48f)
				+ glm::vec3{0, 0.001, 0},
			utils::GetInterpolatedPoint(
				m_track->m_trackPoints[i], m_track->m_exteriorPoints[i], 0.52f)
				+ glm::vec3{0, 0.001, 0},
			utils::GetInterpolatedPoint(
				m_track->m_trackPoints[i + 1], m_track->m_exteriorPoints[i + 1], 0.52f)
				+ glm::vec3{0, 0.001, 0},
			utils::GetInterpolatedPoint(
				m_track->m_trackPoints[i + 1], m_track->m_exteriorPoints[i + 1], 0.48f)
				+ glm::vec3{0, 0.001, 0},
			m_gameSettings->GetTrackParameters().m_linesColor));
	}
}