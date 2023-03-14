#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "amUtilities/Camera.hpp"
#include "amUtilities/Colors.hpp"

class GeometryObject3d
{
public:
	GeometryObject3d() = default;
	GeometryObject3d(const Shader* const shader, CustomCamera* const camera, Color color);
	GeometryObject3d(
		const Shader* const shader,
		CustomCamera* const camera,
		std::string meshPath,
		std::string meshName);

	// Simple render
	void Render(const glm::mat4& modelMatrix) const;

	// Render with lights
	void Render(
		const glm::mat4& modelMatrix,
		const glm::vec3& eyePosition,
		const std::vector<const class LightSourceAdapter*>& lightingComponents) const;
	void Render(
		const glm::mat4& modelMatrix,
		const Texture2D* texture,
		const glm::vec3& eyePosition,
		const std::vector<const class LightSourceAdapter*>& lightingComponents) const;
	void Render(
		const glm::mat4& modelMatrix,
		const Texture2D* texture,
		glm::vec2 texturePosition,
		const glm::vec3& eyePosition,
		const std::vector<const class LightSourceAdapter*>& lightingComponents) const;

	// Rnadare cu culoare fortata
	void Render(const glm::mat4& modelMatrix, const Color& color) const;

	// Randare cu texture simpla si cu ciclare
	void Render(const glm::mat4& modelMatrix, const Texture2D* texture);
	void Render(const glm::mat4& modelMatrix, const Texture2D* texture, glm::vec2 texturePosition);

	// Randare cu shader si camera custom
	void Render(
		const Shader* const shader,
		const CustomCamera* const camera,
		const glm::mat4& modelMatrix) const;
	void Render(
		const Shader* const shader,
		const CustomCamera* const camera,
		const glm::mat4& modelMatrix,
		const Color color) const;

	// Bbox related functions
	void ComputeInitialBBox();

private:
	void SendDataToShader(const glm::mat4& modelMatrix) const;
	void SendLightingDataToShader(
		const glm::vec3& eyePosition,
		const std::vector<const LightSourceAdapter*>& lightingComponents) const;

protected:
	void CalculateVerticesNormals(
		std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);

protected:
	std::unique_ptr<Mesh> m_mesh = nullptr;
	Color m_color;

	const Shader* m_shader = nullptr;	
	CustomCamera* m_camera = nullptr;
};