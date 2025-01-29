#pragma once
#include "includy.h"
#include "shapes.h"
#include "const.h"
#include "Observer.h"
#include "light.h"
#include "prim.h"

using namespace std;

class Engine {
public:
	// Static boolean variables
	static bool LightE;
	static bool PrimE;
	static bool CubE;
	static bool PyramidE;
	static bool SolidE;
	static bool TeapotE; // New boolean for Utah Teapot

	// Global light object (as a pointer)
	static Light* light;

	static void initialize(int argc, char** argv) {
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		glutCreateWindow("JoJo_Engine");

		glEnable(GL_DEPTH_TEST);

		glutDisplayFunc(renderScene);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keyboard);
		glutSpecialFunc(specialKeys);
		glutPassiveMotionFunc(mouseMove);
		glutTimerFunc(0, timer, 0);

		// Initialize the boolean variables
		LightE = false;
		PrimE = false;
		CubE = false;
		PyramidE = false;
		SolidE = false;
		TeapotE = false; // Initialize TeapotE to false

		// Initialize the light object using the parameterized constructor
		light = new Light(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	}

	static void run() {
		glutMainLoop();
	}

	// Clean up dynamically allocated memory
	static void cleanup() {
		delete light;
	}

private:
	static void renderScene() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		Observer observer;
		glm::mat4 view = observer.getViewMatrix();
		glMultMatrixf(glm::value_ptr(view));

		// Enable or disable lighting based on LightE
		if (LightE) {
			glEnable(GL_LIGHTING); // Enable lighting
			glEnable(GL_LIGHT0);   // Enable light source 0
			light->setupLight(GL_LIGHT0); // Set up light properties
		}
		else {
			glDisable(GL_LIGHTING); // Disable lighting
			glDisable(GL_LIGHT0);   // Disable light source 0
		}

		// Set material for objects
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		setMaterial(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);

		// Draw objects if PrimE is true
		if (PrimE) {
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(cubeRotation));

			// Example usage of Point, Line, and Triangle
			Point point(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f); // Red point at origin
			point.draw();

			Line line({ -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f }); // Green line
			line.draw();

			Triangle triangle({ -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }); // Blue triangle
			triangle.draw();

			glPopMatrix();
		}

		// Draw cube if CubE is true
		if (CubE) {
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(cubeRotation));
			CUBE cube;
			cube.draw();
			glPopMatrix();
		}

		// Draw pyramid if PyramidE is true
		if (PyramidE) {
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(cubeRotation));
			PYRAMID pyramid;
			pyramid.draw();
			glPopMatrix();
		}

		// Draw Utah Teapot if TeapotE is true
		if (TeapotE) {
			glPushMatrix();
			glTranslatef(0.0f, -0.5f, -3.0f); // Position the teapot
			glMultMatrixf(glm::value_ptr(cubeRotation)); // Apply rotation to the teapot
			glColor3f(1.0f, 0.5f, 0.0f); // Set color to orange
			glutSolidTeapot(0.5); // Draw the teapot with a size of 0.5
			glPopMatrix();
		}

		// Render text in the top-left corner
		renderText();

		glutSwapBuffers();
	}

	static void renderText() {
		// Switch to 2D projection for text rendering
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT); // Set up orthographic projection
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		// Disable depth testing for text
		glDisable(GL_DEPTH_TEST);

		// Set text color (white)
		glColor3f(1.0f, 1.0f, 1.0f);

		// Position the text in the top-left corner
		int x = 10;
		int y = WINDOW_HEIGHT - 20;

		// Render the text
		string lightStatus = "LightE: " + string(LightE ? "ON" : "OFF");
		string primStatus = "PrimE: " + string(PrimE ? "ON" : "OFF");
		string cubeStatus = "CubE: " + string(CubE ? "ON" : "OFF");
		string pyramideStatus = "PyramidE: " + string(PyramidE ? "ON" : "OFF");
		string solidEStatus = "SolidE: " + string(SolidE ? "ON" : "OFF");
		string teapotStatus = "TeapotE: " + string(TeapotE ? "ON" : "OFF");

		renderString(x, y, lightStatus);
		renderString(x, y - 20, primStatus);
		renderString(x, y - 40, cubeStatus);
		renderString(x, y - 60, pyramideStatus);
		renderString(x, y - 80, solidEStatus);
		renderString(x, y - 100, teapotStatus);

		// Restore the original projection and modelview matrices
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		// Re-enable depth testing
		glEnable(GL_DEPTH_TEST);
	}

	static void renderString(int x, int y, const string & text) {
		glRasterPos2i(x, y); // Set the position for the text
		for (char c : text) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c); // Render each character
		}
	}

	static void reshape(int w, int h) {
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
	}

	static void keyboard(unsigned char key, int x, int y) {
		Observer observer;
		observer.processKeyboard(key);
		glutPostRedisplay();
	}

	static void specialKeys(int key, int x, int y) {
		switch (key) {
		case GLUT_KEY_LEFT: cubeRotation = glm::rotate(cubeRotation, glm::radians(CUBE_ROTATION_SPEED), glm::vec3(0.0f, 1.0f, 0.0f)); break;
		case GLUT_KEY_RIGHT: cubeRotation = glm::rotate(cubeRotation, glm::radians(-CUBE_ROTATION_SPEED), glm::vec3(0.0f, 1.0f, 0.0f)); break;
		case GLUT_KEY_UP: cubeRotation = glm::rotate(cubeRotation, glm::radians(CUBE_ROTATION_SPEED), glm::vec3(1.0f, 0.0f, 0.0f)); break;
		case GLUT_KEY_DOWN: cubeRotation = glm::rotate(cubeRotation, glm::radians(-CUBE_ROTATION_SPEED), glm::vec3(1.0f, 0.0f, 0.0f)); break;
		case GLUT_KEY_F1:
			SolidE = !SolidE; // Toggle solid/wireframe mode
			if (SolidE) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Solid mode
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
			}
			break;
		case GLUT_KEY_F3: LightE = !LightE; break; // Toggle light
		case GLUT_KEY_F4: PrimE = !PrimE; break; // Toggle primitives
		case GLUT_KEY_F5: CubE = !CubE; break; // Toggle cube
		case GLUT_KEY_F6: PyramidE = !PyramidE; break; // Toggle pyramid
		case GLUT_KEY_F7: TeapotE = !TeapotE; break; // Toggle Utah Teapot
		default: cout << "Nacisnieto klawisz " << (char)key << " kod " << (int)key << "\n"; break;
		}
		glutPostRedisplay();
	}

	static void mouseMove(int x, int y) {
		if (firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		float xoffset = x - lastX;
		float yoffset = lastY - y; // Reversed since y-coordinates range from bottom to top
		lastX = x;
		lastY = y;

		Observer observer;
		observer.processMouse(xoffset, yoffset);
		glutPostRedisplay();
	}

	static void timer(int value) {
		glutPostRedisplay();
		glutTimerFunc(1000 / 60, timer, 0); // 60 FPS
	}
};

// Initialize static members
bool Engine::LightE;
bool Engine::PrimE;
bool Engine::CubE;
bool Engine::SolidE;
bool Engine::PyramidE;
bool Engine::TeapotE; // Initialize TeapotE
Light* Engine::light = nullptr; // Initialize the light pointer to nullptr