#pragma once

#include "Track.hpp"

class TrackBuilder
{
public:
	TrackBuilder(Track* track);

	void BuildTrack(const Shader* const shader, CustomCamera* camera);

private:
	void ReadTrackPointsFromFile();
	void GenerateExteriorPoints();
	void GenerateGeometries(const Shader* const shader, CustomCamera* camera);
	void GenerateLines(const Shader* const shader, CustomCamera* camera);

private:
	Track* m_track;
};