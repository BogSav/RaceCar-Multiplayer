#pragma once

#include "amGame/GameComponents/BaseCar.hpp"
#include "amGame/GameComponents/Track.hpp"

class PlaceTracker
{
public:
	PlaceTracker() = delete;
	PlaceTracker(const BaseCar* car, const Track* track);

	void UpdateCurrentPositionOnTrack();

	bool IsOutsideOfTrack();

	size_t GetCurrentPosition() { return m_currentPosition; }
	float GetCurrentPositionAsPercent();

private:
	void FindInitialPositionOnTrack();

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