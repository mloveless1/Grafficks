#pragma once
#include <ObjectWrap.h>
class PointLight : public ObjectWrap{
public:
	PointLight();
	void Init() override;
	void Update(float deltaTime) override;
	void Draw(const SceneParameters& sceneParameters) override;
	void ProcessLighting(SceneParameters& sceneParameters) override;
public:
	glm::vec3 AmbientColor{};
	glm::vec3 DiffuseColor{};
	glm::vec3 SpecularColor{};

	float Constant{ 0.f };
	float Linear{ 0.f };
	float Quadratic{ 0.f };

private:
	void createShaders();
	void createModels();
	std::shared_ptr<Shader> _basicLitShader;
	std::shared_ptr<Shader> _textureShader;
	std::vector<Model> _models;
};

