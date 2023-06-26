#include <Material.h>

Material::Material(std::shared_ptr<Shader> shader){
	_shader = shader;
}

void Material::AddTexture(const std::shared_ptr<Texture> texture){

	_textures.push_back(texture);
}

void Material::Bind(const SceneParameters& sceneParameters, const glm::mat4& model){
	_shader->Bind();
	_shader->SetMat4("projection", sceneParameters.ProjectionMatrix);
	_shader->SetMat4("view", sceneParameters.ViewMatrix);

	//Cam position
	_shader->SetVec3("eyePos", sceneParameters.CameraPosition);
	_shader->SetMat4("model", model);

	for (auto i = 0; i < _textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		_textures[i]->Bind();
		std::string baseUniformName = "tex";
		baseUniformName += std::to_string(i);
		_shader->SetInt(baseUniformName, i);
	}

}
