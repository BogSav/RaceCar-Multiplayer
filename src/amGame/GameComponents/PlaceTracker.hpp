#pragma once

#include "amGame/GameComponents/BaseCar.hpp"
#include "amGame/GameComponents/Track.hpp"

class PlaceTracker
{
public:
	PlaceTracker() = delete;
	PlaceTracker(const BaseCar* car, const Track* track)
		: m_searchPositionTrashHold(10),
		  m_track(track),
		  m_car(car),
		  m_trackSize(track->GetInteriorPoints().size())
	{
		FindInitialPositionOnTrack();
	}

	void UpdateCurrentPositionOnTrack()
	{
		int startSearchPosition = m_currentPosition - m_searchPositionTrashHold;
		if (startSearchPosition < 0)
			startSearchPosition = m_trackSize + startSearchPosition;

		float minDistance = std::numeric_limits<float>::max();
		size_t minPosition = -1;

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

	bool IsOutsideOfTrack()
	{
		return glm::dot(
				   m_car->GetPosition() - m_track->GetInteriorPoints()[m_currentPosition],
				   m_car->GetPosition() - m_track->GetExteriorPoints()[m_currentPosition])
			> 0;
	}

	size_t GetCurrentPosition() { return m_currentPosition; }
	float GetCurrentPositionAsPercent()
	{
		return static_cast<float>(m_currentPosition) / static_cast<float>(m_trackSize) * 100.f;
	}

private:
	void FindInitialPositionOnTrack()
	{
		float minDistance = std::numeric_limits<float>::max();
		size_t minPosition = -1;

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

public:
	static int firstPlaceID;
	static size_t firstPlacePosition;
	static size_t firstPlaceLapNumber;

private:
	const BaseCar* m_car;
	const Track* m_track;

	size_t m_currentPosition;
	const size_t m_searchPositionTrashHold;
	const size_t m_trackSize;
};