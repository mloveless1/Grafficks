#include <Torus.h>
#include <types.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

Torus::Torus() {
	createShaders();
	createModels();

	Ambient = { 0.2f, 0.2f, 0.1f };
	Diffuse = { 0.9f, 1.f, 0.8f };
	Specular = { 0.9f, 1.f, 0.8f };
	Shine = 32;
}

void Torus::Init() {

}

void Torus::Update(float deltaTime) {
	// Transform = 
}


void Torus::createShaders() {
	// Creates a shader object and assigns it to our model
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";

	_basicLitShader = std::make_shared<Shader>(shaderPath / "basic_lit.vert", shaderPath / "basic_lit.frag");
}

void Torus::createModels() {
	auto texturePath = std::filesystem::current_path() / "assets" / "textures";

	auto torusMesh = GenTorusMesh();

	auto torusMaterial = std::make_shared<Material>(_basicLitShader);

	auto torusTexture = std::make_shared<Texture>(texturePath / "dirty-metal-texture.jpg");

	torusMaterial->AddTexture(torusTexture);

	_models.emplace_back(torusMesh, torusMaterial);
}

void Torus::Draw(const SceneParameters& sceneParameters) {
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

void Torus::ProcessLighting(SceneParameters& sceneParameters)
{
	return;
}

std::shared_ptr<Mesh> Torus::GenTorusMesh(){
	std::vector<Vertex> vertices{};
	std::vector<uint32_t> indices{};

	int _mainSegments = 30;
	int _tubeSegments = 30;
	float _mainRadius = 1.0f;
	float _tubeRadius = .1f;

	auto mainSegmentAngleStep = glm::radians(360.0f / float(_mainSegments));
	auto tubeSegmentAngleStep = glm::radians(360.0f / float(_tubeSegments));

	std::vector<glm::vec3> vertex_list;
	std::vector<std::vector<glm::vec3>> segments_list;
	std::vector<glm::vec3> normals_list;
	std::vector<glm::vec2> texture_coords;
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 normal;
	glm::vec3 vertex;
	glm::vec2 text_coord;

	// generate the torus vertices
	auto currentMainSegmentAngle = 0.0f;
	for (auto i = 0; i < _mainSegments; i++)
	{
		// Calculate sine and cosine of main segment angle
		auto sinMainSegment = sin(currentMainSegmentAngle);
		auto cosMainSegment = cos(currentMainSegmentAngle);
		auto currentTubeSegmentAngle = 0.0f;
		std::vector<glm::vec3> segment_points;
		for (auto j = 0; j < _tubeSegments; j++)
		{
			// Calculate sine and cosine of tube segment angle
			auto sinTubeSegment = sin(currentTubeSegmentAngle);
			auto cosTubeSegment = cos(currentTubeSegmentAngle);

			// Calculate vertex position on the surface of torus
			auto surfacePosition = glm::vec3(
				(_mainRadius + _tubeRadius * cosTubeSegment) * cosMainSegment,
				(_mainRadius + _tubeRadius * cosTubeSegment) * sinMainSegment,
				_tubeRadius * sinTubeSegment);

			//vertex_list.push_back(surfacePosition);
			segment_points.push_back(surfacePosition);

			// Update current tube angle
			currentTubeSegmentAngle += tubeSegmentAngleStep;
		}
		segments_list.push_back(segment_points);
		segment_points.clear();

		// Update main segment angle
		currentMainSegmentAngle += mainSegmentAngleStep;
	}

	float horizontalStep = 1.0 / _mainSegments;
	float verticalStep = 1.0 / _tubeSegments;
	float u = 0.0;
	float v = 0.0;

	// connect the various segments together, forming triangles
	for (int i = 0; i < _mainSegments; i++)
	{
		for (int j = 0; j < _tubeSegments; j++)
		{
			if (((i + 1) < _mainSegments) && ((j + 1) < _tubeSegments))
			{
				vertex_list.push_back(segments_list[i][j]);
				normal = normalize(segments_list[i][j] - center);
				normals_list.push_back(normal);
				texture_coords.push_back(glm::vec2(u, v));
				vertex_list.push_back(segments_list[i][j + 1]);
				normal = normalize(segments_list[i][j + 1] - center);
				normals_list.push_back(normal);
				texture_coords.push_back(glm::vec2(u, v + verticalStep));
				vertex_list.push_back(segments_list[i + 1][j + 1]);
				normal = normalize(segments_list[i + 1][j + 1] - center);
				normals_list.push_back(normal);
				texture_coords.push_back(glm::vec2(u + horizontalStep, v + verticalStep));
				vertex_list.push_back(segments_list[i][j]);
				normal = normalize(segments_list[i][j] - center);
				normals_list.push_back(normal);
				texture_coords.push_back(glm::vec2(u, v));
				vertex_list.push_back(segments_list[i + 1][j]);
				normal = normalize(segments_list[i + 1][j] - center);
				normals_list.push_back(normal);
				texture_coords.push_back(glm::vec2(u + horizontalStep, v));
				vertex_list.push_back(segments_list[i + 1][j + 1]);
				normal = normalize(segments_list[i + 1][j + 1] - center);
				normals_list.push_back(normal);
				texture_coords.push_back(glm::vec2(u + horizontalStep, v - verticalStep));
				vertex_list.push_back(segments_list[i][j]);
				normal = normalize(segments_list[i][j] - center);
				normals_list.push_back(normal);
				texture_coords.push_back(glm::vec2(u, v));
			}
			else
			{
				if (((i + 1) == _mainSegments) && ((j + 1) == _tubeSegments))
				{
					vertex_list.push_back(segments_list[i][j]);
					normal = normalize(segments_list[i][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][0]);
					normal = normalize(segments_list[i][0] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, 0));
					vertex_list.push_back(segments_list[0][0]);
					normal = normalize(segments_list[0][0] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(0, 0));
					vertex_list.push_back(segments_list[i][j]);
					normal = normalize(segments_list[i][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[0][j]);
					normal = normalize(segments_list[0][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(0, v));
					vertex_list.push_back(segments_list[0][0]);
					normal = normalize(segments_list[0][0] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(0, 0));
					vertex_list.push_back(segments_list[i][j]);
					normal = normalize(segments_list[i][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, v));
				}
				else if ((i + 1) == _mainSegments)
				{
					vertex_list.push_back(segments_list[i][j]);
					normal = normalize(segments_list[i][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][j + 1]);
					normal = normalize(segments_list[i][j + 1] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, v + verticalStep));
					vertex_list.push_back(segments_list[0][j + 1]);
					normal = normalize(segments_list[0][j + 1] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(0, v + verticalStep));
					vertex_list.push_back(segments_list[i][j]);
					normal = normalize(segments_list[i][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[0][j]);
					normal = normalize(segments_list[0][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(0, v));
					vertex_list.push_back(segments_list[0][j + 1]);
					normal = normalize(segments_list[0][j + 1] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(0, v + verticalStep));
					vertex_list.push_back(segments_list[i][j]);
					normal = normalize(segments_list[i][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, v));
				}
				else if ((j + 1) == _tubeSegments)
				{
					vertex_list.push_back(segments_list[i][j]);
					normal = normalize(segments_list[i][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][0]);
					normal = normalize(segments_list[i][0] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, 0));
					vertex_list.push_back(segments_list[i + 1][0]);
					normal = normalize(segments_list[i + 1][0] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u + horizontalStep, 0));
					vertex_list.push_back(segments_list[i][j]);
					normal = normalize(segments_list[i][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i + 1][j]);
					normal = normalize(segments_list[i + 1][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u + horizontalStep, v));
					vertex_list.push_back(segments_list[i + 1][0]);
					normal = normalize(segments_list[i + 1][0] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u + horizontalStep, 0));
					vertex_list.push_back(segments_list[i][j]);
					normal = normalize(segments_list[i][j] - center);
					normals_list.push_back(normal);
					texture_coords.push_back(glm::vec2(u, v));
				}

			}
			v += verticalStep;
		}
		v = 0.0;
		u += horizontalStep;
	}

	for (int i = 0; i < vertex_list.size(); i++) {
		vertices.push_back({
			.Position = {vertex_list[i].x, vertex_list[i].y, vertex_list[i].z},
			//.Color = {0.9f, 0.6f, 0.1f},
			.Normal = {normals_list[i].x, normals_list[i].y, normals_list[i].z},
			.Uv = {texture_coords[i].x, texture_coords[i].y}
			});
	}
	GLuint size = vertex_list.size();
	return std::make_shared<Mesh>(vertices, size, 1);
}