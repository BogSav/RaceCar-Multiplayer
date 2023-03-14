#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "UIElement.hpp"

#include "TextComponents/TextEngine.hpp"

class MainMenu : public gfxc::SimpleScene
{
public:
	MainMenu();
	~MainMenu() = default;

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) override;
	void FrameEnd() override;

	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;

	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

private:
	const utils::LogicSpace m_NDCSpace;
	const utils::LogicSpace m_logicSpace;

	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<TextEngine> m_textEngine;

	glm::mat3 m_logicToNDCSpaceMatrix;

	std::unique_ptr<PolygonUIElement> m_optionsElement;

	std::unique_ptr<GeometryObject2d> test;

	std::string teststring;
};