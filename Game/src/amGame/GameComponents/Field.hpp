#pragma once

#include "GameComponent.hpp"

class Field : public GameComponent
{
public:
	Field() = delete;
	Field(const Shader* const shader, CustomCamera* const camera)
		: GameComponent()
	{
		if (Engine::GetGameSettings()->GetFieldParameters().increaseTextureDensity)
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
			GeometryRenderer::Render(
				geometry.get(), m_modelMatrix, eyePosition, lightingComponents);
	}

private:
	void IncreaseTextureDensity(const Shader* const shader, CustomCamera* const camera)
	{
		const float zstep = Engine::GetGameSettings()->GetFieldParameters().length
			/ static_cast<const float>(
								Engine::GetGameSettings()->GetFieldParameters().ozTriangleDensity);
		const float xstep = Engine::GetGameSettings()->GetFieldParameters().width
			/ static_cast<const float>(
								Engine::GetGameSettings()->GetFieldParameters().oxTrianglreDensity);

		for (float currentZstep = 0;
			 currentZstep < Engine::GetGameSettings()->GetFieldParameters().length;
			 currentZstep += zstep)
		{
			for (float currentXstep = 0;
				 currentXstep < Engine::GetGameSettings()->GetFieldParameters().width;
				 currentXstep += xstep)
			{
				m_geometries.emplace_back(new Polygon3d(
					shader,
					camera,
					Engine::GetGameSettings()->GetFieldParameters().startPos
						+ glm::vec3{currentXstep + xstep, -0.1, currentZstep},
					Engine::GetGameSettings()->GetFieldParameters().startPos
						+ glm::vec3{currentXstep, -0.1, currentZstep},
					Engine::GetGameSettings()->GetFieldParameters().startPos
						+ glm::vec3{currentXstep, -0.1, currentZstep + zstep},
					Engine::GetGameSettings()->GetFieldParameters().startPos
						+ glm::vec3{currentXstep + xstep, -0.1, currentZstep + zstep},
					Engine::GetGameSettings()->GetFieldParameters().color));
			}
		}
	}


private:
	std::vector<std::unique_ptr<GeometryObject3d>> m_geometries;
};