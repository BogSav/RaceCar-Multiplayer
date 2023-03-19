#pragma once

#include "amGame/GameComponents/GameComponent.hpp"
#include "amGame/GameComponents/Track.hpp"

class PositionGenerator
{
public:
	PositionGenerator() = delete;

	static glm::vec3 GeneratePosition(
		const GameSettings* gameSettings,
		const Track* track,
		const bool aroundTrack = false,
		const size_t elementNr = 0)
	{
		if (aroundTrack)
			return GeneratePositionAroundTrack(gameSettings, track, elementNr);

		return GeneratePositionOffTheTrack(gameSettings, track);
	}

private:
	static glm::vec3 GeneratePositionAroundTrack(
		const GameSettings* gameSettings, const Track* track, const size_t elementNr)
	{
		size_t distanceBetweenElements = static_cast<size_t>(std::round(
			track->GetInteriorPoints().size()
			/ gameSettings->GetWorldParameters().m_nrOfStreetLights));

		return track->GetInteriorPoints()[elementNr * distanceBetweenElements];
	}

	static glm::vec3 GeneratePositionOffTheTrack(
		const GameSettings* gameSettings, const Track* track)
	{
		std::mt19937 randEngine(std::random_device{}());

		std::uniform_real_distribution<float> posXGenerator(
			gameSettings->GetFieldParameters().startPos.x,
			gameSettings->GetFieldParameters().startPos.x
				+ gameSettings->GetFieldParameters().width);
		std::uniform_real_distribution<float> posZGenerator(
			gameSettings->GetFieldParameters().startPos.z,
			gameSettings->GetFieldParameters().startPos.z
				+ gameSettings->GetFieldParameters().length);

		glm::vec3 pos;

		do
		{
			pos = glm::vec3{posXGenerator(randEngine), 0.f, posZGenerator(randEngine)};
		} while (IsOnTrack(pos, track));

		return pos;
	}

	static bool IsOnTrack(const glm::vec3& pos, const Track* track)
	{
		for (size_t i = 0; i < track->GetInteriorPoints().size(); i++)
		{
			if (glm::dot(pos - track->GetInteriorPoints()[i], pos - track->GetExteriorPoints()[i])
				< 0)
				return true;
		}
		return false;
	}
};
