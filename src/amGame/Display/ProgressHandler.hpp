#pragma once

#include "MainMenu/2dGeometries/2dPolygon.hpp"
#include "MainMenu/GameComponents/Car.hpp"

class ProgressHandler
{
public:
	ProgressHandler() = delete;
	ProgressHandler(const glm::vec3& position, const float& height, const float width)
		: m_height(height),
		  m_width(width),
		  m_position(position),
		  m_currentHeightScale(1.f),
		  m_progress(0.f)
	{
		m_frame = std::make_unique<Polygon2d>(position, width, height, Colors::LightGray, true);
		m_handlerBar = std::make_unique<Polygon2d>(position, width, 10, Colors::LightGray);
	}

	// Set progress in percent
	void SetProgress(float progress)
	{
		m_progress = progress;
		this->Update();
	}

	void Render(
		const Shader* const shader,
		const glm::mat3& transformMatrix,
		const glm::mat3& modelMatrix) const
	{
		m_frame->Render(shader, transformMatrix, glm::mat3(1));
		m_handlerBar->Render(
			shader,
			transformMatrix,
			modelMatrix * utils::Translate2d(0.f, -(m_position.y * (m_currentHeightScale - 1.f)))
				* utils::Scale2d(1.f, m_currentHeightScale));
	}

	const glm::vec3& GetPosition() { return m_position; }
	const float& GetHeight() { return m_height; }

private:
	void Update() { m_currentHeightScale = m_progress / 100 * (m_height / 10.f); }

private:
	const glm::vec3 m_position;
	float m_height;
	float m_width;

	float m_progress;
	float m_currentHeightScale;

	std::unique_ptr<GeometryObject2d> m_frame;
	std::unique_ptr<GeometryObject2d> m_handlerBar;
};