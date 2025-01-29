#pragma once
#include "includy.h"

#include "const.h"
#include "TextureHandler.h"


// Class CUBE
class CUBE {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 transform;
	GLuint textureID;  // ID tekstury Allegro/OpenGL

	CUBE()
		: position(0.0f), rotation(0.0f), scale(1.0f), transform(glm::mat4(1.0f)), textureID(0) {}


	// Aktualizacja macierzy transformacji
	void UpdateTransform() {
		transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, scale);
	}

	void Translate(float x, float y, float z) {
		position += glm::vec3(x, y, z);
		UpdateTransform();
	}

	void Rotate(float x, float y, float z) {
		rotation += glm::vec3(x, y, z);
		UpdateTransform();
	}

	void Scale(float x, float y, float z) {
		scale *= glm::vec3(x, y, z);
		UpdateTransform();
	}

	// Funkcja rysuj¹ca szeœcian
	void draw() {
		glEnable(GL_CULL_FACE); // Enable face culling
		glCullFace(GL_BACK);    // Cull back faces
		glFrontFace(GL_CCW);    // Set counter-clockwise winding as front faces

		glPushMatrix();
		glMultMatrixf(glm::value_ptr(transform));

		glBegin(GL_QUADS);

		// Front face
		glNormal3f(0.0f, 0.0f, 1.0f); // Normal for front face
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);

		// Back face
		glNormal3f(0.0f, 0.0f, -1.0f); // Normal for back face
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);

		// Top face
		glNormal3f(0.0f, 1.0f, 0.0f); // Normal for top face
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.5f, 0.5f, -0.5f);

		// Bottom face
		glNormal3f(0.0f, -1.0f, 0.0f); // Normal for bottom face
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

		// Left face
		glNormal3f(-1.0f, 0.0f, 0.0f); // Normal for left face
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f, -0.5f);

		// Right face
		glNormal3f(1.0f, 0.0f, 0.0f); // Normal for right face
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);

		glEnd();
		glPopMatrix();
	}
};

// Class PYRAMID
class PYRAMID {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 transform;

	PYRAMID()
		: position(0.0f, 0.0f, -3.0f), rotation(0.0f), scale(1.0f), transform(glm::mat4(1.0f)) {}

	// Aktualizacja macierzy transformacji
	void UpdateTransform() {
		transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, scale);
	}

	void Translate(float x, float y, float z) {
		position += glm::vec3(x, y, z);
		UpdateTransform();
	}

	void Rotate(float x, float y, float z) {
		rotation += glm::vec3(x, y, z);
		UpdateTransform();
	}

	void Scale(float x, float y, float z) {
		scale *= glm::vec3(x, y, z);
		UpdateTransform();
	}

	// Funkcja rysuj¹ca piramidê
	void draw() {
		glPushMatrix();
        glMultMatrixf(glm::value_ptr(transform));

        glBegin(GL_TRIANGLES);

        // Base of the pyramid (square)
        glm::vec3 baseNormal = glm::vec3(0.0f, -1.0f, 0.0f); // Normal for the base
        glNormal3fv(glm::value_ptr(baseNormal));

        // Set color for the base (e.g., blue)
        glColor3f(0.0f, 0.0f, 1.0f); // Blue

        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);

        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);

        // Side walls of the pyramid
        // Left wall
        glm::vec3 leftNormal = calculateNormal(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.5f, 0.0f));
        glNormal3fv(glm::value_ptr(leftNormal));

        // Set color for the left wall (e.g., red)
        glColor3f(1.0f, 0.0f, 0.0f); // Red

        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.0f, 0.5f, 0.0f);

        // Front wall
        glm::vec3 frontNormal = calculateNormal(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.5f, 0.0f));
        glNormal3fv(glm::value_ptr(frontNormal));

        // Set color for the front wall (e.g., green)
        glColor3f(0.0f, 1.0f, 0.0f); // Green

        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.0f, 0.5f, 0.0f);

        // Right wall
        glm::vec3 rightNormal = calculateNormal(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.5f, 0.0f));
        glNormal3fv(glm::value_ptr(rightNormal));

        // Set color for the right wall (e.g., yellow)
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow

        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.0f, 0.5f, 0.0f);

        // Back wall
        glm::vec3 backNormal = calculateNormal(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.5f, 0.0f));
        glNormal3fv(glm::value_ptr(backNormal));

        // Set color for the back wall (e.g., magenta)
        glColor3f(1.0f, 0.0f, 1.0f); // Magenta

        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.0f, 0.5f, 0.0f);

        glEnd();
        glPopMatrix();
	}

private:
	// Funkcja do obliczania normalnej dla trójk¹ta
	glm::vec3 calculateNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
		glm::vec3 edge1 = v2 - v1;
		glm::vec3 edge2 = v3 - v1;
		return glm::normalize(glm::cross(edge1, edge2));
	}
};