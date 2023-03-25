#pragma once

#include "amConnectivity/Client.hpp"
#include "amGame/GameComponents/BaseCar.hpp"

class NPCCar : public BaseCar
{
public:
	NPCCar(
		const GameSettings* gameSettings,
		const Shader* const shader,
		std::shared_ptr<CustomCamera> camera,
		const Client& client)
		: BaseCar(gameSettings, shader, camera), m_client(client)
	{
		m_mesh = std::make_unique<Mesh>();
		m_mesh->LoadMesh(
			PATH_JOIN(Engine::GetWindow()->props.selfDir, RESOURCE_PATH::MODELS, "mele"), "F1.obj");

		m_position = {-10.f, -0.1f, -40.f};
		m_scale = {0.7f, 0.7f, 0.7f};
		m_direction = {1.f, 0.f, 0.f};
		m_angleOrientation = 0.f;
	}

	void Render() override { this->Render(m_camera.get(), m_shader); }

	void Render(CustomCamera* const camera, const Shader* const shader)
	{
		assert(shader != nullptr);
		assert(camera != nullptr);

		if (!m_mesh || !shader || !shader->program)
			return;

		shader->Use();

		glUniformMatrix4fv(
			shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

		glUniformMatrix4fv(
			shader->loc_projection_matrix,
			1,
			GL_FALSE,
			glm::value_ptr(camera->GetProjectionMatrix()));

		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

		m_mesh->Render();
	}

	void Update(float deltaTime) override
	{
		m_client.UpdatePositionAndDirection(m_position, m_angleOrientation);
		/*Engine::GetClient()->UpdatePositionAndDirection(m_position, m_direction);*/
		this->ComputeModelMatrix();
	}

private:

	void ComputeModelMatrix()
	{
		m_modelMatrix = glm::mat4(1);
		m_modelMatrix = glm::translate(m_modelMatrix, m_position);
		m_modelMatrix = glm::rotate(m_modelMatrix, m_angleOrientation, glm::vec3{0, 1, 0});
		m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
	}

private:
	std::unique_ptr<Mesh> m_mesh;
	const Client& m_client;
};