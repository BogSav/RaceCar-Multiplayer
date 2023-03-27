#include "Car.hpp"

#include "PlaceTracker.hpp"

#include <iostream>

Car::Car(
	const Shader* const shader,
	std::shared_ptr<CustomCamera> camera,
	Client& client)
	: BaseCar(shader, camera, client),
	  m_distanceFromCamera(6.f),
	  m_stirringAngularSpeed(RADIANS(45.f))
{
	BaseCar::InitMesh();

	m_position = {-10.f, -0.1f, -40.f};
	m_scale = {0.7f, 0.7f, 0.7f};
	m_direction = {1.f, 0.f, 0.f};
	m_angleOrientation = 0.f;

	this->UpdateOrientation(-1.f);

	m_camera->Set(
		m_position - m_direction * (m_distanceFromCamera * 2.f),
		m_position,
		glm::vec3{0, 1, 0},
		Engine::GetWindow()->props.aspectRatio);
	m_camera->TranslateUpward(m_distanceFromCamera);
	m_camera->RotateFirstPerson_OX(RADIANS(-15));

	m_gearBox = std::make_unique<CarComponents::GearBox>();
	m_engine = std::make_unique<CarComponents::CarEngine>(m_gearBox.get());

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

	ComputeModelMatrix();

	m_headLightLeft->SetDirection(m_direction + glm::vec3(0, -0.07, 0));
	m_headLightLeft->SetPosition(
		m_position + glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_direction)) * 1.2f
		+ glm::vec3(0, 1, 0) - m_direction * 0.2f);

	m_headLightRight->SetDirection(m_direction + glm::vec3(0, -0.07, 0));
	m_headLightRight->SetPosition(
		m_position + glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_direction)) * -1.2f
		+ glm::vec3(0, 1, 0) - m_direction * 0.2f);

	m_placeTracker->UpdateCurrentPositionOnTrack();

	//std::cout << std::boolalpha << m_placeTracker->IsOutsideOfTrack() << std::endl;
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

void Car::InitPlaceTracker(const Track* track)
{
	m_placeTracker = std::make_unique<PlaceTracker>(this, track);
}

int Car::GetGear() const
{
	return m_gearBox->GetCurrentGear();
}

float Car::GetSpeed() const
{
	return m_engine->GetSpeedKmh();
}

float Car::GetProgress() const
{
	return m_placeTracker->GetCurrentPositionAsPercent();
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
