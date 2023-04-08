#pragma once

#include "TextComponents/TextEngine.hpp"
#include "UIElement.hpp"

template <class ObjectType>
class Button : public UIElement<ObjectType>
{
private:
	using Element = UIElement<ObjectType>;

public:
	Button(
		std::shared_ptr<ObjectType>& object,
		const float& posX,
		const float& posY,
		const float& scaleX,
		const float& scaleY,
		std::shared_ptr<TextEngine> textEngine,
		std::string text)
		: Element(object, posX, posY, scaleX, scaleY), m_textEngine(textEngine), m_text(text)
	{
		m_bbox.InitBBoxBasedOnMesh(object->GetMesh().get());
		m_textHalfDimensions = m_textEngine->GetSizeOfText(m_text) / 2.f;
	}

	void Render(
		const std::unique_ptr<Shader>& shader, const glm::mat3& transformationMatrix) override
	{
		m_textEngine->Render(
			m_text,
			Element::m_posX - m_textHalfDimensions.x,
			Element::m_posY + m_textHalfDimensions.y);
		Element::Render(shader, transformationMatrix);
	}

	bool HasBeenClicked(const float& mouseX, const float& mouseY) const
	{
		if (std::is_same<ObjectType, Polygon2d>::value == true)
		{
			return m_bbox.GetTranformedBBox(Element::m_modelMatrix)
				.IsInside(glm::vec2{mouseX, mouseY});
		}
		if (std::is_same<ObjectType, Circle2d>::value == true)
		{
			// TODO
			return false;
		}
		if (std::is_same<ObjectType, SemiCircle2d>::value == true)
		{
			// TODO
			return false;
		}

		return false;
	}

private:
	std::shared_ptr<TextEngine> m_textEngine;
	std::string m_text;

	glm::ivec2 m_textHalfDimensions;

	BoundingBox2d m_bbox;
};