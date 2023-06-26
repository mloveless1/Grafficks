#include <Texture.h>
#include <stb_image.h>
#include <iostream>

Texture::Texture(const std::filesystem::path& path){
	// Load images using correct filepath
	auto texturePath = path.string();

	int width, height, numChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &numChannels, 4);

	// Gen and bind textures
	glGenTextures(1, &_textureHandle);
	glBindTexture(GL_TEXTURE_2D, _textureHandle);

	// IF the data exists or is not empty
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "Failed to load texture at path: " << texturePath << std::endl;
	}
	stbi_image_free(data);

}

void Texture::Bind(){
	
	glBindTexture(GL_TEXTURE_2D, _textureHandle);

}
