#pragma once

#include "amUtilities/Camera.hpp"
#include "amUtilities/Colors.hpp"
#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

class GeometryRenderer
{
public:
	// Randare simpla
	static void Render(const class GeometryObject3d* geometryObject, const glm::mat4& modelMatrix);
	static void Render(
		const class GeometryObject3d* geometryObject,
		const glm::mat4& modelMatrix,
		const Color& color);

	// Randare fara lumini si cu texturi
	static void Render(
		const class GeometryObject3d* geometryObject,
		const glm::mat4& modelMatrix,
		const Texture2D* texture);
	static void Render(
		const class GeometryObject3d* geometryObject,
		const glm::mat4& modelMatrix,
		const Texture2D* texture,
		glm::vec2 texturePosition);

	// Render cu lumini si cu/fara texturi
	static void Render(
		const class GeometryObject3d* geometryObject,
		const glm::mat4& modelMatrix,
		const glm::vec3& eyePosition,
		const class LightSourcesVector& lightingComponents);
	static void Render(
		const class GeometryObject3d* geometryObject,
		const glm::mat4& modelMatrix,
		const glm::vec3& eyePosition,
		const class LightSourcesVector& lightingComponents,
		const Texture2D* texture);
	static void Render(
		const class GeometryObject3d* geometryObject,
		const glm::mat4& modelMatrix,
		const glm::vec3& eyePosition,
		const class LightSourcesVector& lightingComponents,
		const Texture2D* texture,
		glm::vec2 texturePosition);

	// Randare cu shader custom si camera custom
	static void Render(
		const class GeometryObject3d* geometryObject,
		const glm::mat4& modelMatrix,
		const Shader* const shader,
		const CustomCamera* const camera);
	static void Render(
		const class GeometryObject3d* geometryObject,
		const glm::mat4& modelMatrix,
		const Shader* const shader,
		const CustomCamera* const camera,
		const Color color);

private:
	static void SendDataToShader(
		const GeometryObject3d* geometryObject, const glm::mat4& modelMatrix);
	static void SendLightingDataToShader(
		const class GeometryObject3d* geometryObject,
		const glm::vec3& eyePosition,
		const class LightSourcesVector& lightingComponents);
};