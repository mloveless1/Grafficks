#include <camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <algorithm>

Camera::Camera(int width, int height, glm::vec3 initialPosition, bool isPerspective)
    : _isPerspective{ isPerspective },
    _position { initialPosition },
    _lookDirection{ 0.5f, 0.f, 2.f },
    _width { width }, _height { height } {

    recalcVectors();
}



glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(_position, _position + _lookDirection, _upDirection);
}

glm::mat4 Camera::GetProjectionMatrix() const{
    auto aspectRatio = (float)_width / (float)_height;
    if (_isPerspective) {
        return glm::perspective(glm::radians(_fieldOfView), aspectRatio, _nearClip, _farClip);
    }
    // Ortho view for switch
    return glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f, _nearClip, _farClip);
}

void Camera::MoveCamera(MoveDirection direction, float moveAmount){
    glm::vec3 moveDirection;

    switch (direction) {
        //Forward
    case MoveDirection::Forward: {
       _position +=  moveAmount * _lookDirection;
        break;
    }
        //Backward
    case MoveDirection::Backward: {
        _position -= moveAmount * _lookDirection;
        break;
    }
          // Left
    case MoveDirection::Left: {
        _position -= glm::normalize(glm::cross(_lookDirection, _upDirection)) * moveAmount;
        break;
    }
          // Right
    case MoveDirection::Right:
        _position += glm::normalize(glm::cross(_lookDirection, _upDirection)) * moveAmount;
        break;
        // Up
    case MoveDirection::Up: {
        _position += moveAmount * _upDirection;
        break;
    }
          // Down
    case MoveDirection::Down: {
        _position -= moveAmount * _upDirection;
        break;
    }

    }

}

void Camera::RotateBy(float yaw, float pitch){
    _yaw += yaw;
    _pitch += pitch;

    // Make sure pitch is no less or more than 89 deg
    _pitch = std::clamp(_pitch, 89.f, 89.f);
    recalcVectors();
}

void Camera::recalcVectors() {
    glm::vec3 front{};
    front.x = front.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
    front.y = std::sin(glm::radians(_pitch));
    front.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));

    _lookDirection = glm::normalize(front);
  
    glm::vec3 rightDirection = glm::normalize(glm::cross(_lookDirection, glm::vec3(0.f, 1.f, 0.f)));
    _upDirection = glm::normalize(glm::cross(rightDirection, _lookDirection));
}


