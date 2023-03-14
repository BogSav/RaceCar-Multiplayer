#pragma once

#include "GearBox.hpp"
#include "MainMenu/Utilities/Timer.hpp"
#include "utils/glm_utils.h"

namespace physics
{

struct PhysicsComponents
{
	static PhysicsComponents* InstantiateComponents()
	{
		PhysicsComponents* newComponents = new PhysicsComponents();

		newComponents->mass = 800;  // Kg
		newComponents->gravitationalAcceleration = 9.8052;  // m/s^2
		newComponents->frictionCoefficient = 1.7;  // Adimensional
		newComponents->mass = 800;  // Kg

		newComponents->airDensity = 1.225;  // kg/m^3;
		newComponents->frontalArea = 1.08;  // m^2

		newComponents->windSpeed = 10;  // m/s
		newComponents->windDirection = glm::vec3{0, 0, 1};
		newComponents->dragCoefficient = 0.9;  // Adimensional

		newComponents->speedReductionScalingFcator = 2;

		return newComponents;
	}

	double mass = 0;
	double gravitationalAcceleration = 0;
	double frictionCoefficient = 0;

	double windSpeed = 0;
	double airDensity = 0;
	double dragCoefficient = 0;
	double frontalArea = 0;
	glm::vec3 windDirection = {0, 0, 0};

	double speedReductionScalingFcator = 0;
};


class Engine
{
public:
	Engine() = delete;
	Engine(const PhysicsComponents* physics, GearBox* gearBox, const bool isNpc = false)
		: m_physics(physics), m_gearBox(gearBox){};

	void Reset()
	{
		m_gearBox->Reset();
		v1 = 0;
		v2 = 0;
		F1 = 0;
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
		const glm::vec3& currentPosition, const glm::vec3 carDirection, const double& deltaTime)
	{
		if (!this->UpdateSpeed(carDirection, deltaTime))
			return currentPosition;

		glm::vec3 newPosition = currentPosition
			+ carDirection
				* static_cast<float>(
					(v1 + v2) * deltaTime / 2 / m_physics->speedReductionScalingFcator);

		v1 = v2;

		return newPosition;
	}

	int GetCurrentGear() const { return m_gearBox->GetCurrentGear(); }

	double GetSpeedMps() const { return v1; }
	double GetSpeedKmh() const { return v1 * 3.6; }

private:
	double ComputeResultantForce(const glm::vec3& carDirection) const
	{
		return F1 - ComputeAirResistenForce(carDirection) - ComputeFrictionForce();
	}
	double ComputeGForce() const { return m_physics->mass * m_physics->gravitationalAcceleration; }
	double ComputeFrictionForce() const { return m_physics->frictionCoefficient * ComputeGForce(); }
	double ComputeRelativeSpeed(const glm::vec3& carDirection) const
	{
		return this->v1 - m_physics->windSpeed * glm::dot(m_physics->windDirection, carDirection);
	}
	double ComputeAirResistenForce(const glm::vec3& carDirection) const
	{
		return 0.5 * m_physics->dragCoefficient * m_physics->frontalArea * m_physics->airDensity
			* glm::pow(ComputeRelativeSpeed(carDirection), 2);
	}

	bool UpdateSpeed(const glm::vec3& carDirection, double deltaTime)
	{
		v2 = v1 + ComputeResultantForce(carDirection) * deltaTime / m_physics->mass;
		if (v2 < 0)
		{
			v2 = 0;
			v1 = 0;
			return false;
		}
		return true;
	}

private:
	double v1 = 0;
	double v2 = 0;

	double F1 = 0;

	GearBox* m_gearBox;
	const PhysicsComponents* m_physics;
};
}  // namespace physics