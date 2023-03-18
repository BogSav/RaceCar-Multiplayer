#pragma once

#include "Track.hpp"

class TrackBuilder
{
public:
	TrackBuilder(const GameSettings* gameSettings, Track* track);

	void BuildTrack(const Shader* const shader, CustomCamera* camera);

private:
	void ReadTrackPointsFromFile();
	void GenerateExteriorPoints();
	void GenerateGeometries(const Shader* const shader, CustomCamera* camera);
	void GenerateLines(const Shader* const shader, CustomCamera* camera);

private:
	Track* m_track;
	const GameSettings* m_gameSettings;
};