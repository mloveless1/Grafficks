#include <Ball.h>
#include <types.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

Ball::Ball() {
	createShaders();
	createModels();
	Ambient = { 0.2f, 0.2f, 0.1f };
	Diffuse = { 0.9f, 1.f, 0.8f };
	Specular = { 0.01f, 0.01f, 0.01f };
	Shine = 1.5;
}

void Ball::Init() {

}

void Ball::Update(float deltaTime) {
	
	// Transform = 
}


void Ball::createShaders() {
	// Creates a shader object and assigns it to our model
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";

	_basicLitShader = std::make_shared<Shader>(shaderPath / "basic_lit.vert", shaderPath / "basic_lit.frag");
}

void Ball::createModels() {
	auto texturePath = std::filesystem::current_path() / "assets" / "textures";

	auto BallMesh = GenBallMesh(1.0f, 18, 36);

	auto BallMaterial = std::make_shared<Material>(_basicLitShader);

	// Found this texture online being shared in a public forum 
	// Credit to DownDate: https://opengameart.org/content/basket-ball-texture
	auto BallTexture = std::make_shared<Texture>(texturePath / "balldimpled.png");

	BallMaterial->AddTexture(BallTexture);

	_models.emplace_back(BallMesh, BallMaterial);
}

void Ball::Draw(const SceneParameters& sceneParameters) {
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

void Ball::ProcessLighting(SceneParameters& sceneParameters)
{
	return;
}

std::shared_ptr<Mesh> Ball::GenBallMesh(float radius, uint32_t stacks, uint32_t sectors) {
	std::vector<Vertex> vertices{};
	std::vector<uint32_t> indices{};

	float x, y, z, xy;
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;

	float sectorStep = glm::two_pi<float>() / sectors;
	float stackStep = glm::pi<float>() / stacks;
	float sectorAngle, stackAngle;

	for (uint32_t i = 0; i <= stacks; ++i)
	{
		stackAngle = glm::half_pi<float>() - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * glm::cos(stackAngle);             // r * cos(u)
		z = radius * glm::sin(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// first and last vertices have same position and normal, but different tex coords
		for (uint32_t j = 0; j <= sectors; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * glm::cos(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * glm::sin(sectorAngle);             // r * cos(u) * sin(v)

			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / static_cast<float>(sectors);
			t = (float)i / static_cast<float>(stacks);

			vertices.push_back({
				.Position = { x, y, z },
				.Normal = { nx, ny, nz },
				.Uv = { s, t }
				});
		}
	}

	uint32_t k1, k2;
	for (uint32_t i = 0; i < stacks; ++i)
	{
		k1 = i * (sectors + 1);     // beginning of current stack
		k2 = k1 + sectors + 1;      // beginning of next stack

		for (uint32_t j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k1 + 1);
				indices.push_back(k2);
			}

			// k1+1 => k2 => k2+1
			if (i != (stacks - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2 + 1);
				indices.push_back(k2);
			}
		}
	}
	return std::make_shared<Mesh>(vertices, indices, 3);
}