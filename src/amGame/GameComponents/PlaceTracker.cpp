#include "PlaceTracker.hpp"

PlaceTracker::PlaceTracker(const BaseCar* car, const Track* track)
	: m_searchPositionTrashHold(10),
	  m_track(track),
	  m_car(car),
	  m_trackSize(track->GetInteriorPoints().size())
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

	m_currentPosition = minPosition;
}

bool PlaceTracker::IsOutsideOfTrack()
{
	return glm::dot(
			   m_car->GetPosition() - m_track->GetInteriorPoints()[m_currentPosition],
			   m_car->GetPosition() - m_track->GetExteriorPoints()[m_currentPosition])
		> 0;
}

float PlaceTracker::GetCurrentPositionAsPercent()
{
	return static_cast<float>(m_currentPosition) / static_cast<float>(m_trackSize) * 100.f;
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
}
