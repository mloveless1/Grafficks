#pragma once
#include <ObjectWrap.h>
#include <Model.h>

class Ball : public ObjectWrap {
public:
	Ball();
	void Init() override;
	void Update(float deltaTime) override;
	void Draw(const SceneParameters& sceneParameters);
	void ProcessLighting(SceneParameters& sceneParameters) override;
	float shine;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shine;
private:
	void createShaders();
	void createModels();
	std::shared_ptr<Shader> _basicLitShader;
	std::shared_ptr<Shader> _textureShader;
	std::vector<Model> _models;
	std::shared_ptr<Mesh> GenBallMesh(float radius, uint32_t stacks, uint32_t sectors);
};

