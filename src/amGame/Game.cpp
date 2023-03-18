#include "amGame/Game.hpp"

Game::Game(GameSettings* gameSettings) : m_gameSettings(gameSettings)
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

	m_track = std::make_unique<Track>(m_gameSettings);
	{
		TrackBuilder* trackBuilder = new TrackBuilder(m_gameSettings, m_track.get());
		trackBuilder->BuildTrack(m_shaders["SimpleShader"].get(), m_camera.get());
		delete trackBuilder;
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
	DrawCoordinateSystem(m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
	m_track->Render();
}

void Game::Update(float deltaTimeSeconds)
{
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
	// else
	//{
	//	// Control for the car
	//	if (window->KeyHold(GLFW_KEY_W))
	//	{
	//		m_car->Accelerate();
	//	}
	//	else if (window->KeyHold(GLFW_KEY_S))
	//	{
	//		m_car->Brake();
	//	}
	//	else
	//	{
	//		m_car->InertialDecceleration();
	//	}

	//	if (window->KeyHold(GLFW_KEY_A))
	//	{
	//		m_car->UpdateOrientation(deltaTime);
	//	}
	//	else if (window->KeyHold(GLFW_KEY_D))
	//	{
	//		m_car->UpdateOrientation(-deltaTime);
	//	}
	//}
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
