#include <Backboard.h>
#include <types.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

Backboard::Backboard() {
	createShaders();
	createModels();
	Ambient = { 0.5f, 0.5f, 0.5f };
	Diffuse = { 0.9f, 1.f, 0.8f };
	Specular = { 0.48f, 0.48f, 0.48f };
	Shine = 32;
}

void Backboard::Init() {

}

void Backboard::Update(float deltaTime) {
	// Transform = 
}


void Backboard::createShaders() {
	// Creates a shader object and assigns it to our model
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";

	_basicLitShader = std::make_shared<Shader>(shaderPath / "basic_lit2.vert", shaderPath / "basic_lit2.frag");
}

void Backboard::createModels() {
	auto texturePath = std::filesystem::current_path() / "assets" / "textures";

	auto backboardMesh = std::make_shared<Mesh>(Shapes::cubeVertices, Shapes::cubeElements, 0);

	auto backboardMaterial = std::make_shared<Material>(_basicLitShader);

	auto backboardTexture1 = std::make_shared<Texture>(texturePath / "backboard.jpg");
	auto backboardTexture2 = std::make_shared<Texture>(texturePath / "dirty-metal-texture.jpg");

	backboardMaterial->AddTexture(backboardTexture1);
	backboardMaterial->AddTexture(backboardTexture2);

	_models.emplace_back(backboardMesh, backboardMaterial);
}

void Backboard::Draw(const SceneParameters& sceneParameters) {
	Shader* lastBoundShader = nullptr;
	for (auto& model : _models) {
		auto* shader = model.GetMaterial()->GetShader();
		auto* mesh = model.GetMesh();
		if (shader != lastBoundShader) {
			model.GetMaterial()->Bind(sceneParameters, Transform * mesh->Transform);
			// Take care of lights
			for (auto i = 0; i < MAX_POINT_LIGHTS; i++) {
				std::string baseUniformName = "pointLights[";
				baseUniformName += std::to_string(i) + "]";
				PointLightStruct pointLight = i < sceneParameters.Lights.size() ? sceneParameters.Lights[i] : PointLightStruct{};
				shader->SetVec3(baseUniformName + ".Position", pointLight.Position);
				shader->SetVec3(baseUniformName + ".Ambient", pointLight.AmbientColor);
				shader->SetVec3(baseUniformName + ".Diffuse", pointLight.DiffuseColor);
				shader->SetVec3(baseUniformName + ".Specular", pointLight.SpecularColor);

				shader->SetFloat(baseUniformName + ".Constant", pointLight.Constant);
				shader->SetFloat(baseUniformName + ".Linear", pointLight.Linear);
				shader->SetFloat(baseUniformName + ".Quadratic", pointLight.Quadratic);
			}

			shader->SetVec3("dirLight.Direction", sceneParameters.DirLight.Direction);
			shader->SetVec3("dirLight.Ambient", sceneParameters.DirLight.AmbientColor);
			shader->SetVec3("dirLight.Diffuse", sceneParameters.DirLight.DiffuseColor);
			shader->SetVec3("dirLight.Specular", sceneParameters.DirLight.SpecularColor);

			shader->SetVec3("material.Ambient", Ambient);
			shader->SetVec3("material.Diffuse", Diffuse);
			shader->SetVec3("material.Specular", Specular);
			shader->SetFloat("material.Shininess", Shine);
		}
		// shader->SetMat4("model", Transform * mesh->Transform);
		mesh->Draw();
	}

}

void Backboard::ProcessLighting(SceneParameters& sceneParameters)
{
	return;
}
