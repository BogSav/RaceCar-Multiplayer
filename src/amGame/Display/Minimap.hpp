#pragma once

#include "MainMenu/2dGeometries/2dCircle.hpp"
#include "MainMenu/2dGeometries/2dPolygon.hpp"

#include "MainMenu/Utilities/Camera.hpp"
#include "MainMenu/Utilities/Utilities.hpp"


class MiniMap
{
public:
	MiniMap(
		const WindowObject* window,
		const glm::vec2 screenPosition,
		const float width,
		const float height,
		const float left = -200.f,
		const float bottom = -200.f)
		: m_screenPosition(screenPosition),
		  m_width(width),
		  m_height(height),
		  m_ortoCamera(new CustomCamera())
	{
		m_ortoCamera->Set(
			glm::vec3(0, 10, 0),
			glm::vec3(0, 0, 0),
			glm::vec3(1, 0, 1),
			window->props.aspectRatio,
			true,
			left,
			-left,
			bottom,
			-bottom);

		m_carRepresentation = std::make_unique<Polygon2d>(
			m_screenPosition + glm::vec2{GetWidth() / 2.f - 10.f, GetHeight() / 2.f - 10.f},
			20.f,
			20.f,
			Colors::Magenta);
	}

	float GetHeight() const { return m_height; }
	float GetWidth() const { return m_width; }
	float GetX() const { return m_screenPosition.x; }
	float GetY() const { return m_screenPosition.y; }

	const CustomCamera* const GetCamera() const { return m_ortoCamera.get(); }

	void UpdateMinimapCameraBasedOnCarPosition(const Car* car)
	{
		m_ortoCamera->SetPosition(car->GetPosition() + glm::vec3{0, 10, 0}, car->GetPosition());
	}

	void RenderCarRepresentation(
		const Shader* const shader,
		const glm::mat3& transformMatrix,
		const glm::mat3 modelMatrix) const
	{
		m_carRepresentation->Render(shader, transformMatrix, modelMatrix);
	}

private:
	void UpdateCameraPosition(const glm::vec3& position, const glm::vec3& center)
	{
		m_ortoCamera->SetPosition(position, center);
	}

private:
	std::unique_ptr<CustomCamera> m_ortoCamera;
	std::unique_ptr<GeometryObject2d> m_carRepresentation;

	glm::vec2 m_screenPosition;
	float m_width;
	float m_height;
};