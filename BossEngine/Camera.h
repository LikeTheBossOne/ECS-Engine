#pragma once
#include <glad/glad.h>
#include <glm/mat4x2.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Enums.h"

class BOSSENGINE_API Camera
{
public:
    constexpr static float DEFAULT_YAW = -90.f;
    constexpr static float DEFAULT_PITCH = 0.0f;
    constexpr static float DEFAULT_SENSITIVITY = 0.1f;
    constexpr static float DEFAULT_FOV = 45.0f;

    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;

    float _yaw;
    float _pitch;
    float _mouseSensitivity;
    float _fov;

    Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
        float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH) :
        _front(glm::vec3(0.0f, 0.0f, -1.0f)), _mouseSensitivity(DEFAULT_SENSITIVITY), _fov(DEFAULT_FOV)
    {
        _position = position;
        _worldUp = up;
        _yaw = yaw;
        _pitch = pitch;
        UpdateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
        _front(glm::vec3(0.0f, 0.0f, -1.0f)), _mouseSensitivity(DEFAULT_SENSITIVITY), _fov(DEFAULT_FOV)
    {
        _position = glm::vec3(posX, posY, posZ);
        _worldUp = glm::vec3(upX, upY, upZ);
        _yaw = yaw;
        _pitch = pitch;
        UpdateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        // Own View Matrix calculation
        return glm::lookAt(_position, _position + _front, _up);
    }

    void ProcessKeyBoard(Camera_Movement direction, float delta, float velocity)
    {
        if (direction == Camera_Movement::FORWARD)
        {
            _position += _front * velocity;
        }
        if (direction == Camera_Movement::BACKWARD)
        {
            _position -= _front * velocity;
        }
        if (direction == Camera_Movement::LEFT)
        {
            _position -= _right * velocity;
        }
        if (direction == Camera_Movement::RIGHT)
        {
            _position += _right * velocity;
        }
    }

    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
    {
        xOffset *= _mouseSensitivity;
        yOffset *= _mouseSensitivity;

        _yaw += xOffset;
        _pitch += yOffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (_pitch > 89.0f)
            {
                _pitch = 89.0f;
            }
            if (_pitch < -89.0f)
            {
                _pitch = -89.0f;
            }
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        UpdateCameraVectors();
    }

    void ProcessMouseScroll(float yOffset)
    {
        _fov -= static_cast<float>(yOffset);
        if (_fov < 1.0f)
        {
            _fov = 1.0f;
        }
        if (_fov > 45.0f)
        {
            _fov = 45.0f;
        }
    }

private:
    void UpdateCameraVectors()
    {
        // Calc front vector
        glm::vec3 front;
        front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        front.y = sin(glm::radians(_pitch));
        front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        _right = glm::normalize(glm::cross(_front, _worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        _up = glm::normalize(glm::cross(_right, _front));
    }
};
