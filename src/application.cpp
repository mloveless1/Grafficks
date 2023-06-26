#include <application.h>
#include <iostream>
#include <glm/glm.hpp>
#include <types.h>
#include <vector>
#include <mesh.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <Pyramid.h>
#include <PointLight.h>
#include <Plane.h>
#include <Cube.h>
#include <Torus.h>
#include <BackBoard.h>
#include <Pole.h>
#include <Ball.h>
#include <Net.h>
#include <SoilPlane.h>
#include <Fence.h>


Application::Application(const char* WindowTitle, int width, int height) 
	: _applicationName{ WindowTitle },
	_width{ width }, _height{ height },
	_camera{ width, height, {0.f, 0.f, 3.f }, true},
	_sensitivity{ 0.1f } {}

void Application::Run() {
	// If the window can't open, close
	if (!openWindow()) {
		return;
	}
	setupInputs();
	_running = true;

	// Setup the scene
	setupScene();

	// Runs the application.
	while (_running) {
		if (glfwWindowShouldClose(_window)) {
			_running = false;
			continue;
		}


		// Update
		update();

		// Draw
		draw();

	}
	glfwTerminate();
}


bool Application::openWindow(){
	// Build our window here
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	_window = glfwCreateWindow(this->_width, this->_height, this->_applicationName, nullptr, nullptr);

	if (!_window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, (void*)this);
	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
		// This is called every time the window gets resized
		std::cout << "Window resized" << std::endl;
		glViewport(0, 0, width, height);
		auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		app->_width = width;
		app->_height = height;
		app->_camera.SetSize(width, height);
		});

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to intialize GLAD" << std::endl;
		glfwTerminate();
		return  false;
	}
	// see depth
	glEnable(GL_DEPTH_TEST);

	/*
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	*/
	return true;
}

