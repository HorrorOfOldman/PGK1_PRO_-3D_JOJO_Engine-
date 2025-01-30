#ifndef LIGHT_H
#define LIGHT_H

#include "includy.h"

/**
* @class Light
* @brief Klasa odpowiadaj¹ca za oœwietlenie w programie
*/
class Light 
{
public:
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
		: position(pos), ambient(amb), diffuse(diff), specular(spec) {}

	void setupLight(GLenum lightID) 
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		// Set light position
		GLfloat lightPosition[] = { position.x, position.y, position.z, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		// Set light properties
		GLfloat lightAmbient[] = { ambient.x, ambient.y, ambient.z, 1.0f };
		GLfloat lightDiffuse[] = { diffuse.x, diffuse.y, diffuse.z, 1.0f };
		GLfloat lightSpecular[] = { specular.x, specular.y, specular.z, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	}
};

/**
* @brief Funkcja która odpowiada za materia³ na obiekcie
*/
void setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) 
{
	GLfloat matAmbient[] = { ambient.x, ambient.y, ambient.z, 1.0f };
	GLfloat matDiffuse[] = { diffuse.x, diffuse.y, diffuse.z, 1.0f };
	GLfloat matSpecular[] = { specular.x, specular.y, specular.z, 1.0f };
	GLfloat matShininess[] = { shininess };

	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}

#endif // LIGHT_H