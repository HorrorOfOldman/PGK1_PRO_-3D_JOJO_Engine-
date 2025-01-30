#pragma once

/**
* @brief Przechowuje sta³e naszego programu jak i zmienne globalne
*/

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float CAMERA_SPEED = 0.1f;
const float MOUSE_SENSITIVITY = 0.1f;
const float CUBE_ROTATION_SPEED = 1.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f, pitch = 0.0f;
float lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
bool firstMouse = true;

glm::mat4 cubeRotation = glm::mat4(1.0f);


