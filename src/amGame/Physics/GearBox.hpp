#pragma once

#include "MainMenu/Utilities/Timer.hpp"

#include <assert.h>
#include <vector>

namespace physics
{
struct GearBox
{
	void Reset()
	{
		currentGear = 1;
		isInShift = false;
	}

	double GetNeededSpeedToShiftGearDown() const { return speeds[currentGear - 1]; }
	double GetNeededSpeedToShiftGearUp() { return speeds[currentGear]; }
	double GetForceForCurrentGear()
	{
		if (isInShift)
		{
			if (m_shiftingTimer.PassedTime(m_shiftingTime))
			{
				isInShift = false;
			}
			return 0;
		}
		return forces[currentGear];
	}
	void ShiftGearUp()
	{
		assert(currentGear < 7);
		currentGear++;
		isInShift = true;
	};
	void ShiftGearDown()
	{
		assert(currentGear > 0);
		currentGear--;
	};
	int GetCurrentGear() const { return currentGear; }
	double GetBrakeForce() const { return brakeFroce; }

private:
	std::vector<double> forces = {0, 40000, 35000, 30000, 25000, 23000, 20000, 18000};  // Newtons
	std::vector<double> speeds = {0, 20, 40, 55, 70, 85, 110, 200};  // m/s
	int currentGear = 1;
	double brakeFroce = 30000;

	double m_shiftingTime = 0.4;
	DTimer m_shiftingTimer;
	bool isInShift = false;
};
}  // namespace physics