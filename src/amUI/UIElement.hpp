#pragma once

#include "BoundingBox2d.hpp"
#include "amGeometry/2dGeometries/2dCircle.hpp"
#include "amGeometry/2dGeometries/2dPolygon.hpp"
#include "amGeometry/2dGeometries/2dSemiCircle.hpp"

template <class ObjectType>
class UIElement
{
public:
	UIElement(
		std::shared_ptr<ObjectType>& object,
		const float& posX = 0.f,
		const float& posY = 0.f,
		const float& scaleX = 1.f,
		const float& scaleY = 1.f)
		: m_scaleX(scaleX), m_scaleY(scaleY), m_posX(posX), m_posY(posY)
	{
		m_modelMatrix = utils::Translate2d(m_posX, m_posY) * utils::Scale2d(m_scaleX, m_scaleY);
		m_geometryObject = object;
		m_bbox.InitBBoxBasedOnMesh(object->GetMesh().get());
	}

	UIElement(
		std::shared_ptr<ObjectType>& object,
		const float& posX = 0.f,
		const float& posY = 0.f,
		const float& uniformScaleFactor = 1.f)
		: UIElement(object, posX, posY, uniformScaleFactor, uniformScaleFactor)
	{
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
			return m_bbox.GetTranformedBBox(m_modelMatrix).IsInside(glm::vec2{mouseX, mouseY});
		}
		return false;
	}

private:
	std::shared_ptr<ObjectType> m_geometryObject;

	float m_scaleX;
	float m_scaleY;

	float m_posX;
	float m_posY;

	glm::mat3 m_modelMatrix;

	BoundingBox2d m_bbox;
};

typedef UIElement<Polygon2d> PolygonUIElement;
typedef UIElement<Circle2d> CircleUIElement;
typedef UIElement<SemiCircle2d> SemCircleUIElement;
