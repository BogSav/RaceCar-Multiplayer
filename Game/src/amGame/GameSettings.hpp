#pragma once

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
			m_nrOfStreetLights = 15;
			m_nrOfTrees = 10;

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

		float m_width = 0.f;
		float m_scaleFactor = 0.f;
	};

	struct FieldParameters
	{
		void InitParameters()
		{
			startPos = glm::vec3{-480.f, 0.f, -350.f};
			width = 930.f;
			length = 600.f;
			ozTriangleDensity = 20;
			oxTrianglreDensity = 20;
			increaseTextureDensity = true;
			color = Colors::DarkGreen;
		}

		glm::vec3 startPos;
		float width = 0.f;
		float length = 0.f;
		size_t ozTriangleDensity = 0;
		size_t oxTrianglreDensity = 0;
		bool increaseTextureDensity = false;
		Color color;
	};

	struct InGameDisplayParameters
	{
		void InitParams()
		{
			tur_indicatorColor = Colors::White;
			tur_numberingColor = Colors::White;
			tur_backgroundColor = Colors::DarkGray;
			tur_fontName = "Algerian.ttf";

			progress_position = glm::vec3{1150.f, 350.f, 0.f};
			progress_height = 300.f;
			progress_width = 50.f;
			progress_fillColor = Colors::LightGray;
			progress_borderColor = Colors::LightGray;
		}

		// Turometer parameters
		Color tur_indicatorColor;
		Color tur_numberingColor;
		Color tur_backgroundColor;
		std::string tur_fontName;

		// Progress handler parameters
		glm::vec3 progress_position;
		float progress_height;
		float progress_width;
		Color progress_fillColor;
		Color progress_borderColor;
	};

public:
	const PhysicsParameters& GetPhysicsParameters() const { return (m_physicsParameters); }
	const WorldParameters& GetWorldParameters() const { return (m_worldParameters); }
	const CarParameters& GetCarParameters() const { return (m_carParameters); }
	const TrackParaneters& GetTrackParameters() const { return (m_trackParemetrs); }
	const FieldParameters& GetFieldParameters() const { return m_fieldParameters; }
	const InGameDisplayParameters& GetInGameDisplayParameters() const
	{
		return m_inGameDisplayParameters;
	}

	void InitParameters()
	{
		m_physicsParameters.InitParameters();
		m_worldParameters.InitParameters();

		m_carParameters.InitParameters();
		m_trackParemetrs.InitParameters();
		m_fieldParameters.InitParameters();
		m_inGameDisplayParameters.InitParams();
	}

public:
	GameMode m_gameMode = GameMode::UNSELECTED;
	bool m_isMultiplayer;
	size_t m_nrOfPlayers = 0;

	glm::ivec2 m_resolution;

	bool m_frameTimerEnabled;
	bool m_forceFixedFramerate;
	float m_nrOfFramesPerSecondForFixedFramerate;

private:
	PhysicsParameters m_physicsParameters;
	WorldParameters m_worldParameters;

	// Game components parameters
	CarParameters m_carParameters;
	TrackParaneters m_trackParemetrs;
	FieldParameters m_fieldParameters;
	InGameDisplayParameters m_inGameDisplayParameters;
};