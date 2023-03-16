#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

class GameSettings
{
public:
	enum class GameMode
	{
		UNSELECTED = 0,
		SINGLEPLAYER = 1,
		MULTIPLAYER = 2
	};

	struct PhysicsParameters
	{
		void InitComponentsWithDefaultValues()
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

	struct CarParameters
	{
		void InitComponentsWithDefaultValues()
		{
			this->brakeFroce = 30000.f;
			this->m_shiftingTime = 0.4f;

			this->mass = 800.f;  // Kg
			this->frictionCoefficient = 1.7f;  // Adimensional
			this->frontalArea = 1.08f;  // m^2
			this->dragCoefficient = 0.9f;  // Adimensional


			this->m_gearForces = {0.f, 40000.f, 35000.f, 30000.f, 25000.f, 23000.f, 20000.f, 18000.f};  // Newtons
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

	struct WorldParameters
	{
		void InitWorldParametersToDefaultValues()
		{
			m_nrOfStreetLights = 5;
			m_nrOfTrees = 5;

			m_trackName = "MonacoF1";
		}

		size_t m_nrOfStreetLights = 0;
		size_t m_nrOfTrees = 0;

		std::string m_trackName;
	};

	struct GraphicsParameters
	{
		void InitiGraphicsParameters()
		{
			m_increasedFieldTextureDensity = true;
			m_useLighhting = true;
		}

		bool m_increasedFieldTextureDensity = false;
		bool m_useLighhting = false;
	};

public:
	GameMode m_gameMode;
	bool m_isMultiplayer;
	size_t m_nrOfPlayers;

	glm::ivec2 m_resolution;

	bool m_frameTimerEnabled;

	PhysicsParameters m_physicsParameters;
	CarParameters m_carParameters;
	WorldParameters m_worldParameters;
	GraphicsParameters m_graphicsParameters;
};