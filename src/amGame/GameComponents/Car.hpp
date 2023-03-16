#pragma once

#include "BaseCar.hpp"

#include "amGame/Physics/CarEngine.hpp"
#include "amGame/Physics/CarGearBox.hpp"

class Car : public BaseCar
{
public:
	Car() = delete;
	Car(const GameSettings* gameSettings, const WindowObject*, const Shader* const);

	void Render() override;
	void Update(float) override;

	void UpdateOrientation(float);

	void Accelerate();
	void Brake();
	void InertialDecceleration();

	void PrintData();

private:
	void Render(CustomCamera* const camera, const Shader* const) const;

private:
	void ComputeModelMatrix();

private:

	std::unique_ptr<CarComponents::CarEngine> m_engine;
	std::unique_ptr<CarComponents::GearBox> m_gearBox;

	std::unique_ptr<Mesh> m_mesh;

	const float m_distanceFromCamera;
	const float m_stirringAngularSpeed;
};