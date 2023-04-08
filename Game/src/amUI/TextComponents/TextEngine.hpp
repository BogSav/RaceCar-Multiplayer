#pragma once

#include "amUtilities/Colors.hpp"
#include "components/simple_scene.h"
#include "components/text_renderer.h"

class TextEngine
{
public:
	TextEngine(const std::string& fontName = "Hack-Bold.ttf", const int fontSize = 18)
		: m_textRenderer(new gfxc::TextRenderer(
			Engine::GetWindow()->props.selfDir,
			Engine::GetWindow()->GetResolution().x,
			Engine::GetWindow()->GetResolution().y))
	{
		m_textRenderer->Load(
			PATH_JOIN(Engine::GetWindow()->props.selfDir, RESOURCE_PATH::FONTS, fontName),
			fontSize);
	}

	void Render(
		const std::string& text,
		const float posX,
		const float posY,
		const float scale = 1.f,
		Color color = Colors::White)
	{
		m_textRenderer->RenderText(
			text, posX, Engine::GetWindow()->GetResolution().y - posY, scale, color());
	}

	glm::vec2 GetSizeOfText(const std::string& text, const float scale = 1.f)
	{
		float width = 0.f;
		float height = 0.f;
		for (auto c = text.cbegin(); c != text.cend(); c++)
		{
			gfxc::Character ch = m_textRenderer->Characters[*c];
			width += (ch.Advance >> 6) * scale;
			height = std::max(height, static_cast<float>(ch.Size.y));
		}

		return {width, height};
	}

private:
	gfxc::TextRenderer* const m_textRenderer;
};