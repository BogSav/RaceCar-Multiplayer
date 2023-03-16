#include "GeometryRenderer.hpp"

#include "3dGeometries/3dGeometryObject.hpp"
#include "amGame/Lighting/LightSource.hpp"

void GeometryRenderer::Render(const GeometryObject3d* geometryObject, const glm::mat4& modelMatrix)
{
	SendDataToShader(geometryObject, modelMatrix);

	glUniform1i(glGetUniformLocation(geometryObject->m_shader->program, "textureActive"), 0);

	geometryObject->m_mesh->Render();
}

void GeometryRenderer::Render(
	const GeometryObject3d* geometryObject,
	const glm::mat4& modelMatrix,
	const glm::vec3& eyePosition,
	const std::vector<const class LightSourceAdapter*>& lightingComponents)
{
	SendDataToShader(geometryObject, modelMatrix);

	SendLightingDataToShader(geometryObject, eyePosition, lightingComponents);

	geometryObject->m_mesh->Render();
}

void GeometryRenderer::Render(
	const GeometryObject3d* geometryObject,
	const glm::mat4& modelMatrix,
	const glm::vec3& eyePosition,
	const std::vector<const class LightSourceAdapter*>& lightingComponents,
	const Texture2D* texture)
{
	SendDataToShader(geometryObject, modelMatrix);

	SendLightingDataToShader(geometryObject, eyePosition, lightingComponents);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
	glUniform1i(glGetUniformLocation(geometryObject->m_shader->program, "texture_1"), 0);

	glUniform1i(glGetUniformLocation(geometryObject->m_shader->program, "textureActive"), 1);
	glUniform1i(glGetUniformLocation(geometryObject->m_shader->program, "ciclareActiva"), 0);

	glBindVertexArray(geometryObject->m_mesh->GetBuffers()->m_VAO);
	glDrawElements(
		geometryObject->m_mesh->GetDrawMode(),
		static_cast<int>(geometryObject->m_mesh->indices.size()),
		GL_UNSIGNED_INT,
		0);
}

void GeometryRenderer::Render(
	const GeometryObject3d* geometryObject,
	const glm::mat4& modelMatrix,
	const glm::vec3& eyePosition,
	const std::vector<const class LightSourceAdapter*>& lightingComponents,
	const Texture2D* texture,
	glm::vec2 texturePosition)
{
}

void GeometryRenderer::Render(
	const GeometryObject3d* geometryObject, const glm::mat4& modelMatrix, const Color& color)
{
	SendDataToShader(geometryObject, modelMatrix);

	int location = glGetUniformLocation(geometryObject->m_shader->program, "UniformColor");
	glUniform3fv(location, 1, glm::value_ptr(color()));

	geometryObject->m_mesh->Render();
}

