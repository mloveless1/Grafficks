#include <PointLight.h>
#include <types.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

PointLight::PointLight() {
	createShaders();
	createModels();
}

void PointLight::Init() {

}

void PointLight::Update(float deltaTime) {
	// Transform = 
}


void PointLight::createShaders() {
	// Creates a shader object and assigns it to our model
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";

	_basicLitShader = std::make_shared<Shader>(shaderPath / "basic_unlit_color.vert", shaderPath / "basic_unlit_color.frag");
}

void PointLight::createModels() {
	// auto texturePath = std::filesystem::current_path() / "assets" / "textures";

	auto cubeMesh = std::make_shared<Mesh>(Shapes::cubeVertices, Shapes::cubeElements, glm::vec3{ 1.f, 1.f, 1.f }, 0);

	auto cubeMaterial = std::make_shared<Material>(_basicLitShader);


	_models.emplace_back(cubeMesh, cubeMaterial);
}

void PointLight::Draw(const SceneParameters& sceneParameters) {

	for (auto& model : _models) {
		auto* shader = model.GetMaterial()->GetShader();
		auto* mesh = model.GetMesh();
		shader->Bind();
		shader->SetMat4("projection", sceneParameters.ProjectionMatrix);
		shader->SetMat4("view", sceneParameters.ViewMatrix);
		shader->SetMat4("model", Transform * mesh->Transform);
		mesh->Draw();
	}

}

void PointLight::ProcessLighting(SceneParameters& sceneParameters){
	if (sceneParameters.Lights.size() < MAX_POINT_LIGHTS) {
		PointLightStruct pointLight{
			.Position = glm::vec3(Transform[3]),
			.AmbientColor = AmbientColor,
			.DiffuseColor = DiffuseColor,
			.SpecularColor = SpecularColor,
			.Constant = Constant,
			.Linear = Linear,
			.Quadratic = Quadratic
		};
		sceneParameters.Lights.emplace_back(pointLight);
	}
}
