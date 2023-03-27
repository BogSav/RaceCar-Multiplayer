#pragma once

#include "amGame/GameComponents/BaseCar.hpp"

class NPCCar : public BaseCar
{
public:
	NPCCar(const Shader* const shader, std::shared_ptr<CustomCamera> camera, Connection& client);

	void Render() override;
	void Render(CustomCamera* const camera, const Shader* const shader);

	void Update(float deltaTime) override;

private:
	void ComputeModelMatrix();
};