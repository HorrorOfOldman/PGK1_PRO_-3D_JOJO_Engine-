#pragma once
#include "includy.h"
#include "GameObject.h"
#include "const.h"

#define M_PI 3.14

/**
* @class Observer
* @brief Klasa będąca kamerą w naszym programie
*/
class Observer
{
public:
	glm::mat4 getViewMatrix() 
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
	
	//sterowanie kamerą
	void processKeyboard(int key) 
	{
		switch (key) {
		case 'w': cameraPos += CAMERA_SPEED * cameraFront; break;
		case 's': cameraPos -= CAMERA_SPEED * cameraFront; break;
		case 'a': cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * CAMERA_SPEED; break;
		case 'd': cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * CAMERA_SPEED; break;
		case 'q': cameraPos += CAMERA_SPEED * cameraUp; break; 
		case 'e': cameraPos -= CAMERA_SPEED * cameraUp; break;
		case 27: exit(0); break;
		default: cout << "Nacisnieto klawisz " << (char)key << " kod " << (int)key << "\n";	break;
		}
	}

	//obsługa myszki
	void processMouse(float xoffset, float yoffset) 
	{
		xoffset *= MOUSE_SENSITIVITY;
		yoffset *= MOUSE_SENSITIVITY;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}

};