void Application::setupScene() {

	
	//First light
	auto& light = _objects.emplace_back(std::make_unique<PointLight>());
	

	light->Transform = glm::translate(light->Transform, glm::vec3 (-3.2f, 1.5f, 0.f));
	light->Transform = glm::scale(light->Transform, glm::vec3(0.2f, 0.2f, 0.2f));

	auto* castLight = reinterpret_cast<PointLight*>(light.get());

	castLight->AmbientColor = { 0.2f, 0.2f, 0.2f };
	castLight->DiffuseColor = { 0.5f, 0.5f, 0.5f };
	castLight->SpecularColor = { 1.f, 1.f, 1.f };

	castLight->Constant = 1.f;
	castLight->Linear = 0.045f;
	castLight->Quadratic = 0.0075f;
	//Second light
	auto& light2 = _objects.emplace_back(std::make_unique<PointLight>());


	light2->Transform = glm::translate(light2->Transform, glm::vec3(3.2f, 1.5f, 0.f));
	light2->Transform = glm::scale(light2->Transform, glm::vec3(0.2f, 0.2f, 0.2f));

	auto* castLight2 = reinterpret_cast<PointLight*>(light2.get());

	castLight2->AmbientColor = { 0.2f, 0.2f, 0.2f };
	castLight2->DiffuseColor = { 0.5f, 0.5f, 0.5f };
	castLight2->SpecularColor = { 1.f, 1.f, 1.f };

	castLight2->Constant = 1.f;
	castLight2->Linear = 0.045f;
	castLight2->Quadratic = 0.0075f;
	
	//Pyramid on top of building (roof)
	auto& roof = _objects.emplace_back(std::make_unique<Pyramid>());
	roof->Transform = glm::translate(roof->Transform, glm::vec3(3.5f, 5.5f, -10.0f));
	roof->Transform = glm::scale(roof->Transform, glm::vec3(5.f, 5.f, 5.f));
	// Cube for building walls of background house
	auto& building = _objects.emplace_back(std::make_unique<Cube>());
	building->Transform = glm::translate(building->Transform, glm::vec3(3.5f, 0.49f, -10.0f));
	building->Transform = glm::scale(building->Transform, glm::vec3(5.f, 5.f, 5.f));

	// Plane
	auto& court = _objects.emplace_back(std::make_unique<Plane>());
	court->Transform = glm::translate(court->Transform, glm::vec3(0.0f, -1.5f, 0.0f));
	// Soil plane
	auto& soilPlane = _objects.emplace_back(std::make_unique<SoilPlane>());
	soilPlane->Transform = glm::translate(soilPlane->Transform, glm::vec3(0.0f, -1.52f, 0.0f));
	soilPlane->Transform = glm::scale(soilPlane->Transform, glm::vec3(5.f, 0.f, 5.f));


	/* Here builds the basketball hoop, the court, hoop*/
	// Rim or goal
	auto& goal = _objects.emplace_back(std::make_unique<Torus>());
	goal->Transform = glm::translate(goal->Transform, glm::vec3(0.f, 0.4f, -3.59f));
	goal->Transform = glm::scale(goal->Transform, glm::vec3(0.3f, 0.3f, 0.3f));
	goal->Transform = glm::rotate(goal->Transform, glm::radians(90.f), glm::vec3(1, 0, 0));
	// Hoop backboard
	auto& backboard = _objects.emplace_back(std::make_unique<Backboard>()); 
	backboard->Transform = glm::translate(backboard->Transform, glm::vec3(0.f, 0.8f, -3.95f));
	backboard->Transform = glm::scale(backboard->Transform, glm::vec3(1.99f, 1.5f, 0.02f));
	// Pole for hoop
	auto& pole = _objects.emplace_back(std::make_unique<Pole>());
	pole->Transform = glm::translate(pole->Transform, glm::vec3(0.f, -0.25f, -4.0f));
	pole->Transform = glm::scale(pole->Transform, glm::vec3(0.09f, 2.5f, 0.09f));

	// Basketballs
	auto& ball1 = _objects.emplace_back(std::make_unique<Ball>());
	ball1->Transform = ball1->Transform = glm::translate(ball1->Transform, glm::vec3(-0.2f, -1.40f, -4.f));
	ball1->Transform = glm::scale(ball1->Transform, glm::vec3(0.1f, 0.1f, 0.1f));

	auto& ball2 = _objects.emplace_back(std::make_unique<Ball>());
	ball2->Transform = ball2->Transform = glm::translate(ball2->Transform, glm::vec3(0.2f, -1.40f, -4.f));
	ball2->Transform = glm::scale(ball2->Transform, glm::vec3(0.1f, 0.1f, 0.1f));

	
	// Fence 
	auto& fence1 = _objects.emplace_back(std::make_unique<Fence>());
	fence1->Transform = glm::translate(fence1->Transform, glm::vec3(-0.123f, -0.05f, -5.15f));
	fence1->Transform = glm::scale(fence1->Transform, glm::vec3(11.3f, 3.4f, 0.03f));

	auto& fence2 = _objects.emplace_back(std::make_unique<Fence>());
	fence2->Transform = glm::rotate(fence2->Transform, glm::radians(90.f), glm::vec3(0, 1, 0));
	fence2->Transform = glm::translate(fence2->Transform, glm::vec3(0.4f, -0.05f, -5.78f));
	fence2->Transform = glm::scale(fence2->Transform, glm::vec3(9.5f, 3.4f, 0.03f));

	auto& fence3 = _objects.emplace_back(std::make_unique<Fence>());
	fence3->Transform = glm::rotate(fence3->Transform, glm::radians(-90.f), glm::vec3(0, 1, 0));
	fence3->Transform = glm::translate(fence3->Transform, glm::vec3(-0.4f, -0.05f, -5.5f));
	fence3->Transform = glm::scale(fence3->Transform, glm::vec3(9.5f, 3.4f, 0.03f));

	// TODO: Tapered cylinder for the net is coming out wrong
	// as a deformed cube, check draw calls and buffers
	auto& net = _objects.emplace_back(std::make_unique<Net>());

	// Ran out of time to finish the net.
}


bool Application::update(){
	glfwPollEvents();
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	handleInput();
	
	return false;
}

