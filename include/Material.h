#pragma once
#include <shader.h>
#include <Texture.h>
#include <types.h>

class Material{
public:
	explicit Material(std::shared_ptr<Shader> shader);

	void AddTexture(const std::shared_ptr<Texture> texture);
	void Bind(const SceneParameters& sceneParameters, const glm::mat4& model);
	Shader* GetShader() { return _shader.get(); }


private:
	std::shared_ptr<Shader> _shader{ nullptr };
	std::vector<std::shared_ptr<Texture>> _textures;

};

