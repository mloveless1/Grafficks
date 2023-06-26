#pragma once

#include <filesystem>
#include <glad/glad.h>
class Texture{
public:
	Texture();
	explicit Texture(const std::filesystem::path& path);
	void Bind();

private: 
	GLuint _textureHandle;

};

