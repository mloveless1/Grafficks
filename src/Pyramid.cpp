#include <Pyramid.h>
#include <types.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

Pyramid::Pyramid() {
	createShaders();
	createModels();

	Ambient = { 0.2f, 0.2f, 0.1f };
	Diffuse = { 0.9f, 1.f, 0.8f };
	Specular = { 0.01f, 0.01f, 0.01f };
	Shine = 32;
}

void Pyramid::Init() {

}

void Pyramid::Update(float deltaTime) {
	// Transform = 
}


void Pyramid::createShaders() {
	// Creates a shader object and assigns it to our model
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";

	_basicLitShader = std::make_shared<Shader>(shaderPath / "basic_lit.vert", shaderPath / "basic_lit.frag");
}

void Pyramid::createModels() {
	auto texturePath = std::filesystem::current_path() / "assets" / "textures";

	auto pyramidMesh = std::make_shared<Mesh>(Shapes::pyramidVertices, Shapes::pyramidElements, 0);

	auto pyramidMaterial = std::make_shared<Material>(_basicLitShader);

	auto pyramidTexture = std::make_shared<Texture>(texturePath / "shingles.jpg");

	pyramidMaterial->AddTexture(pyramidTexture);

	_models.emplace_back(pyramidMesh, pyramidMaterial);
}

void Pyramid::Draw(const SceneParameters& sceneParameters) {
	Shader* lastBoundShader = nullptr;
	for (auto& model : _models) {
		auto* shader = model.GetMaterial()->GetShader();
		auto* mesh = model.GetMesh();
		if (shader != lastBoundShader) {
			/*
			//Set projection and view matrix
			shader->Bind();
			shader->SetMat4("projection", sceneParameters.ProjectionMatrix);
			shader->SetMat4("view", sceneParameters.ViewMatrix);
			// Set eye position
			shader->SetVec3("eyePos", sceneParameters.CameraPosition);

			*/
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

void Pyramid::ProcessLighting(SceneParameters& sceneParameters)
{
	return;
}
