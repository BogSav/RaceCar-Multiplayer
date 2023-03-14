//#include "3dGeometryObject.hpp"
//
//#include <iostream>
//#include <string>
//
//GeometryObject3d::GeometryObject3d(
//	const Shader* const shader, CustomCamera* const camera, Color color)
//	: m_shader(shader), m_camera(camera), m_color(color)
//{
//}
//
//GeometryObject3d::GeometryObject3d(
//	const Shader* const shader,
//	CustomCamera* const camera,
//	std::string meshPath,
//	std::string meshName)
//	: m_shader(shader), m_camera(camera)
//{
//	m_mesh = std::make_unique<Mesh>();
//	m_mesh->LoadMesh(meshPath, meshName);
//}
//
//void GeometryObject3d::Render(const glm::mat4& modelMatrix) const
//{
//	this->SendDataToShader(modelMatrix);
//
//	glUniform1i(glGetUniformLocation(m_shader->program, "textureActive"), 0);
//
//	m_mesh->Render();
//}
//
//void GeometryObject3d::Render(
//	const glm::mat4& modelMatrix,
//	const glm::vec3& eyePosition,
//	const std::vector<const class LightSourceAdapter*>& lightingComponents) const
//{
//	this->SendDataToShader(modelMatrix);
//
//	SendLightingDataToShader(eyePosition, lightingComponents);
//
//	m_mesh->Render();
//}
//
//void GeometryObject3d::Render(
//	const glm::mat4& modelMatrix,
//	const Texture2D* texture,
//	const glm::vec3& eyePosition,
//	const std::vector<const class LightSourceAdapter*>& lightingComponents) const
//{
//	this->SendDataToShader(modelMatrix);
//
//	SendLightingDataToShader(eyePosition, lightingComponents);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
//	glUniform1i(glGetUniformLocation(m_shader->program, "texture_1"), 0);
//
//	glUniform1i(glGetUniformLocation(m_shader->program, "textureActive"), 1);
//	glUniform1i(glGetUniformLocation(m_shader->program, "ciclareActiva"), 0);
//
//	glBindVertexArray(m_mesh->GetBuffers()->m_VAO);
//	glDrawElements(
//		m_mesh->GetDrawMode(), static_cast<int>(m_mesh->indices.size()), GL_UNSIGNED_INT, 0);
//}
//
//void GeometryObject3d::Render(
//	const glm::mat4& modelMatrix,
//	const Texture2D* texture,
//	glm::vec2 texturePosition,
//	const glm::vec3& eyePosition,
//	const std::vector<const class LightSourceAdapter*>& lightingComponents) const
//{
//}
//
//void GeometryObject3d::Render(const glm::mat4& modelMatrix, const Color& color) const
//{
//	this->SendDataToShader(modelMatrix);
//
//	int location = glGetUniformLocation(m_shader->program, "UniformColor");
//	glUniform3fv(location, 1, glm::value_ptr(color()));
//
//	m_mesh->Render();
//}
//
//void GeometryObject3d::Render(const glm::mat4& modelMatrix, const Texture2D* texture)
//{
//	SendDataToShader(modelMatrix);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
//	glUniform1i(glGetUniformLocation(m_shader->program, "texture_1"), 0);
//
//	glUniform1i(glGetUniformLocation(m_shader->program, "textureActive"), 1);
//	glUniform1i(glGetUniformLocation(m_shader->program, "ciclareActiva"), 0);
//
//	glBindVertexArray(m_mesh->GetBuffers()->m_VAO);
//	glDrawElements(
//		m_mesh->GetDrawMode(), static_cast<int>(m_mesh->indices.size()), GL_UNSIGNED_INT, 0);
//}
//
//void GeometryObject3d::Render(
//	const glm::mat4& modelMatrix, const Texture2D* texture, glm::vec2 texturePosition)
//{
//	SendDataToShader(modelMatrix);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
//	glUniform1i(glGetUniformLocation(m_shader->program, "texture_1"), 0);
//
//	glUniform2fv(
//		glGetUniformLocation(m_shader->program, "ciclare"), 1, glm::value_ptr(texturePosition));
//
//	glUniform1i(glGetUniformLocation(m_shader->program, "textureActive"), 1);
//	glUniform1i(glGetUniformLocation(m_shader->program, "ciclareActiva"), 1);
//
//	glBindVertexArray(m_mesh->GetBuffers()->m_VAO);
//	glDrawElements(
//		m_mesh->GetDrawMode(), static_cast<int>(m_mesh->indices.size()), GL_UNSIGNED_INT, 0);
//}
//
//
//void GeometryObject3d::Render(
//	const Shader* const shader,
//	const CustomCamera* const camera,
//	const glm::mat4& modelMatrix) const
//{
//	if (!m_mesh || !shader || !shader->program)
//		return;
//
//	shader->Use();
//
//	glUniformMatrix4fv(
//		shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
//
//	glUniformMatrix4fv(
//		shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
//
//	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
//
//	m_mesh->Render();
//}
//
//void GeometryObject3d::Render(
//	const Shader* const shader,
//	const CustomCamera* const camera,
//	const glm::mat4& modelMatrix,
//	const Color color) const
//{
//	if (!m_mesh || !shader || !shader->program)
//		return;
//
//	shader->Use();
//
//	glUniformMatrix4fv(
//		shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
//
//	glUniformMatrix4fv(
//		shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
//
//	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
//
//	int location = glGetUniformLocation(shader->program, "UniformColor");
//	glUniform3fv(location, 1, glm::value_ptr(color()));
//
//	m_mesh->Render();
//}
//
//void GeometryObject3d::SendDataToShader(const glm::mat4& modelMatrix) const
//{
//	assert(m_shader != nullptr);
//	assert(m_camera != nullptr);
//
//	if (!m_mesh || !m_shader || !m_shader->program)
//		return;
//
//	m_shader->Use();
//
//	glUniformMatrix4fv(
//		m_shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetViewMatrix()));
//
//	glUniformMatrix4fv(
//		m_shader->loc_projection_matrix,
//		1,
//		GL_FALSE,
//		glm::value_ptr(m_camera->GetProjectionMatrix()));
//
//	glUniformMatrix4fv(m_shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
//}
//
//void GeometryObject3d::SendLightingDataToShader(
//	const glm::vec3& eyePosition,
//	const std::vector<const LightSourceAdapter*>& lightingComponents) const
//{
//	// Send the position of the camera
//	int location = glGetUniformLocation(m_shader->program, "eye_position");
//	glUniform3fv(location, 1, glm::value_ptr(eyePosition));
//
//	// Send material properties
//	location = glGetUniformLocation(m_shader->program, "material_kd");
//	glUniform1f(location, LightSourceAdapter::materialKd);
//	location = glGetUniformLocation(m_shader->program, "material_ks");
//	glUniform1f(location, LightSourceAdapter::materialKs);
//	location = glGetUniformLocation(m_shader->program, "material_shininess");
//	glUniform1i(location, LightSourceAdapter::materialShiness);
//
//	// Send the color of the object
//	location = glGetUniformLocation(m_shader->program, "object_color");
//	glUniform3fv(location, 1, glm::value_ptr(m_color()));
//
//	// Send the nr of light sources
//	location = glGetUniformLocation(m_shader->program, "nrOfLightSources");
//	glUniform1i(location, static_cast<int>(lightingComponents.size()));
//
//	// Send the light sources data
//	int i = 0;
//	std::for_each(
//		lightingComponents.begin(),
//		lightingComponents.end(),
//		[this, &i, &location](const LightSourceAdapter* source)
//		{
//			glUniform1i(m_shader->ltype[i], source->GetLightTypeInt());
//			glUniform3fv(m_shader->lposition[i], 1, glm::value_ptr(source->GetPosition()));
//			glUniform3fv(m_shader->lcolor[i], 1, glm::value_ptr(source->GetLightColor()()));
//			glUniform1f(m_shader->lintensity[i], source->GetLightIntensity());
//
//			if (source->GetLightType() == LightSourceAdapter::SPOT)
//			{
//				const Spot& spotSource = dynamic_cast<const Spot&>(*source);
//
//				glUniform3fv(m_shader->ldirection[i], 1, glm::value_ptr(spotSource.GetDirection()));
//				glUniform1f(m_shader->lcutoff[i], spotSource.GetCutOffAngle());
//			}
//
//			if (source->GetLightType() == LightSourceAdapter::DIRECTIONAL)
//			{
//				const Directional& directionalSource = dynamic_cast<const Directional&>(*source);
//
//				glUniform3fv(
//					m_shader->ldirection[i], 1, glm::value_ptr(directionalSource.GetDirection()));
//			}
//
//			i++;
//		});
//}
//
//void GeometryObject3d::CalculateVerticesNormals(
//	std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
//{
//	for (size_t i = 0; i < indices.size() - 2; i += 3)
//	{
//		int vertexA = indices[i];
//		int vertexB = indices[i + 1];
//		int vertexC = indices[i + 2];
//
//		glm::vec3 edgeAB = vertices[vertexB].position - vertices[vertexA].position;
//		glm::vec3 edgeAC = vertices[vertexC].position - vertices[vertexA].position;
//
//		glm::vec3 areaWeightedNormal = glm::cross(edgeAB, edgeAC);
//
//		vertices[vertexA].normal += areaWeightedNormal;
//		vertices[vertexB].normal += areaWeightedNormal;
//		vertices[vertexC].normal += areaWeightedNormal;
//	}
//
//	std::for_each(
//		vertices.begin(),
//		vertices.end(),
//		[](VertexFormat& current) { current.normal = glm::normalize(current.normal); });
//}