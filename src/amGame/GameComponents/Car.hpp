#pragma once

#include "Game/Utilities/Camera.hpp"
#include "Game/Physics/Engine.hpp"

#include "Game/Display/Speedometer.hpp"
#include "Game/GameComponents/Track.hpp"

class Car
{
public:
	Car() = delete;
	Car(const WindowObject*, const Shader* const);

	void InitializeObserver(const Track*);

	void Render() const;
	void Render(CustomCamera* const camera, const Shader* const) const;

	void Update(double);
	void UpdateOrientation(float);

	void Accelerate();
	void Brake();
	void InertialDecceleration();

	void RestoreLastState();

	void PrintData();

	const glm::vec3& GetPosition() const { return m_position; }
	const std::shared_ptr<Speedometer>& GetTurometru() const { return m_speedometer; }
	const std::shared_ptr<CustomCamera>& GetCamera() const { return m_camera; };

	const HeadLight* GetLeftHeadLight() const { return m_headLightLeft.get(); }
	const HeadLight* GetRightHeadLight() const { return m_headLightRight.get(); }

	friend class CollisionEngine;

private:
	void ComputeModelMatrix();
	void UpdateLastParameters();

private:
	const Shader* const m_shader;
	std::shared_ptr<CustomCamera> m_camera;
	std::unique_ptr<CustomCamera> m_lastCamera;

	std::unique_ptr<physics::Engine> m_engine;
	std::unique_ptr<physics::GearBox> m_gearBox;
	std::shared_ptr<Speedometer> m_speedometer;

	std::unique_ptr<HeadLight> m_headLightLeft;
	std::unique_ptr<HeadLight> m_headLightRight;

	std::unique_ptr<Mesh> m_mesh;

	glm::vec3 m_position;
	glm::vec3 m_direction;
	float m_angleOrientation;
	glm::vec3 m_scale;

	glm::vec3 m_lastPosition;
	glm::vec3 m_lastDirection;
	float m_lastOrientationAngle;

	DTimer m_updateLastParametersTimer;

	const float m_distanceFromCamera;
	const float m_stirringAngularSpeed;

	glm::mat4 m_modelMatrix;
};