#include <SoilPlane.h>
#include <types.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

SoilPlane::SoilPlane() {
	createShaders();
	createModels();

	Ambient = { 0.2f, 0.2f, 0.1f };
	Diffuse = { 0.9f, 1.f, 0.8f };
	Specular = { 0.9f, 1.f, 0.8f };
	Shine = 32;
}

void SoilPlane::Init() {

}

void SoilPlane::Update(float deltaTime) {
	// Transform = 
}


void SoilPlane::createShaders() {
	// Creates a shader object and assigns it to our model
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";

	_basicLitShader = std::make_shared<Shader>(shaderPath / "basic_lit.vert", shaderPath / "basic_lit.frag");
}

void SoilPlane::createModels() {
	auto texturePath = std::filesystem::current_path() / "assets" / "textures";

	auto soilPlaneMesh = std::make_shared<Mesh>(Shapes::planeVertices, Shapes::planeElements, 0);

	auto soilPlaneMaterial = std::make_shared<Material>(_basicLitShader);

	auto soilPlaneTexture = std::make_shared<Texture>(texturePath / "topsoil.jpg");

	soilPlaneMaterial->AddTexture(soilPlaneTexture);
	_models.emplace_back(soilPlaneMesh, soilPlaneMaterial);
}

void SoilPlane::Draw(const SceneParameters& sceneParameters) {
	Shader* lastBoundShader = nullptr;
	for (auto& model : _models) {
		auto* shader = model.GetMaterial()->GetShader();
		auto* mesh = model.GetMesh();
		if (shader != lastBoundShader) {
			// Bind appropriate textures and shaders
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
		mesh->Draw();
	}

}

void SoilPlane::ProcessLighting(SceneParameters& sceneParameters)
{
	// Don't need this to do anything
	return;
}
