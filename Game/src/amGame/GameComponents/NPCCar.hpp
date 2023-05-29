#pragma once

#include "amGame/GameComponents/BaseCar.hpp"

class NPCCar : public BaseCar
{
public:
	NPCCar(const Shader* const shader, std::shared_ptr<CustomCamera> camera, std::size_t id);

	void Render() override;
	void Render(CustomCamera* const camera, const Shader* const shader);

	void Update(float deltaTime) override;

	uint8_t GetLapNr() const { return m_lapNr; }
	size_t GetPositionOnTrack() const { return m_positionOnTrack; }

private:
	void ComputeModelMatrix();
	const std::size_t id;
	size_t m_positionOnTrack;
	uint8_t m_lapNr;
};