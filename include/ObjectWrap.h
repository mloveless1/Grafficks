#pragma once
#include <types.h>
#include <Model.h>
class ObjectWrap{

public:
	~ObjectWrap() = default;
	virtual void Init() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(const SceneParameters& sceneParameters) = 0;
	virtual void ProcessLighting(SceneParameters& sceneParameters) = 0;
	glm::mat4 Transform{ 1.f };
	std::vector<Model> _models;

};



