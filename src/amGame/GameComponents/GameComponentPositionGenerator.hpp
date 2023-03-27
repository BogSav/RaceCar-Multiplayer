#pragma once

#include "amGame/GameComponents/GameComponent.hpp"
#include "amGame/GameComponents/StreetLight.hpp"
#include "amGame/GameComponents/Track.hpp"
#include "amGame/GameComponents/Tree.hpp"

class PositionGenerator
{
public:
	PositionGenerator() = delete;

	static glm::vec3 GeneratePosition(
		const Track* track, const bool aroundTrack = false, const size_t elementNr = 0);

private:
	static glm::vec3 GeneratePositionAroundTrack(const Track* track, const size_t elementNr);
	static glm::vec3 GeneratePositionOffTheTrack(const Track* track);

	static bool IsOnTrack(const glm::vec3& pos, const Track* track);
};
