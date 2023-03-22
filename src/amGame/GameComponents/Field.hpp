#pragma once

#include "GameComponent.hpp"

class Field : public GameComponent
{
public:
	Field() = delete;
	Field(const GameSettings* gameSettings, const Shader* const shader, CustomCamera* const camera)
		: GameComponent(gameSettings)
	{
		if (gameSettings->GetFieldParameters().increaseTextureDensity)
		{
			this->IncreaseTextureDensity(shader, camera);
		}
		else
		{
			// TO DO
		}
	}

	void Render() override
	{
		for (auto& geometry : m_geometries)
			GeometryRenderer::Render(geometry.get(), m_modelMatrix);
	}

	void Render(const glm::vec3& eyePosition, const LightSourcesVector& lightingComponents) override
	{
		for (auto& geometry : m_geometries)
			GeometryRenderer::Render(geometry.get(), m_modelMatrix, eyePosition, lightingComponents);
	}

private:
	void IncreaseTextureDensity(const Shader* const shader, CustomCamera* const camera)
	{
		const float zstep = m_gameSettings->GetFieldParameters().length
			/ static_cast<const float>(m_gameSettings->GetFieldParameters().ozTriangleDensity);
		const float xstep = m_gameSettings->GetFieldParameters().width
			/ static_cast<const float>(m_gameSettings->GetFieldParameters().oxTrianglreDensity);

		for (float currentZstep = 0; currentZstep < m_gameSettings->GetFieldParameters().length;
			 currentZstep += zstep)
		{
			for (float currentXstep = 0; currentXstep < m_gameSettings->GetFieldParameters().width;
				 currentXstep += xstep)
			{
				m_geometries.emplace_back(new Polygon3d(
					shader,
					camera,
					m_gameSettings->GetFieldParameters().startPos
						+ glm::vec3{currentXstep + xstep, -0.1, currentZstep},
					m_gameSettings->GetFieldParameters().startPos
						+ glm::vec3{currentXstep, -0.1, currentZstep},
					m_gameSettings->GetFieldParameters().startPos
						+ glm::vec3{currentXstep, -0.1, currentZstep + zstep},
					m_gameSettings->GetFieldParameters().startPos
						+ glm::vec3{currentXstep + xstep, -0.1, currentZstep + zstep},
					m_gameSettings->GetFieldParameters().color));
			}
		}
	}


private:
	std::vector<std::unique_ptr<GeometryObject3d>> m_geometries;
};