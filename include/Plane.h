#pragma once
#include <ObjectWrap.h>
#include <Model.h>

class Plane : public ObjectWrap {
public:
	Plane();
	void Init() override;
	void Update(float deltaTime) override;
	void Draw(const SceneParameters& sceneParameters) override;
	void ProcessLighting(SceneParameters& sceneParameters) override;
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

};

