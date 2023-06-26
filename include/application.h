#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <mesh.h>
#include <shader.h>
#include <camera.h>
#include <Texture.h>
#include <ObjectWrap.h>

class Application {

public:
	Application(const char* WindowTitle, int width, int height);
	void Run();
	float _moveSpeed{ 10.f };
	std::vector<glm::vec3> _torusVertices;


private:
	
	float _sensitivity{};
	
	float _zoom{};
	Camera _camera;
	const char* _applicationName;
	int _width{};
	int _height{};
	GLFWwindow* _window{ nullptr };

	std::vector<Mesh> _meshes;
	std::vector<Texture> _textures;

	bool _running{ false };
	Shader _shader {};

	void setupScene();
	bool openWindow();
	void setupInputs();
	bool update();
	bool draw();

	void handleInput();
	void mousePositionCallback(double xpos, double ypos);
	bool _firstMouse{ true };
	glm::vec2 _lastMousePosition{};
	glm::vec2 _cameraLookSpeed{};

	float _ambientStrength{ 0.f };
	glm::vec3 _ambientLightcolor{ 1.f, 1.f, 1.f };
	std::vector<std::unique_ptr<ObjectWrap>> _objects;
};