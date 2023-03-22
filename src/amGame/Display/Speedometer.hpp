#pragma once

#include "amGame/GameSettings.hpp"
#include "amGeometry/2dGeometries/2dGeometriesIncludes.hpp"
#include "amUI/TextComponents/TextEngine.hpp"
#include "amUtilities/Utilities.hpp"

#include <string>

class Speedometer
{
public:
	Speedometer() = delete;
	Speedometer(const GameSettings* gameSettings, std::shared_ptr<TextEngine>& textEngine)
		: m_gameSettings(gameSettings)
	{
		m_background = std::make_unique<SemiCircle2d>(
			glm::vec2{175.f, 0.f},
			175.f,
			16.f,
			m_gameSettings->GetInGameDisplayParameters().tur_backgroundColor);
		m_indicator = std::make_unique<Polygon2d>(
			glm::vec2{-5.f, 0.f},
			7.f,
			74.f,
			m_gameSettings->GetInGameDisplayParameters().tur_indicatorColor);

		m_textEngine = textEngine;
	}

	void Render(
		const Shader* const shader,
		const glm::mat3& transformMatrix,
		const glm::mat3& modelMatrix) const
	{
		// Render the background of the speedometer
		m_background->Render(shader, transformMatrix, modelMatrix);

		// Render number on board
		glClear(GL_DEPTH_BUFFER_BIT);
		m_textEngine->Render(
			"0",
			22.f,
			659.f,
			1.2f,
			m_gameSettings->GetInGameDisplayParameters().tur_numberingColor);
		m_textEngine->Render(
			"85",
			66.f,
			600.f,
			1.2f,
			m_gameSettings->GetInGameDisplayParameters().tur_numberingColor);
		m_textEngine->Render(
			"170",
			153.f,
			572.f,
			1.2f,
			m_gameSettings->GetInGameDisplayParameters().tur_numberingColor);
		m_textEngine->Render(
			"255",
			236.f,
			600.f,
			1.2f,
			m_gameSettings->GetInGameDisplayParameters().tur_numberingColor);
		m_textEngine->Render(
			"340",
			296.f,
			659.f,
			1.2f,
			m_gameSettings->GetInGameDisplayParameters().tur_numberingColor);

		// Render the indicator line
		m_indicator->Render(
			shader,
			transformMatrix,
			utils::Translate2d(175.f, 50.f) * utils::Rotate2d(GetAngleAccordingToSpeed(m_speed))
				* glm::mat3(1));

		// Render the speed
		m_textEngine->Render(
			std::to_string(static_cast<int>(std::round(m_speed))) + " km/h", 62.f, 695.f, 1.2f);

		// Render the current gear
		m_textEngine->Render(m_speed == 0 ? "Liber" : std::to_string(m_gear), 218.f, 695.f, 1.2f);
	}

	void SetSpeed(const float& speed) { m_speed = speed; }
	void SetGear(const int& gear) { m_gear = gear; }

private:
	float GetAngleAccordingToSpeed(float speed) const
	{
		if (speed > 170.f)
			return -((1.f / 170.f * speed - 1.f) * piOver2);
		else
			return piOver2 * (1.f - 1.f / 170.f * speed);
	}

private:
	std::unique_ptr<GeometryObject2d> m_indicator;
	std::unique_ptr<GeometryObject2d> m_background;

	std::shared_ptr<TextEngine> m_textEngine;

	const GameSettings* m_gameSettings;

	float m_speed = 0.f;
	int m_gear = 0;
};