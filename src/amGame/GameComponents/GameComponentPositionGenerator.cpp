#include "GameComponentPositionGenerator.hpp"

#include "amGame/GameSettings.hpp"

glm::vec3 PositionGenerator::GeneratePosition(
	const Track* track, const bool aroundTrack, const size_t elementNr)
{
	if (aroundTrack)
		return GeneratePositionAroundTrack(track, elementNr);

	return GeneratePositionOffTheTrack(track);
}

glm::vec3 PositionGenerator::GeneratePositionAroundTrack(const Track* track, const size_t elementNr)
{
	size_t distanceBetweenElements = static_cast<size_t>(std::round(
		track->GetInteriorPoints().size()
		/ Engine::GetGameSettings()->GetWorldParameters().m_nrOfStreetLights));

	return track->GetInteriorPoints()[elementNr * distanceBetweenElements];
}

glm::vec3 PositionGenerator::GeneratePositionOffTheTrack(const Track* track)
{
	std::mt19937 randEngine(std::random_device{}());

	std::uniform_real_distribution<float> posXGenerator(
		Engine::GetGameSettings()->GetFieldParameters().startPos.x,
		Engine::GetGameSettings()->GetFieldParameters().startPos.x
			+ Engine::GetGameSettings()->GetFieldParameters().width);
	std::uniform_real_distribution<float> posZGenerator(
		Engine::GetGameSettings()->GetFieldParameters().startPos.z,
		Engine::GetGameSettings()->GetFieldParameters().startPos.z
			+ Engine::GetGameSettings()->GetFieldParameters().length);

	glm::vec3 pos;

	do
	{
		pos = glm::vec3{posXGenerator(randEngine), 0.f, posZGenerator(randEngine)};
	} while (IsOnTrack(pos, track));

	return pos;
}

bool PositionGenerator::IsOnTrack(const glm::vec3& pos, const Track* track)
{
	for (size_t i = 0; i < track->GetInteriorPoints().size(); i++)
	{
		if (glm::dot(pos - track->GetInteriorPoints()[i], pos - track->GetExteriorPoints()[i]) < 0)
			return true;
	}
	return false;
}
