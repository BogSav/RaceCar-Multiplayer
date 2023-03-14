#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

class GameSettings
{
private:
	int m_gameID;

public:
	size_t m_nrOfStreetLights;
	size_t m_nrOfTrees;

	bool m_isMultiplayer;
	size_t m_nrOfPlayers;

	float m_curveCoefficent;

	glm::ivec2 m_resolution;
	
	bool m_frameTimer;
};