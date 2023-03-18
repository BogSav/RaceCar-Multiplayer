#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "amUtilities/Colors.hpp"

class GameSettings
{
public:
	enum class GameMode
	{
		UNSELECTED = 0,
		SINGLEPLAYER = 1,
		MULTIPLAYER = 2
	};

public:
	struct PhysicsParameters
	{
		void InitParameters()
		{
			this->gravitationalAcceleration = 9.8052f;  // m/s^2

			this->airDensity = 1.225f;  // kg/m^3;
			this->windSpeed = 10.f;  // m/s
			this->windDirection = glm::vec3{0.f, 0.f, 1.f};

			this->speedReductionScalingFcator = 2.f;
		}

		float gravitationalAcceleration = 0.f;

		float windSpeed = 0.f;
		float airDensity = 0.f;
		glm::vec3 windDirection = {0.f, 0.f, 0.f};

		float speedReductionScalingFcator = 0.f;
	};

	struct WorldParameters
	{
		void InitParameters()
		{
			m_nrOfStreetLights = 5;
			m_nrOfTrees = 5;

			// m_trackName = "MonacoF1";
		}

		size_t m_nrOfStreetLights = 0;
		size_t m_nrOfTrees = 0;
	};

	struct CarParameters
	{
		void InitParameters()
		{
			this->brakeFroce = 30000.f;
			this->m_shiftingTime = 0.4f;

			this->mass = 800.f;  // Kg
			this->frictionCoefficient = 1.7f;  // Adimensional
			this->frontalArea = 1.08f;  // m^2
			this->dragCoefficient = 0.9f;  // Adimensional


			this->m_gearForces = {
				0.f, 40000.f, 35000.f, 30000.f, 25000.f, 23000.f, 20000.f, 18000.f};  // Newtons
			this->m_gearShiftLimits = {0.f, 20.f, 40.f, 55.f, 70.f, 85.f, 110.f, 200.f};  // m/s
		}

		std::vector<float> m_gearForces;
		std::vector<float> m_gearShiftLimits;

		float brakeFroce = 0.f;
		float m_shiftingTime = 0.f;

		float mass = 0.f;
		float dragCoefficient = 0.f;
		float frontalArea = 0.f;
		float frictionCoefficient = 0.f;
	};

	struct TrackParaneters
	{
		void InitParameters()
		{
			m_trackName = "MonacoF1";
			m_roadColor = Colors::Gray;
			m_linesColor = Colors::White;

			m_width = 20.f;
			m_scaleFactor = 6.0f;
		}

		std::string m_trackName;
		Color m_roadColor = Colors::Gray;
		Color m_linesColor = Colors::White;

		float m_width = 20.f;
		float m_scaleFactor = 6.0f;
	};

public:
	const PhysicsParameters& GetPhysicsParameters() const { return (m_physicsParameters); }
	const WorldParameters& GetWorldParameters() const { return (m_worldParameters); }
	const CarParameters& GetCarParameters() const { return (m_carParameters); }
	const TrackParaneters& GetTrackParameters() const { return (m_trackParemetrs); }

	void InitParameters()
	{
		m_physicsParameters.InitParameters();
		m_worldParameters.InitParameters();
		
		m_carParameters.InitParameters();
		m_trackParemetrs.InitParameters();
	}

public:
	GameMode m_gameMode = GameMode::UNSELECTED;
	bool m_isMultiplayer;
	size_t m_nrOfPlayers = 0;

	glm::ivec2 m_resolution;

	bool m_frameTimerEnabled;

private:
	PhysicsParameters m_physicsParameters;
	WorldParameters m_worldParameters;

	// Game components parameters
	CarParameters m_carParameters;
	TrackParaneters m_trackParemetrs;
};