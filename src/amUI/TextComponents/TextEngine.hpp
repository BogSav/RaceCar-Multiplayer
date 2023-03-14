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

private:
	gfxc::TextRenderer* const m_textRenderer;
};