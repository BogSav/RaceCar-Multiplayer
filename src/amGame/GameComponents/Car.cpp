#include "Car.hpp"

#include <iostream>

Car::Car(const GameSettings* gameSettings, const WindowObject* window, const Shader* const shader)
	: BaseCar(gameSettings, shader, std::make_shared<CustomCamera>()),
	  m_distanceFromCamera(6.f),
	  m_stirringAngularSpeed(RADIANS(45.f))
{
	m_mesh = std::make_unique<Mesh>();
	m_mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "mele"), "F1.obj");

	m_position = {-10, -0.1, -40};
	m_scale = {0.7, 0.7, 0.7};
	m_direction = {1, 0, 0};
	m_angleOrientation = 0;

	this->UpdateOrientation(-1);

	m_camera->Set(
		m_position - m_direction * (m_distanceFromCamera * 2.f),
		m_position,
		glm::vec3{0, 1, 0},
		window->props.aspectRatio);
	m_camera->TranslateUpward(m_distanceFromCamera);
	m_camera->RotateFirstPerson_OX(RADIANS(-15));

	m_gearBox = std::make_unique<CarComponents::GearBox>(gameSettings->m_carParameters);
	m_engine = std::make_unique<CarComponents::CarEngine>(*gameSettings, m_gearBox.get());

	m_headLightLeft = std::make_unique<SpotLight>(
		glm::vec3(0, 0, 0), Colors::White, 15.0f, RADIANS(15), m_direction, glm::vec3(0, 0, 1));
	m_headLightRight = std::make_unique<SpotLight>(
		glm::vec3(0, 0, 0), Colors::White, 15.0f, RADIANS(15), m_direction, glm::vec3(0, 0, 1));

	this->ComputeModelMatrix();
}

void Car::Render()
{
	this->Render(m_camera.get(), m_shader);
}

void Car::Render(CustomCamera* const camera, const Shader* const shader) const
{
	assert(shader != nullptr);
	assert(camera != nullptr);

	if (!m_mesh || !shader || !shader->program)
		return;

	shader->Use();

	glUniformMatrix4fv(
		shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	glUniformMatrix4fv(
		shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

	m_mesh->Render();
}

void Car::Update(float deltaTime)
{
	glm::vec3 newPosition = m_engine->GetNewPosition(m_position, m_direction, deltaTime);
	m_camera->MoveForward(glm::l2Norm(newPosition - m_position));
	m_position = newPosition;

	this->ComputeModelMatrix();

	m_headLightLeft->SetDirection(m_direction + glm::vec3(0, -0.07, 0));
	m_headLightLeft->SetPosition(
		m_position + glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_direction)) * 1.2f
		+ glm::vec3(0, 1, 0) - m_direction * 0.2f);

	m_headLightRight->SetDirection(m_direction + glm::vec3(0, -0.07, 0));
	m_headLightRight->SetPosition(
		m_position + glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_direction)) * -1.2f
		+ glm::vec3(0, 1, 0) - m_direction * 0.2f);
}

void Car::Accelerate()
{
	m_engine->Accelerate();
}

void Car::Brake()
{
	m_engine->Brake();
}

void Car::InertialDecceleration()
{
	m_engine->InertialDeccelerate();
}

void Car::PrintData()
{
	std::cout << m_engine->GetSpeedKmh() << " " << m_engine->GetCurrentGear() << std::endl;
}

void Car::UpdateOrientation(float deltaTime)
{
	float angle = deltaTime * m_stirringAngularSpeed;

	m_angleOrientation += angle;

	m_direction =
		glm::normalize(glm::rotate(glm::mat4(1.0f), angle, {0, 1, 0}) * glm::vec4(m_direction, 0));

	m_camera->RotateThirdPerson_OY(angle);
}

void Car::ComputeModelMatrix()
{
	m_modelMatrix = glm::mat4(1);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::rotate(m_modelMatrix, m_angleOrientation, glm::vec3{0, 1, 0});
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
}
