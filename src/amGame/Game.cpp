#include "amGame/Game.hpp"
//
//#include <iostream>
//
//void Game::Init()
//{
//	// Set the shaders
//	this->CreateShaders();
//
//	// Set the parameters
//	m_nrOfNPCs = 3;
//	m_nrOfStreetLights = 10;
//	m_nrOfTrees = 10;
//	m_curveCoefficent = 0.0005f;
//	m_resolution = window->props.resolution;
//	m_carReset = false;
//	m_frameTimer = false;
//
//	// Init the car and set up the camera
//	m_car = std::make_unique<Car>(window, shaders["VertexNormal"]);
//	m_camera = m_car->GetCamera();
//
//	// Init the track and the field
//	m_components.emplace_back(new Track(shaders["LightingShader"], m_camera.get(), 20));
//
//	// Init the car's position observer
//	m_car->InitializeObserver(dynamic_cast<const Track*>(m_components[0].get()));
//
//	// Init the field
//	m_components.emplace_back(
//		new Field(shaders["LightingShader"], m_camera.get(), glm::vec3{-480, 0, -350}, 930, 600));
//
//	// Generation of the NPCs, trees and streetLights
//	if (const Track* pista = dynamic_cast<const Track*>(m_components[0].get()))
//	{
//		// Generation of the NPCs
//		for (size_t i = 0; i < m_nrOfNPCs; i++)
//		{
//			NPC* tmp = NPC::CreateNewNPCRandomized(
//				pista->GetInteriorPoints(), shaders["LightingShader"], m_camera.get());
//			m_components.emplace_back(tmp);
//			m_car->GetPlaceTracker()->InsertNPC(tmp);
//		}
//
//		if (const Field* field = dynamic_cast<const Field*>(m_components[1].get()))
//		{
//			// Generate trees
//			for (size_t i = 0; i < m_nrOfTrees; i++)
//			{
//				m_components.emplace_back(Tree::GenerateRandomTree(
//					shaders["LightingShader"], m_camera.get(), pista, field));
//			}
//
//			// Generate street lights
//			for (size_t i = 0; i < m_nrOfStreetLights; i++)
//			{
//				StreetLight* tmp = StreetLight::GenerateRandomStreetLight(
//					shaders["SimpleShader"], m_camera.get(), pista, field);
//				m_components.emplace_back(tmp);
//				m_lightingComponents.push_back(tmp);
//			}
//		}
//	}
//
//	// Add the headlights to the light sources that will be used for rendering
//	m_lightingComponents.push_back(m_car->GetLeftHeadLight());
//	m_lightingComponents.push_back(m_car->GetRightHeadLight());
//
//	// Init the minimap and the screen objects
//	m_minimap = std::make_shared<MiniMap>(window, glm::vec2{900, 50}, 300.f, 150.f, -30.f, -30.f);
//	m_screen = std::make_unique<ScreenElements>(
//		window, m_minimap, m_car->GetTurometru(), m_car->GetObserver(), m_car->GetPlaceTracker());
//}
//
//
//void Game::FrameStart()
//{
//	// glClearColor(0.48f, 0.6f, 0.63f, 1.f);
//	glClearColor(0.16f, 0.16f, 0.16f, 0.f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}
//
//void Game::Update(float deltaTimeSeconds)
//{
//	this->UpdateCar(deltaTimeSeconds);
//
//	if (m_frameTimer && frametimer.PassedTime(0.5))
//		std::cout << 1 / deltaTimeSeconds << std::endl;
//
//	m_minimap->UpdateMinimapCameraBasedOnCarPosition(m_car.get());
//
//	this->UpdateGameComponents(deltaTimeSeconds);
//
//	this->RenderGameComponents();
//	this->RenderScreenObjects();
//	this->RenderMinimap();
//}
//
//void Game::FrameEnd()
//{
//	// DrawCoordinateSystem(m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
//}
//
//
//void Game::OnInputUpdate(float deltaTime, int mods)
//{
//	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
//	{
//		// Control for the camera independent movement
//		float cameraSpeed = 50.f;
//
//		if (window->KeyHold(GLFW_KEY_W))
//		{
//			m_camera->TranslateForward(deltaTime * cameraSpeed);
//		}
//		if (window->KeyHold(GLFW_KEY_A))
//		{
//			m_camera->TranslateRight(-deltaTime * cameraSpeed);
//		}
//		if (window->KeyHold(GLFW_KEY_S))
//		{
//			m_camera->TranslateForward(-deltaTime * cameraSpeed);
//		}
//		if (window->KeyHold(GLFW_KEY_D))
//		{
//			m_camera->TranslateRight(deltaTime * cameraSpeed);
//		}
//		if (window->KeyHold(GLFW_KEY_Q))
//		{
//			m_camera->TranslateUpward(deltaTime * cameraSpeed);
//		}
//		if (window->KeyHold(GLFW_KEY_E))
//		{
//			m_camera->TranslateUpward(-deltaTime * cameraSpeed);
//		}
//	}
//	else
//	{
//		// Control for the car
//		if (window->KeyHold(GLFW_KEY_W))
//		{
//			m_car->Accelerate();
//		}
//		else if (window->KeyHold(GLFW_KEY_S))
//		{
//			m_car->Brake();
//		}
//		else
//		{
//			m_car->InertialDecceleration();
//		}
//
//		if (window->KeyHold(GLFW_KEY_A))
//		{
//			m_car->UpdateOrientation(deltaTime);
//		}
//		else if (window->KeyHold(GLFW_KEY_D))
//		{
//			m_car->UpdateOrientation(-deltaTime);
//		}
//	}
//}
//
//void Game::OnKeyPress(int key, int mods)
//{
//	// Forza horizon thing
//	if (key == GLFW_KEY_SPACE)
//	{
//		m_car->RestoreLastState();
//	}
//}
//
//void Game::OnKeyRelease(int key, int mods)
//{
//}
//
//void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
//{
//	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
//	{
//		float sensivityOX = 0.001f;
//		float sensivityOY = 0.001f;
//
//		if (window->GetSpecialKeyState() == 0)
//		{
//			m_camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
//			m_camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
//		}
//	}
//}
//
//void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
//{
//}
//
//void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
//{
//}
//
//void Game::RenderGameComponents()
//{
//	// Set the correct viewport
//	glClear(GL_DEPTH_BUFFER_BIT);
//	glm::ivec2 resolution = window->GetResolution();
//	glViewport(0, 0, resolution.x, resolution.y);
//
//	// Render NPCs, trees and streetLights based on thei specific shader
//	std::for_each(
//		m_components.begin(),
//		m_components.end(),
//		[this](const auto& curr)
//		{
//			if (dynamic_cast<const LightSourceAdapter*>(curr.get()))
//			{
//				curr->Render(m_car->GetPosition(), m_curveCoefficent);
//			}
//			else
//			{
//				curr->Render(
//					m_car->GetPosition(),
//					m_camera->GetPosition(),
//					m_curveCoefficent,
//					m_lightingComponents);
//			}
//		});
//
//	// Randam si masina cu shaderul de vertexnormal
//	m_car->Render();
//}
//
//void Game::RenderMinimap()
//{
//	// Set the minimap area
//	glClear(GL_DEPTH_BUFFER_BIT);
//	glViewport(
//		static_cast<int>(m_minimap->GetX()),
//		static_cast<int>(m_minimap->GetY()),
//		static_cast<int>(m_minimap->GetWidth()),
//		static_cast<int>(m_minimap->GetHeight()));
//
//	// Render each game components in the minimap area with a simpleShader and with the ortho camera
//	std::for_each(
//		m_components.begin(),
//		m_components.end(),
//		[this](const auto& curr)
//		{ curr->Render(shaders["SimpleShader"], m_minimap->GetCamera()); });
//
//	// m_car->Render(m_minimap->GetCamera(), shaders["VertexNormal"]);
//
//	// Render the representation of the car to the minimap because it's too small and looks shitty
//	glClear(GL_DEPTH_BUFFER_BIT);
//	glViewport(0, 0, m_resolution.x, m_resolution.y);
//	m_screen->RenderCarRepresentation();
//}
//
//void Game::RenderScreenObjects()
//{
//	// Render the screen objects, such as speedometer, progress handler, lap nr and place
//	glClear(GL_DEPTH_BUFFER_BIT);
//	glm::ivec2 resolution = window->GetResolution();
//	glViewport(0, 0, m_resolution.x, m_resolution.y);
//
//	// First we update the components and than we render them
//	m_screen->Update();
//	m_screen->Render();
//}
//
//void Game::UpdateGameComponents(float deltaTime)
//{
//	// This is self explanatory
//	std::for_each(
//		m_components.begin(),
//		m_components.end(),
//		[this, &deltaTime](const auto& curr) { curr->Update(deltaTime); });
//}
//
//void Game::UpdateCar(float deltaTimeSeconds)
//{
//	if (m_carReset)
//	{
//		if (m_resetTimer.PassedTime(2))
//		{
//			m_carReset = false;
//			m_car->RestoreLastState();
//		}
//	}
//	else
//	{
//		// m_car->PrintData();
//		m_car->Update(deltaTimeSeconds);
//
//		const Track* track = dynamic_cast<const Track*>(m_components.data()[0].get());
//
//		// Update the oberver's point on track
//		m_car->GetObserver()->UpdateCurrentPoint(track, m_car.get());
//
//		// Check if car is on the track
//		if (!CollisionEngine::IsOnTrack(track, m_car.get()))
//		{
//			m_carReset = true;
//			std::cout << "A iesit de pe pista" << std::endl;
//		}
//
//		// Check if car collides with a NPC
//		for (const auto& gameComponent : m_components)
//		{
//			if (const NPC* npc = dynamic_cast<const NPC*>(gameComponent.get()))
//			{
//				if (CollisionEngine::IsCollidingWithNPC(m_car.get(), npc))
//				{
//					std::cout << "Accident!" << std::endl;
//					m_carReset = true;
//					break;
//				}
//			}
//		}
//	}
//}
//
//void Game::CreateShaders()
//{
//	{
//		// Simple shader is a shader that doesn't use any lightiong but have suppport for the curvature
//		Shader* shader = new Shader("SimpleShader");
//		shader->AddShader(
//			PATH_JOIN(
//				window->props.selfDir, SOURCE_PATH::Game, "Shaders", "SimpleVertexShader.glsl"),
//			GL_VERTEX_SHADER);
//		shader->AddShader(
//			PATH_JOIN(window->props.selfDir, SOURCE_PATH::Game, "Shaders", "FragmentShader.glsl"),
//			GL_FRAGMENT_SHADER);
//		shader->CreateAndLink();
//		shaders[shader->GetName()] = shader;
//	}
//
//	{
//		// Lighting shader is a shader that has support for the curvaturew and also lighting models
//		Shader* shader = new Shader("LightingShader");
//		shader->AddShader(
//			PATH_JOIN(
//				window->props.selfDir, SOURCE_PATH::Game, "Shaders", "LightVertexShader.glsl"),
//			GL_VERTEX_SHADER);
//		shader->AddShader(
//			PATH_JOIN(
//				window->props.selfDir, SOURCE_PATH::Game, "Shaders", "LightFragmentShader.glsl"),
//			GL_FRAGMENT_SHADER);
//		shader->CreateAndLink();
//		shaders[shader->GetName()] = shader;
//	}
//}
