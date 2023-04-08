#pragma once

#include "amGame/GameSettings.hpp"
#include "amUtilities/Timer.hpp"

#include <assert.h>
#include <vector>

namespace CarComponents
{
struct GearBox
{
	GearBox() : m_carParameters(Engine::GetGameSettings()->GetCarParameters())
	{
	}

	void Reset()
	{
		currentGear = 1;
		isInShift = false;
	}

	float GetNeededSpeedToShiftGearDown() const
	{
		return m_carParameters.m_gearShiftLimits[currentGear - 1];
	}
	float GetNeededSpeedToShiftGearUp() { return m_carParameters.m_gearShiftLimits[currentGear]; }
	float GetForceForCurrentGear()
	{
		if (isInShift)
		{
			if (m_shiftingTimer.PassedTime(m_carParameters.m_shiftingTime))
			{
				isInShift = false;
			}
			return 0;
		}
		return m_carParameters.m_gearForces[currentGear];
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
	float GetBrakeForce() const { return m_carParameters.brakeFroce; }

private:
	int currentGear = 1;

	FTimer m_shiftingTimer;
	float isInShift = false;

	const GameSettings::CarParameters& m_carParameters;
};
}  // namespace CarComponents