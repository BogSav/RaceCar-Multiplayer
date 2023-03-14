#pragma once

#include "GameComponent.hpp"

class Field : public GameComponent
{
public:
	Field() = delete;
	Field(
		const Shader* const shader,
		CustomCamera* const camera,
		const glm::vec3& startPos,
		const float& width,
		const float& length,
		bool increseTextureDensity = true,
		const size_t ozTriangleDensity = 20,
		const size_t oxTrianglreDensity = 20)
		: GameComponent(shader, camera),
		  m_startPosition(startPos),
		  m_width(width),
		  m_length(length),
		  m_OZTrianglesDensity(ozTriangleDensity),
		  m_OXTrianglesDensity(oxTrianglreDensity)
	{
		if (increseTextureDensity)
		{
			this->IncreaseTextureDensity();
		}
	}

	const glm::vec3& Getposition() const { return m_startPosition; }
	float GetWidth() const { return m_width; }
	float GetLength() const { return m_length; }

private:
	void IncreaseTextureDensity()
	{
		const float zstep = m_length / static_cast<const float>(m_OZTrianglesDensity);
		const float xstep = m_width / static_cast<const float>(m_OXTrianglesDensity);

		for (float currentZstep = 0; currentZstep < m_length; currentZstep += zstep)
		{
			for (float currentXstep = 0; currentXstep < m_width; currentXstep += xstep)
			{
				m_geometries.emplace_back(new Polygon3d(
					m_shader,
					m_camera,
					m_startPosition + glm::vec3{currentXstep + xstep, -0.1, currentZstep},
					m_startPosition + glm::vec3{currentXstep, -0.1, currentZstep},
					m_startPosition + glm::vec3{currentXstep, -0.1, currentZstep + zstep},
					m_startPosition + glm::vec3{currentXstep + xstep, -0.1, currentZstep + zstep},
					Colors::DarkGreen));
			}
		}
	}


private:
	const glm::vec3 m_startPosition;
	const float m_width;
	const float m_length;

	const size_t m_OZTrianglesDensity;
	const size_t m_OXTrianglesDensity;
};