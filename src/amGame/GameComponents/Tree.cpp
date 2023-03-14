#include "Tree.hpp"

Tree::Tree(
	const Shader* const shader,
	CustomCamera* const camera,
	std::shared_ptr<Texture2D>& crownTexture,
	std::shared_ptr<Texture2D>& trunkTexture,
	std::string meshPath,
	std::string trunkMeshName,
	std::string crownMeshName)
	: GameComponent()
{
	m_objects.emplace_back(new GeometryObject3d(shader, camera, meshPath, trunkMeshName));
	m_objects.emplace_back(new GeometryObject3d(shader, camera, meshPath, crownMeshName));
	m_objects.emplace_back(new GeometryObject3d(shader, camera, meshPath, crownMeshName));

	m_objects[0]->ComputeInitialBBox();
	m_objects[1]->ComputeInitialBBox();
	m_objects[2]->ComputeInitialBBox();

	m_crownTexture = crownTexture;
	m_trunkTexture = trunkTexture;
}

Tree::Tree(
	const Shader* const shader,
	CustomCamera* const camera,
	const WindowObject* window,
	std::shared_ptr<Texture2D>& crownTexture,
	std::shared_ptr<Texture2D>& trunkTexture)
	: Tree(
		shader,
		camera,
		crownTexture,
		trunkTexture,
		PATH_JOIN(window->props.selfDir, "assets", "models", "primitives"),
		"box.obj",
		"cone.obj")
{
}

void Tree::Render()
{
	m_modelMatrix = glm::mat4(1);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position + glm::vec3{0, 0.7f, 0});
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{m_scale, m_scale, m_scale});

	glm::mat4 trunkModelMatrix = glm::scale(m_modelMatrix, glm::vec3{0.5f, 1.5f, 0.5f});
	m_objects[0]->Render(trunkModelMatrix, m_trunkTexture.get());
	m_bbox = m_objects[0]->GetTranformedBBox(trunkModelMatrix);

	glm::mat4 crownModelMatrix = glm::translate(m_modelMatrix, glm::vec3{0, 1.5f, 0});
	m_objects[1]->Render(crownModelMatrix, m_crownTexture.get());
	m_bbox += m_objects[1]->GetTranformedBBox(crownModelMatrix);

	crownModelMatrix = glm::translate(crownModelMatrix, glm::vec3{0, 1.f, 0});
	crownModelMatrix = glm::scale(crownModelMatrix, glm::vec3{0.7f, 0.7f, 0.7f});
	m_objects[2]->Render(crownModelMatrix, m_crownTexture.get());
	m_bbox += m_objects[2]->GetTranformedBBox(crownModelMatrix);
}

void Tree::Render(
	const glm::vec3& eyePosition,
	const std::vector<const class LightSourceAdapter*>& lightingComponents)
{
	m_modelMatrix = glm::mat4(1);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position + glm::vec3{0, 0.7f, 0});
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3{m_scale, m_scale, m_scale});

	glm::mat4 trunkModelMatrix = glm::scale(m_modelMatrix, glm::vec3{0.5f, 1.5f, 0.5f});
	m_objects[0]->Render(trunkModelMatrix, m_trunkTexture.get(), eyePosition, lightingComponents);
	m_bbox = m_objects[0]->GetTranformedBBox(trunkModelMatrix);

	glm::mat4 crownModelMatrix = glm::translate(m_modelMatrix, glm::vec3{0, 1.5f, 0});
	m_objects[1]->Render(crownModelMatrix, m_crownTexture.get(), eyePosition, lightingComponents);
	m_bbox += m_objects[1]->GetTranformedBBox(crownModelMatrix);

	crownModelMatrix = glm::translate(crownModelMatrix, glm::vec3{0, 1.f, 0});
	crownModelMatrix = glm::scale(crownModelMatrix, glm::vec3{0.7f, 0.7f, 0.7f});
	m_objects[2]->Render(crownModelMatrix, m_crownTexture.get(), eyePosition, lightingComponents);
	m_bbox += m_objects[2]->GetTranformedBBox(crownModelMatrix);
}

void Tree::InstantiateLightSource()
{
	m_lightSource =
		std::make_unique<PointLight>(m_position + glm::vec3{0, 2, 0}, Colors::MakeRandomRGB(), 1.f);
}

void Tree::Activate(const Field* field)
{
	GameComponent::Activate(field);

	m_lightSource->SetPosition(m_position + glm::vec3{0, 2, 0});
}
