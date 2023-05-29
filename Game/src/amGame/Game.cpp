#include "amGame/Game.hpp"

#include "amConnectivity/Connection.hpp"

#include <iostream>

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{

	// Creem si incarcam shaderele si texturile ce vor fii folosite ulterior
	CreateShaders();
	CreateTextures();

	// Creem camera principala
	m_camera = std::make_unique<CustomCamera>(
		glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), window->props.aspectRatio);

	// Creare masina jucatorului si NPC-uri
	{
		m_cameraAttachedToCar = std::make_shared<CustomCamera>();

		m_car = std::make_unique<Car>(shaders["VertexNormal"], m_cameraAttachedToCar);

		if (Engine::GetGameSettings()->m_isMultiplayer)
		{
			for (std::size_t i = 0; i < maxNumberOfClients; i++)
			{
				if (Engine::GetConnection()->GetClientId() != i)
				{
					m_npcs.emplace_back(std::make_unique<NPCCar>(
						m_shaders["SimpleShader"].get(), m_cameraAttachedToCar, i));
				}
			}
		}
	}

	// Generare pista
	{
		m_track = std::make_unique<Track>();
		TrackBuilder* trackBuilder = new TrackBuilder(m_track.get());
		trackBuilder->BuildTrack(
			m_shaders[m_lightingEnabled ? "LightShader" : "SimpleShader"].get(),
			m_cameraAttachedToCar.get());
		delete trackBuilder;

		m_car->InitPlaceTracker(m_track.get(), m_npcs);
	}

	// Generare camp
	{
		m_field = std::make_unique<Field>(
			m_shaders[m_lightingEnabled ? "LightShader" : "SimpleShader"].get(),
			m_cameraAttachedToCar.get());
	}

	// Alocam preevntiv spatiu pentru obiectele generatoarte de lumina
	m_lightSources.reserve(
		2 + Engine::GetGameSettings()->GetWorldParameters().m_nrOfStreetLights * 2
		+ Engine::GetGameSettings()->GetWorldParameters().m_nrOfTrees);

	m_lightSources.push_back(m_car->GetLightSources());

	// Generare stalpi de lumina
	for (size_t i = 0; i < Engine::GetGameSettings()->GetWorldParameters().m_nrOfStreetLights; i++)
	{
		StreetLight* streetLight = new StreetLight(
			m_shaders[m_lightingEnabled ? "TextureShader" : "SimpleShader"].get(),
			m_cameraAttachedToCar.get(),
			m_textures["Pillar"]);
		streetLight->SetPosition(PositionGenerator::GeneratePosition(m_track.get(), true, i));
		streetLight->InstantiateLightSources();
		m_lightSources.push_back(streetLight->GetLightSources());

		m_streetLights.emplace_back(streetLight);
	}

	// Generare copaci
	for (size_t i = 0; i < Engine::GetGameSettings()->GetWorldParameters().m_nrOfTrees; i++)
	{
		Tree* tree = new Tree(
			m_shaders[m_lightingEnabled ? "TextureShader" : "SimpleShader"].get(),
			m_cameraAttachedToCar.get(),
			m_textures["Crown"],
			m_textures["Trunk"]);
		tree->SetPosition(PositionGenerator::GeneratePosition(m_track.get()));
		tree->InstantiateLightSource();
		m_lightSources.push_back(tree->GetLightSoruce());

		m_trees.emplace_back(tree);
	}

	// Creare element de display
	{
		m_screenElements = std::make_unique<ScreenElements>(m_car.get());
	}
}

void Game::FrameStart()
{
	// glClearColor(0.48f, 0.6f, 0.63f, 1.f);
	glClearColor(0.16f, 0.16f, 0.16f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::Render(float deltaTime)
{
	//DrawCoordinateSystem(
	//	m_cameraAttachedToCar->GetViewMatrix(), m_cameraAttachedToCar->GetProjectionMatrix());
	
	m_car->Render();
	
	for (auto& npc : m_npcs)
	{
		npc->Render();
	}


	if (m_lightingEnabled)
	{
		m_track->Render(m_camera->GetPosition(), m_lightSources);
		m_field->Render(m_camera->GetPosition(), m_lightSources);

		for (auto& streetLight : m_streetLights)
		{
			streetLight->Render(m_camera->GetPosition(), m_lightSources);
		}

		for (auto& tree : m_trees)
		{
			tree->Render(m_camera->GetPosition(), m_lightSources);
		}
	}
	else
	{
		m_track->Render();
		m_field->Render();

		for (auto& streetLight : m_streetLights)
		{
			streetLight->Render();
		}

		for (auto& tree : m_trees)
		{
			tree->Render();
		}
	}

	m_screenElements->Render(deltaTime);

	// if (Engine::GetGameSettings()->m_forceFixedFramerate)
	//	framerateTimer.SuspendThreadToSyncFramerate(deltaTime);

	// if (Engine::GetGameSettings()->m_frameTimerEnabled)
	//	std::cout << 1.f / deltaTime << std::endl;

	if (justInCase.PassedTime(2.0))
		m_car->PrintData();
}

void Game::Update(float deltaTimeSeconds)
{
	m_car->Update(deltaTimeSeconds);

	for (auto& npc : m_npcs)
	{
		npc->Update(deltaTimeSeconds);
	}

	m_screenElements->Update();
	// std::cout << m_placeTracker->GetCurrentPositionAsPercent() << std::endl;
}

void Game::FrameEnd()
{
}


void Game::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		// Control for the camera independent movement
		float cameraSpeed = 50.f;

		if (window->KeyHold(GLFW_KEY_W))
		{
			m_camera->TranslateForward(deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_A))
		{
			m_camera->TranslateRight(-deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_S))
		{
			m_camera->TranslateForward(-deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_D))
		{
			m_camera->TranslateRight(deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_Q))
		{
			m_camera->TranslateUpward(deltaTime * cameraSpeed);
		}
		if (window->KeyHold(GLFW_KEY_E))
		{
			m_camera->TranslateUpward(-deltaTime * cameraSpeed);
		}
	}
	else
	{
		// Control for the car
		if (window->KeyHold(GLFW_KEY_W))
		{
			// if (m_cameraAttachedToCar->GetFov() < RADIANS(85))
			//	m_cameraAttachedToCar->SetFov(m_cameraAttachedToCar->GetFov() + deltaTime *
			// RADIANS(5));
			m_car->Accelerate();
		}
		else if (window->KeyHold(GLFW_KEY_S))
		{
			// if (m_cameraAttachedToCar->GetFov() > RADIANS(70))
			//	m_cameraAttachedToCar->SetFov(
			//		m_cameraAttachedToCar->GetFov() - deltaTime * RADIANS(20));
			m_car->Brake();
		}
		else
		{
			// if (m_cameraAttachedToCar->GetFov() > RADIANS(70))
			//	m_cameraAttachedToCar->SetFov(
			//		m_cameraAttachedToCar->GetFov() - deltaTime * RADIANS(10));
			m_car->InertialDecceleration();
		}

		if (window->KeyHold(GLFW_KEY_A))
		{
			m_car->UpdateOrientation(deltaTime);
		}
		else if (window->KeyHold(GLFW_KEY_D))
		{
			m_car->UpdateOrientation(-deltaTime);
		}
	}
}

