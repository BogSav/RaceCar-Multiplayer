#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"

#include "amUtilities/Colors.hpp"

class TextEngine
{
public:
	TextEngine() = delete;
	TextEngine(
		const WindowObject* window,
		const std::string& fontName = "Hack-Bold.ttf",
		const int fontSize = 18)
		: m_textRenderer(new gfxc::TextRenderer(
			window->props.selfDir, window->GetResolution().x, window->GetResolution().y))
	{
		m_textRenderer->Load(
			PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, fontName), fontSize);
	}

	void Render(
		const std::string& text,
		const float posX,
		const float posY,
		const float scale = 1.f,
		Color color = Colors::White)
	{
		m_textRenderer->RenderText(
			text, posX, /*m_window->GetResolution().y -*/ posY, scale, color());
	}

	glm::vec2 GetSizeOfText(const std::string& text, const float scale = 1.f) {
		glm::vec2 test{0, 0};
		
		for (auto c = text.cbegin(); c != text.cend(); c++)
		{
			gfxc::Character ch = m_textRenderer->Characters[*c];

			test.x += ch.Size.x * scale;
			test.y = ch.Size.y * scale;
		}

		return test;
	}

private:
	gfxc::TextRenderer* const m_textRenderer;
};