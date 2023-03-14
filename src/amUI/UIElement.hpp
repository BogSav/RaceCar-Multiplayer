#pragma once

#include "amGeometry/2dGeometries/2dCircle.hpp"
#include "amGeometry/2dGeometries/2dPolygon.hpp"
#include "amGeometry/2dGeometries/2dSemiCircle.hpp"

template <class ObjectType>
class UIElement
{
public:
	UIElement(
		std::unique_ptr<ObjectType>&& object,
		const float& scaleX = 1.f,
		const float& scaleY = 1.f,
		const float& posX = 0.f,
		const float& posY = 0.f)
		: m_scaleX(scaleX), m_scaleY(scaleY), m_posX(posX), m_posY(posY)
	{
		m_modelMatrix = utils::Translate2d(200, 200) * utils::Scale2d(10, 10);
		m_geometryObject = std::move(object);
	}

	void Update()
	{
		m_modelMatrix = utils::Translate2d(m_posX, m_posY) * utils::Scale2d(m_scaleX, m_scaleY);
	}

	void Render(const std::unique_ptr<Shader>& shader, const glm::mat3& transformationMatrix)
	{
		m_geometryObject->Render(shader.get(), transformationMatrix, m_modelMatrix);
	}

	bool HasBeenClicked(const float& mouseX, const float& mouseY) const
	{
		if (std::is_same<ObjectType, Polygon2d>::value == true)
		{
			return utils::CheckIfPointIsInside(
					   m_modelMatrix * m_geometryObject->GetMesh()->vertices[0].position,
					   m_modelMatrix * m_geometryObject->GetMesh()->vertices[1].position,
					   m_modelMatrix * m_geometryObject->GetMesh()->vertices[2].position,
					   glm::vec2{mouseX, mouseY})
				|| utils::CheckIfPointIsInside(
					   m_modelMatrix * m_geometryObject->GetMesh()->vertices[2].position,
					   m_modelMatrix * m_geometryObject->GetMesh()->vertices[3].position,
					   m_modelMatrix * m_geometryObject->GetMesh()->vertices[0].position,
					   glm::vec2{mouseX, mouseY});
		}
		return false;
	}

private:
	std::unique_ptr<GeometryObject2d> m_geometryObject;

	float m_scaleX;
	float m_scaleY;

	float m_posX;
	float m_posY;

	glm::mat3 m_modelMatrix;
};

typedef UIElement<Polygon2d> PolygonUIElement;
typedef UIElement<Circle2d> CircleUIElement;
typedef UIElement<SemiCircle2d> SemCircleUIElement;