void Game::OnKeyPress(int key, int mods)
{
}

void Game::OnKeyRelease(int key, int mods)
{
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0)
		{
			m_camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
			m_camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
		}
	}
}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Game::CreateShaders()
{
	{
		Shader* shader = new Shader("TextureShader");
		shader->AddShader(
			PATH_JOIN(
				window->props.selfDir,
				SOURCE_PATH::CustomShaders,
				"3dShaders",
				"TextureVertexShader.glsl"),
			GL_VERTEX_SHADER);
		shader->AddShader(
			PATH_JOIN(
				window->props.selfDir,
				SOURCE_PATH::CustomShaders,
				"3dShaders",
				"TextureFragmentShader.glsl"),
			GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		m_shaders[shader->GetName()] = std::unique_ptr<Shader>(shader);
	}

	{
		Shader* shader = new Shader("LightShader");
		shader->AddShader(
			PATH_JOIN(
				window->props.selfDir,
				SOURCE_PATH::CustomShaders,
				"3dShaders",
				"LightVertexShader.glsl"),
			GL_VERTEX_SHADER);
		shader->AddShader(
			PATH_JOIN(
				window->props.selfDir,
				SOURCE_PATH::CustomShaders,
				"3dShaders",
				"LightFragmentShader.glsl"),
			GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		m_shaders[shader->GetName()] = std::unique_ptr<Shader>(shader);
	}

	{
		Shader* shader = new Shader("SimpleShader");
		shader->AddShader(
			PATH_JOIN(
				window->props.selfDir,
				SOURCE_PATH::CustomShaders,
				"3dShaders",
				"VertexShader.glsl"),
			GL_VERTEX_SHADER);
		shader->AddShader(
			PATH_JOIN(
				window->props.selfDir,
				SOURCE_PATH::CustomShaders,
				"3dShaders",
				"FragmentShader.glsl"),
			GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		m_shaders[shader->GetName()] = std::unique_ptr<Shader>(shader);
	}
}

void Game::CreateTextures()
{
	{
		std::shared_ptr<Texture2D> presentTexture = std::make_shared<Texture2D>();
		presentTexture->Load2D(
			PATH_JOIN(window->props.selfDir, "assets", "textures", "gift.jpg").c_str());
		m_textures["Present"] = presentTexture;
	}

	{
		std::shared_ptr<Texture2D> snowTexture = std::make_shared<Texture2D>();
		snowTexture->Load2D(
			PATH_JOIN(window->props.selfDir, "assets", "textures", "snow.jpg").c_str());
		m_textures["Snow"] = snowTexture;
	}

	{
		std::shared_ptr<Texture2D> rockTexture = std::make_shared<Texture2D>();
		rockTexture->Load2D(
			PATH_JOIN(window->props.selfDir, "assets", "textures", "rock.jpg").c_str());
		m_textures["Rock"] = rockTexture;
	}

	{
		std::shared_ptr<Texture2D> crownTexture = std::make_shared<Texture2D>();
		crownTexture->Load2D(
			PATH_JOIN(window->props.selfDir, "assets", "textures", "treeCrown.jpg").c_str());
		m_textures["Crown"] = crownTexture;
	}

	{
		std::shared_ptr<Texture2D> trunkTexture = std::make_shared<Texture2D>();
		trunkTexture->Load2D(
			PATH_JOIN(window->props.selfDir, "assets", "textures", "treeTrunk.jpg").c_str());
		m_textures["Trunk"] = trunkTexture;
	}

	{
		std::shared_ptr<Texture2D> trunkTexture = std::make_shared<Texture2D>();
		trunkTexture->Load2D(
			PATH_JOIN(window->props.selfDir, "assets", "textures", "pillar.jpg").c_str());
		m_textures["Pillar"] = trunkTexture;
	}
}
