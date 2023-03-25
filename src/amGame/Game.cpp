#include "amGame/Game.hpp"

#include <iostream>

Game::Game(GameSettings* gameSettings) : m_gameSettings(gameSettings), m_client(nullptr)
{
}

Game::Game(GameSettings* gameSettings, const Client* client)
	: m_gameSettings(gameSettings), m_client(client)
{
}

Game::~Game()
{
}

void Game::Init()
{
	CreateShaders();
	CreateTextures();

	m_camera = std::make_unique<CustomCamera>(
		glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), window->props.aspectRatio);

	{
		m_cameraAttachedToCar = std::make_shared<CustomCamera>();

		m_car = std::make_unique<Car>(
			m_gameSettings, m_shaders["SimpleShader"].get(), m_cameraAttachedToCar);
		npc = std::make_unique<NPCCar>(
			m_gameSettings, m_shaders["SimpleShader"].get(), m_cameraAttachedToCar, (*m_client));
	}

	{
		m_track = std::make_unique<Track>(m_gameSettings);
		TrackBuilder* trackBuilder = new TrackBuilder(m_gameSettings, m_track.get());
		trackBuilder->BuildTrack(m_shaders["SimpleShader"].get(), m_cameraAttachedToCar.get());
		delete trackBuilder;

		m_car->InitPlaceTracker(m_track.get());
	}

	{
		m_field = std::make_unique<Field>(
			m_gameSettings, m_shaders["SimpleShader"].get(), m_cameraAttachedToCar.get());
	}

	m_lightSources.reserve(
		m_gameSettings->GetWorldParameters().m_nrOfStreetLights * 2
		+ m_gameSettings->GetWorldParameters().m_nrOfTrees);

	for (size_t i = 0; i < m_gameSettings->GetWorldParameters().m_nrOfStreetLights; i++)
	{
		StreetLight* streetLight = new StreetLight(
			m_gameSettings,
			m_shaders["TextureShader"].get(),
			m_cameraAttachedToCar.get(),
			m_textures["Pillar"]);
		streetLight->SetPosition(
			PositionGenerator::GeneratePosition(m_gameSettings, m_track.get(), true, i));
		streetLight->InstantiateLightSources();
		m_lightSources.push_back(streetLight->GetLightSources());

		m_streetLights.emplace_back(streetLight);
	}

	for (size_t i = 0; i < m_gameSettings->GetWorldParameters().m_nrOfTrees; i++)
	{
		Tree* tree = new Tree(
			m_gameSettings,
			m_shaders["TextureShader"].get(),
			m_cameraAttachedToCar.get(),
			m_textures["Crown"],
			m_textures["Trunk"]);
		tree->SetPosition(PositionGenerator::GeneratePosition(m_gameSettings, m_track.get()));
		tree->InstantiateLightSource();
		m_lightSources.push_back(tree->GetLightSoruce());

		m_trees.emplace_back(tree);
	}

	{
		m_screenElements = std::make_unique<ScreenElements>(m_gameSettings, m_car.get());
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
	DrawCoordinateSystem(
		m_cameraAttachedToCar->GetViewMatrix(), m_cameraAttachedToCar->GetProjectionMatrix());

	m_track->Render();
	m_car->Render();
	npc->Render();
	m_field->Render();

	for (auto& streetLight : m_streetLights)
	{
		streetLight->Render();
	}

	for (auto& tree : m_trees)
	{
		tree->Render();
	}

	m_screenElements->Render();

	if (m_gameSettings->m_frameTimerEnabled && frameTimer.PassedTime(0.1f))
		std::cout << 1.f / deltaTime << std::endl;
}

void Game::Update(float deltaTimeSeconds)
{
	m_car->Update(deltaTimeSeconds);
	npc->Update(deltaTimeSeconds);

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
