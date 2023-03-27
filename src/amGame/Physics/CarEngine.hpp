#pragma once

#include "CarGearBox.hpp"
#include "utils/glm_utils.h"


namespace CarComponents
{
class CarEngine
{
public:
	CarEngine() = delete;
	CarEngine(GearBox* gearBox)
		: m_physicsParameters(Engine::GetGameSettings()->GetPhysicsParameters()),
		  m_carParameters(Engine::GetGameSettings()->GetCarParameters()),
		  m_gearBox(gearBox){};

	void Reset()
	{
		m_gearBox->Reset();
		v1 = 0.f;
		v2 = 0.f;
		F1 = 0.f;
	}

	void Accelerate()
	{
		if (m_gearBox->GetCurrentGear() == 8)
			return;

		if (v1 > m_gearBox->GetNeededSpeedToShiftGearUp())
		{
			m_gearBox->ShiftGearUp();
		}

		F1 = m_gearBox->GetForceForCurrentGear();
	}

	void Brake()
	{
		if (m_gearBox->GetCurrentGear() == 0)
			return;

		if (v1 < m_gearBox->GetNeededSpeedToShiftGearDown())
		{
			m_gearBox->ShiftGearDown();
		}

		F1 = -m_gearBox->GetBrakeForce();
	}

	void InertialDeccelerate()
	{
		if (m_gearBox->GetCurrentGear() == 0)
			return;

		if (v1 < m_gearBox->GetNeededSpeedToShiftGearDown())
		{
			m_gearBox->ShiftGearDown();
		}

		F1 = 0;
	}

	glm::vec3 GetNewPosition(
		const glm::vec3& currentPosition, const glm::vec3 carDirection, const float& deltaTime)
	{
		if (!this->UpdateSpeed(carDirection, deltaTime))
			return currentPosition;

		glm::vec3 newPosition = currentPosition
			+ carDirection * (v1 + v2) * deltaTime / 2.f
				/ m_physicsParameters.speedReductionScalingFcator;

		v1 = v2;

		return newPosition;
	}

	int GetCurrentGear() const { return m_gearBox->GetCurrentGear(); }

	float GetSpeedMps() const { return v1; }
	float GetSpeedKmh() const { return v1 * 3.6f; }

private:
	float ComputeResultantForce(const glm::vec3& carDirection) const
	{
		return F1 - ComputeAirResistenForce(carDirection) - ComputeFrictionForce();
	}
	float ComputeGForce() const
	{
		return m_carParameters.mass * m_physicsParameters.gravitationalAcceleration;
	}
	float ComputeFrictionForce() const
	{
		return m_carParameters.frictionCoefficient * ComputeGForce();
	}
	float ComputeRelativeSpeed(const glm::vec3& carDirection) const
	{
		return this->v1
			- m_physicsParameters.windSpeed
			* glm::dot(m_physicsParameters.windDirection, carDirection);
	}
	float ComputeAirResistenForce(const glm::vec3& carDirection) const
	{
		return 0.5f * m_carParameters.dragCoefficient * m_carParameters.frontalArea
			* m_physicsParameters.airDensity * glm::pow(ComputeRelativeSpeed(carDirection), 2.f);
	}

	bool UpdateSpeed(const glm::vec3& carDirection, float deltaTime)
	{
		v2 = v1 + ComputeResultantForce(carDirection) * deltaTime / m_carParameters.mass;
		if (v2 < 0)
		{
			v2 = 0;
			v1 = 0;
			return false;
		}
		return true;
	}

private:
	float v1 = 0.f;
	float v2 = 0.f;

	float F1 = 0.f;

	GearBox* m_gearBox;
	const GameSettings::PhysicsParameters& m_physicsParameters;
	const GameSettings::CarParameters& m_carParameters;
};
}  // namespace CarComponents