#include "PlaceTracker.hpp"
#include "amGame/GameSettings.hpp"

PlaceTracker::PlaceTracker(
	const BaseCar* car, const Track* track, const std::vector<std::unique_ptr<NPCCar>>& npcs)
	: m_searchPositionTrashHold(10),
	  m_track(track),
	  m_car(car),
	  m_trackSize(track->GetInteriorPoints().size()),
	  m_npcs(npcs),
	  m_lapNr(0)
{
	FindInitialPositionOnTrack();
}

void PlaceTracker::UpdateCurrentPositionOnTrack()
{
	int startSearchPosition = static_cast<int>(m_currentPosition - m_searchPositionTrashHold);
	if (startSearchPosition < 0)
		startSearchPosition = static_cast<int>(m_trackSize) + startSearchPosition;

	float minDistance = std::numeric_limits<float>::max();
	size_t minPosition = 0;

	for (size_t i = static_cast<size_t>(startSearchPosition), counter = 0;
		 counter <= 2 * m_searchPositionTrashHold + 1;
		 i++, counter++)
	{
		if (i == m_trackSize)
			i = i % m_trackSize;

		float currentDistance =
			glm::distance2(m_car->GetPosition(), m_track->GetInteriorPoints()[i]);

		if (currentDistance < minDistance)
		{
			minDistance = currentDistance;
			minPosition = i;
		}
	}

	if (minPosition < m_currentPosition)
	{
		m_lapNr++;
	}

	m_currentPosition = minPosition;

	UpdatePlace();
}

bool PlaceTracker::IsOutsideOfTrack()
{
	return glm::dot(
			   m_car->GetPosition() - m_track->GetInteriorPoints()[m_currentPosition],
			   m_car->GetPosition() - m_track->GetExteriorPoints()[m_currentPosition])
		> 0;
}

float PlaceTracker::GetCurrentPositionAsPercent() const
{
	return static_cast<float>(m_currentPosition) / static_cast<float>(m_trackSize) * 100.f;
}

uint8_t PlaceTracker::GedtLapNumber() const
{
	return m_lapNr;
}

void PlaceTracker::FindInitialPositionOnTrack()
{
	float minDistance = std::numeric_limits<float>::max();
	size_t minPosition = 0;

	for (size_t i = 0; i < m_trackSize; i++)
	{
		float currentDistance =
			glm::distance2(m_car->GetPosition(), m_track->GetInteriorPoints()[i]);

		if (currentDistance < minDistance)
		{
			minDistance = currentDistance;
			minPosition = i;
		}
	}

	m_currentPosition = minPosition;

	UpdatePlace();
}

void PlaceTracker::UpdatePlace()
{
	m_place = Engine::GetGameSettings()->m_nrOfPlayers;
	for (auto& npc : m_npcs)
	{
		if (static_cast<size_t>(npc->GetLapNr()) * m_trackSize + npc->GetPositionOnTrack()
			< static_cast<size_t>(m_lapNr) * m_trackSize + m_currentPosition)
			m_place--;
	}
}
