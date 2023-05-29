#pragma once

#include "amGame/GameComponents/BaseCar.hpp"
#include "amGame/GameComponents/Track.hpp"
#include "amGame/GameComponents/NPCCar.hpp"

class PlaceTracker
{
public:
	PlaceTracker() = delete;
	PlaceTracker(
		const BaseCar* car, const Track* track, const std::vector<std::unique_ptr<NPCCar>>& npcs);

	void UpdateCurrentPositionOnTrack();

	bool IsOutsideOfTrack();

	size_t GetCurrentPosition() const { return m_currentPosition; }
	float GetCurrentPositionAsPercent() const;
	uint8_t GedtLapNumber() const; 
	std::size_t GetPlace() const { return m_place; };

private:
	void FindInitialPositionOnTrack();
	void UpdatePlace();

private:
	const BaseCar* m_car;
	const Track* m_track;

	size_t m_currentPosition;
	const size_t m_searchPositionTrashHold;
	const size_t m_trackSize;

	uint8_t m_lapNr;

	std::size_t m_place;

	const std::vector<std::unique_ptr<NPCCar>>& m_npcs;
};