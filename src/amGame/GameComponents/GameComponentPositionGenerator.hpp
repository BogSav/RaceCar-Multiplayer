#pragma once

#include "amGame/GameComponents/GameComponent.hpp"
#include "amGame/GameComponents/Track.hpp"

class PositionGenerator
{
public:
	PositionGenerator() = delete;

	static glm::vec3 GeneratePosition(const GameSettings* gameSettings, const Track* track)
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

private:
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
