#pragma once

#include "BaseCar.hpp"
#include "amGame/Physics/CarEngine.hpp"
#include "amGame/Physics/CarGearBox.hpp"

class Car : public BaseCar
{
public:
	Car() = delete;
	Car(const Shader* const, std::shared_ptr<CustomCamera>);

	void Render() override;
	void Update(float) override;

	void UpdateOrientation(float);

	void Accelerate();
	void Brake();
	void InertialDecceleration();

	void InitPlaceTracker(const class Track*, const std::vector<std::unique_ptr<class NPCCar>>&);

	int GetGear() const;
	float GetSpeed() const;
	float GetProgress() const;
	uint8_t GetLapNumber() const;
	std::size_t GetPlace() const;

	void PrintData();

private:
	void Render(CustomCamera* const camera, const Shader* const) const;
	void ComputeModelMatrix();

private:
	std::unique_ptr<CarComponents::CarEngine> m_engine;
	std::unique_ptr<CarComponents::GearBox> m_gearBox;

	const float m_distanceFromCamera;
	const float m_stirringAngularSpeed;

	std::unique_ptr<class PlaceTracker> m_placeTracker;
};