void GeometryRenderer::Render(
	const GeometryObject3d* geometryObject, const glm::mat4& modelMatrix, const Texture2D* texture)
{
	SendDataToShader(geometryObject, modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
	glUniform1i(glGetUniformLocation(geometryObject->m_shader->program, "texture_1"), 0);

	glUniform1i(glGetUniformLocation(geometryObject->m_shader->program, "textureActive"), 1);
	glUniform1i(glGetUniformLocation(geometryObject->m_shader->program, "ciclareActiva"), 0);

	glBindVertexArray(geometryObject->m_mesh->GetBuffers()->m_VAO);
	glDrawElements(
		geometryObject->m_mesh->GetDrawMode(),
		static_cast<int>(geometryObject->m_mesh->indices.size()),
		GL_UNSIGNED_INT,
		0);
}

void GeometryRenderer::Render(
	const GeometryObject3d* geometryObject,
	const glm::mat4& modelMatrix,
	const Texture2D* texture,
	glm::vec2 texturePosition)
{
	SendDataToShader(geometryObject, modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
	glUniform1i(glGetUniformLocation(geometryObject->m_shader->program, "texture_1"), 0);

	glUniform2fv(
		glGetUniformLocation(geometryObject->m_shader->program, "ciclare"),
		1,
		glm::value_ptr(texturePosition));

	glUniform1i(glGetUniformLocation(geometryObject->m_shader->program, "textureActive"), 1);
	glUniform1i(glGetUniformLocation(geometryObject->m_shader->program, "ciclareActiva"), 1);

	glBindVertexArray(geometryObject->m_mesh->GetBuffers()->m_VAO);
	glDrawElements(
		geometryObject->m_mesh->GetDrawMode(),
		static_cast<int>(geometryObject->m_mesh->indices.size()),
		GL_UNSIGNED_INT,
		0);
}


void GeometryRenderer::Render(
	const GeometryObject3d* geometryObject,
	const glm::mat4& modelMatrix,
	const Shader* const shader,
	const CustomCamera* const camera)
{
	if (!geometryObject->m_mesh || !shader || !shader->program)
		return;

	shader->Use();

	glUniformMatrix4fv(
		shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	glUniformMatrix4fv(
		shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	geometryObject->m_mesh->Render();
}

void GeometryRenderer::Render(
	const GeometryObject3d* geometryObject,
	const glm::mat4& modelMatrix,
	const Shader* const shader,
	const CustomCamera* const camera,
	const Color color)
{
	if (!geometryObject->m_mesh || !shader || !shader->program)
		return;

	shader->Use();

	glUniformMatrix4fv(
		shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	glUniformMatrix4fv(
		shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int location = glGetUniformLocation(shader->program, "UniformColor");
	glUniform3fv(location, 1, glm::value_ptr(color()));

	geometryObject->m_mesh->Render();
}

void GeometryRenderer::SendDataToShader(
	const GeometryObject3d* geometryObject, const glm::mat4& modelMatrix)
{
	assert(geometryObject->m_shader != nullptr);
	assert(geometryObject->m_camera != nullptr);

	if (!geometryObject->m_mesh || !geometryObject->m_shader || !geometryObject->m_shader->program)
		return;

	geometryObject->m_shader->Use();

	glUniformMatrix4fv(
		geometryObject->m_shader->loc_view_matrix,
		1,
		GL_FALSE,
		glm::value_ptr(geometryObject->m_camera->GetViewMatrix()));

	glUniformMatrix4fv(
		geometryObject->m_shader->loc_projection_matrix,
		1,
		GL_FALSE,
		glm::value_ptr(geometryObject->m_camera->GetProjectionMatrix()));

	glUniformMatrix4fv(
		geometryObject->m_shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void GeometryRenderer::SendLightingDataToShader(
	const GeometryObject3d* geometryObject,
	const glm::vec3& eyePosition,
	const std::vector<const LightSourceAdapter*>& lightingComponents)
{
	// Send the position of the camera
	int location = glGetUniformLocation(geometryObject->m_shader->program, "eye_position");
	glUniform3fv(location, 1, glm::value_ptr(eyePosition));

	// Send material properties
	location = glGetUniformLocation(geometryObject->m_shader->program, "material_kd");
	glUniform1f(location, LightSourceAdapter::materialKd);
	location = glGetUniformLocation(geometryObject->m_shader->program, "material_ks");
	glUniform1f(location, LightSourceAdapter::materialKs);
	location = glGetUniformLocation(geometryObject->m_shader->program, "material_shininess");
	glUniform1i(location, LightSourceAdapter::materialShiness);

	// Send the color of the object
	location = glGetUniformLocation(geometryObject->m_shader->program, "object_color");
	glUniform3fv(location, 1, glm::value_ptr(geometryObject->m_color()));

	// Send the nr of light sources
	location = glGetUniformLocation(geometryObject->m_shader->program, "nrOfLightSources");
	glUniform1i(location, static_cast<int>(lightingComponents.size()));

	// Send the light sources data
	int i = 0;
	std::for_each(
		lightingComponents.begin(),
		lightingComponents.end(),
		[&geometryObject = std::as_const(geometryObject), &i, &location](
			const LightSourceAdapter* source)
		{
			glUniform1i(geometryObject->m_shader->ltype[i], source->GetLightTypeInt());
			glUniform3fv(
				geometryObject->m_shader->lposition[i], 1, glm::value_ptr(source->GetPosition()));
			glUniform3fv(
				geometryObject->m_shader->lcolor[i], 1, glm::value_ptr(source->GetLightColor()()));
			glUniform1f(geometryObject->m_shader->lintensity[i], source->GetLightIntensity());

			if (source->GetLightType() == LightSourceAdapter::SPOT)
			{
				const Spot& spotSource = dynamic_cast<const Spot&>(*source);

				glUniform3fv(
					geometryObject->m_shader->ldirection[i],
					1,
					glm::value_ptr(spotSource.GetDirection()));
				glUniform1f(geometryObject->m_shader->lcutoff[i], spotSource.GetCutOffAngle());
			}

			if (source->GetLightType() == LightSourceAdapter::DIRECTIONAL)
			{
				const Directional& directionalSource = dynamic_cast<const Directional&>(*source);

				glUniform3fv(
					geometryObject->m_shader->ldirection[i],
					1,
					glm::value_ptr(directionalSource.GetDirection()));
			}

			i++;
		});
}