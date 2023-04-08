#pragma once

#include "amGame/GameComponents/BaseCar.hpp"

class NPCCar : public BaseCar
{
public:
	NPCCar(const Shader* const shader, std::shared_ptr<CustomCamera> camera, std::size_t id);

	void Render() override;
	void Render(CustomCamera* const camera, const Shader* const shader);

	void Update(float deltaTime) override;

private:
	void ComputeModelMatrix();
	const std::size_t id;
};