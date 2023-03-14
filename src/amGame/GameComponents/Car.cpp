#include "Car.hpp"
#include "Game/Physics/Collision/CollisionEngine.hpp"

#include <iostream>

Car::Car(const WindowObject* window, const Shader* const shader)
	: m_shader(shader),
	  m_camera(new CustomCamera()),
	  m_lastCamera(new CustomCamera()),
	  m_lastObserver(new PositionObeserver()),
	  m_placeTracker(new PlaceTracker()),
	  m_distanceFromCamera(6),
	  m_stirringAngularSpeed(RADIANS(45))
{
	m_mesh = std::make_unique<Mesh>();
	m_mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "mele"), "F1.obj");

	m_modelMatrix = glm::mat4(1);

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

	m_gearBox = std::make_unique<physics::GearBox>();
	m_engine = std::make_unique<physics::Engine>(
		physics::PhysicsComponents::InstantiateComponents(), m_gearBox.get());

	m_speedometer = std::make_shared<Speedometer>(window);

	m_headLightLeft = std::make_unique<HeadLight>(
		glm::vec3(0, 0, 0), Colors::White, 15.0f, RADIANS(15), m_direction, glm::vec3(0, 0, 1));
	m_headLightRight = std::make_unique<HeadLight>(
		glm::vec3(0, 0, 0), Colors::White, 15.0f, RADIANS(15), m_direction, glm::vec3(0, 0, 1));

	this->ComputeModelMatrix();
}

void Car::InitializeObserver(const Track* track)
{
	m_observer = std::make_shared<PositionObeserver>(
		CollisionEngine::GetCurrentPlaceOnTrackForCar(track, this),
		track->GetInteriorPoints().size());
}

void Car::Render() const
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

void Car::Update(double deltaTime)
{
	glm::vec3 newPosition = m_engine->GetNewPosition(m_position, m_direction, deltaTime);
	m_camera->MoveForward(glm::l2Norm(newPosition - m_position));
	m_position = newPosition;

	this->ComputeModelMatrix();
	this->UpdateLastParameters();

	m_headLightLeft->SetDirection(m_direction + glm::vec3(0, -0.07, 0));
	m_headLightLeft->SetPosition(
		m_position + glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_direction)) * 1.2f
		+ glm::vec3(0, 1, 0) - m_direction * 0.2f);

	m_headLightRight->SetDirection(m_direction + glm::vec3(0, -0.07, 0));
	m_headLightRight->SetPosition(
		m_position + glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_direction)) * -1.2f
		+ glm::vec3(0, 1, 0) - m_direction * 0.2f);

	m_speedometer->UpdateSpeed(static_cast<float>(m_engine->GetSpeedKmh()));
	m_speedometer->UpdateGear(m_engine->GetCurrentGear());

	m_placeTracker->UpdatePlace(m_observer->GetTotalNrOfQuads(), m_observer->GetStartPoint(), m_observer->GetTrackPointsNr());
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

void Car::RestoreLastState()
{
	m_position = m_lastPosition;
	m_direction = m_lastDirection;
	m_angleOrientation = m_lastOrientationAngle;

	*m_observer = *m_lastObserver;
	*m_camera = *m_lastCamera;

	this->ComputeModelMatrix();

	m_engine->Reset();
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

void Car::UpdateLastParameters()
{
	if (m_updateLastParametersTimer.PassedTime(5))
	{
		m_lastDirection = m_direction;
		m_lastOrientationAngle = m_angleOrientation;
		m_lastPosition = m_position;

		*m_lastCamera = *m_camera;
		*m_lastObserver = *m_observer;
	}
}

Car::PositionObeserver::PositionObeserver(int startPt, size_t trackPointsNr)
	: m_startPoint(startPt), m_trackPointsNr(trackPointsNr)
{
	m_currentPoint = m_startPoint;
	m_prevQuad = m_currentPoint;
	m_totalNrOfQuads = m_currentPoint;
}

void Car::PositionObeserver::UpdateCurrentPoint(const Track* track, const Car* car)
{
	m_currentPoint = CollisionEngine::GetCurrentPlaceOnTrackForCar(track, car);
	
	if (m_currentPoint < m_startPoint)
		m_currentPoint = m_currentPoint + (int)m_trackPointsNr;

	if (m_currentPoint != m_prevQuad)
		m_totalNrOfQuads++;

	m_prevQuad = m_currentPoint;
}

float Car::PositionObeserver::GetPercent()
{
	float percent = 100.f * static_cast<float>(m_currentPoint - m_startPoint)
		/ static_cast<float>(m_trackPointsNr);

	if (percent > 99.5f)
	{
		m_currentPoint = m_startPoint;
	}

	return percent;
}

int Car::PositionObeserver::GetTotalNrOfQuads() const
{
	return m_totalNrOfQuads;
}

int Car::PositionObeserver::GetStartPoint() const
{
	return m_startPoint;
}

int Car::PositionObeserver::GetTrackPointsNr() const
{
	return static_cast<int>(m_trackPointsNr);
}

void Car::PlaceTracker::InsertNPC(const NPC* npc)
{
	m_npcs.push_back(npc);
}

void Car::PlaceTracker::UpdatePlace(int nrOfQuads, int startQuad, int totalNrOfPts)
{
	m_place = static_cast<int>(m_npcs.size()) + 1;
	std::for_each(
		m_npcs.begin(),
		m_npcs.end(),
		[this, nrOfQuads, startQuad, totalNrOfPts](const NPC* npc)
		{
			int& tot = const_cast<NPC*>(npc)->GetTotalNrOfQuads();
			if (tot <= startQuad)
				tot += totalNrOfPts;

			if (tot < nrOfQuads)
				m_place--;
		});
}

std::pair<int, size_t> Car::PlaceTracker::GetPlaceAndNrOfNPCs()
{
	return std::pair<int, size_t>(m_place, m_npcs.size() + 1);
}
