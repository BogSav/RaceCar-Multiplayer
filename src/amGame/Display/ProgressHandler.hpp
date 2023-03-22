#pragma once

#include "amGame/GameSettings.hpp"
#include "amGeometry/2dGeometries/2dGeometriesIncludes.hpp"
#include "amUI/TextComponents/TextEngine.hpp"

class ProgressHandler
{
public:
	ProgressHandler() = delete;
	ProgressHandler(const GameSettings* gameSettings, std::shared_ptr<TextEngine>& textEngine)
		: m_gameSettings(gameSettings),
		  m_currentHeightScale(1.f),
		  m_progress(0.f)
	{
		m_border = std::make_unique<Polygon2d>(
			gameSettings->GetInGameDisplayParameters().progress_position,
			gameSettings->GetInGameDisplayParameters().progress_width,
			gameSettings->GetInGameDisplayParameters().progress_height,
			gameSettings->GetInGameDisplayParameters().progress_borderColor,
			true);
		m_handlerBar = std::make_unique<Polygon2d>(
			gameSettings->GetInGameDisplayParameters().progress_position,
			gameSettings->GetInGameDisplayParameters().progress_width,
			10.f,
			Colors::LightGray);

		m_textEngine = textEngine;
	}

	// Set progress in percent
	void SetProgress(float progress)
	{
		m_progress = progress;
		this->UpdateHightScale();
	}

	void Render(
		const Shader* const shader,
		const glm::mat3& transformMatrix,
		const glm::mat3& modelMatrix) const
	{
		// Render the border of the progress handler
		m_border->Render(shader, transformMatrix, glm::mat3(1));

		// Render the fill of the progress handler
		m_handlerBar->Render(
			shader,
			transformMatrix,
			modelMatrix
				* utils::Translate2d(
					0.f,
					-(m_gameSettings->GetInGameDisplayParameters().progress_position.y
					  * (m_currentHeightScale - 1.f)))
				* utils::Scale2d(1.f, m_currentHeightScale));

		// Render the associated text
		m_textEngine->Render(
			"Start",
			m_gameSettings->GetInGameDisplayParameters().progress_position.x - 2.5f,
			m_gameSettings->GetInGameDisplayParameters().progress_position.y + 35,
			1.0f,
			Colors::LightGray);
		m_textEngine->Render(
			"Finish",
			m_gameSettings->GetInGameDisplayParameters().progress_position.x - 10.f,
			m_gameSettings->GetInGameDisplayParameters().progress_position.y
				- m_gameSettings->GetInGameDisplayParameters().progress_height - 5.f,
			1.0f,
			Colors::LightGray);
	}

private:
	void UpdateHightScale()
	{
		m_currentHeightScale = m_progress / 100
			* (m_gameSettings->GetInGameDisplayParameters().progress_height / 10.f);
	}

private:
	std::unique_ptr<GeometryObject2d> m_border;
	std::unique_ptr<GeometryObject2d> m_handlerBar;

	std::shared_ptr<TextEngine> m_textEngine;

	const GameSettings* m_gameSettings;

	float m_progress;
	float m_currentHeightScale;
};