bool Application::draw(){
	// Give window its color
	glClearColor(0.f, 0.f, 0.f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	const glm::mat4 view = _camera.GetViewMatrix();
	const glm::mat4 projection = _camera.GetProjectionMatrix();

	SceneParameters sceneParams{
		.ProjectionMatrix = projection,
		.ViewMatrix = view,
		.CameraPosition = _camera.GetPosition(),
		.DirLight = {
			.Direction = glm::normalize(glm::vec3({ -0.2f, -0.2f, 1.f })), // Simulating the sun on the back right
			.AmbientColor = {0.2f, 0.2f, 0.1f},
			.DiffuseColor = {0.5f, 0.5f, 0.5f},
			.SpecularColor = {1.f, 1.f, 1.f}},
	};
	for (auto& model : _objects) {
		model->ProcessLighting(sceneParams);
	}

	for (auto& model : _objects) {
		model->Draw(sceneParams);
	}
	glfwSwapBuffers(_window);

	return true;
}

void Application::setupInputs() {
	glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));

		switch (key) {
			// Quit
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
				app->_running = false;
			break;
			// Switch between perspective and orthographic displays
			// Changes perspective bool to false or true based on _isPerspective variable
		case GLFW_KEY_P:
			if (action == GLFW_PRESS) {
				if (app->_camera.IsPerspective()) {
					app->_camera.SetIsPerspective(false);
				}
				else {
					app->_camera.SetIsPerspective(true);
				}
		default: {}
			}
		}

	});
	// Mouse controls 
	glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos) {
		 auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		app->mousePositionCallback(xpos, ypos);
		});
	
	//Scroll wheel
	glfwSetScrollCallback(_window, [](GLFWwindow* window, double xoffset, double yoffset) {
		auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		app->_moveSpeed += yoffset;
		});
}

void Application::handleInput() {
	const auto deltaTime = 0.0001f;

	auto moveAmount = _moveSpeed * deltaTime;

	if (glfwGetKey(_window, GLFW_KEY_W)) {
		_camera.MoveCamera(Camera::MoveDirection::Forward, moveAmount);
	}
	if (glfwGetKey(_window, GLFW_KEY_S)) {
		_camera.MoveCamera(Camera::MoveDirection::Backward, moveAmount);
	}
	if (glfwGetKey(_window, GLFW_KEY_A)) {
		_camera.MoveCamera(Camera::MoveDirection::Left, moveAmount);
	}
	if (glfwGetKey(_window, GLFW_KEY_D)) {
		_camera.MoveCamera(Camera::MoveDirection::Right, moveAmount);
	}
	if (glfwGetKey(_window, GLFW_KEY_Q)) {
		_camera.MoveCamera(Camera::MoveDirection::Up, moveAmount);
	}
	if (glfwGetKey(_window, GLFW_KEY_E)) {
		_camera.MoveCamera(Camera::MoveDirection::Down, moveAmount);
	}
}

void Application::mousePositionCallback(double xpos, double ypos) {
	// Track first mouse movement
	if (_firstMouse)
	{
		_lastMousePosition.x = xpos;
		_lastMousePosition.y = ypos;
		_firstMouse = false;
	}
	// Get mouse movement
	float xoffset = xpos - _lastMousePosition.x;
	float yoffset = _lastMousePosition.y - ypos;
	_lastMousePosition.x = xpos;
	_lastMousePosition.y = ypos;

	// move speed
	xoffset *= _sensitivity;
	yoffset *= _sensitivity;

	// ad offsets to pitch and yaw
	_camera._yaw += xoffset;
	_camera._pitch += yoffset;
	// Clamps pitch to 89 degrees both ways
	if (_camera._pitch > 89.0f)
		_camera._pitch = 89.0f;
	if (_camera._pitch < -89.0f)
		_camera._pitch = -89.0f;
	// Change the front and up direction vectors
	glm::vec3 direction {};
	direction.x = cos(glm::radians(_camera._yaw)) * cos(glm::radians(_camera._pitch));
	direction.y = sin(glm::radians(_camera._pitch));
	direction.z = sin(glm::radians(_camera._yaw)) * cos(glm::radians(_camera._pitch));
	_camera._lookDirection = glm::normalize(direction);


	//_camera.RotateBy(xoffset, yoffset); not using this anymore but saving it for later
}
