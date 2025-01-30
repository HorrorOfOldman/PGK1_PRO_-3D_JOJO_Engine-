#pragma once
#include "includy.h"
#include "shapes.h"
#include "const.h"
#include "Observer.h"
#include "light.h"
#include "prim.h"

/**
* @class Engine
* @brief Klasa główna programu
* odpowiada za wyświetlanie okna
* zbiera wszystkie skłądowe programu w jedno
*/
class Engine
{
public:
	//"flagi" w programie odpowiadajace za wyświetlanie
	static bool LightE;
	static bool PrimE;
	static bool CubE;
	static bool PyramidE;
	static bool SolidE;
	static bool TeapotE;
	static bool MatE;

	//oświetlenie globalne- jako wskaźnik
	static Light* light;

	//funkcja inicjalizująca elementy niesbędne do uruchomienia programu
	static void initialize(int argc, char** argv)
	{
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

		//inicjalizacja stanu flag
		LightE = false;
		PrimE = false;
		CubE = false;
		PyramidE = false;
		SolidE = false;
		TeapotE = false;
		MatE = false;

		//inicjalizacja światła przez konstrunktor
		light = new Light(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	}

	//uruchomienie pęli głównej
	static void run()
	{
		glutMainLoop();
	}

	//dynamiczne alokowanie światła
	static void cleanup()
	{
		delete light;
	}

private:
	//funkcja odpowiadajaca za rendoerowanie sceny
	static void renderScene()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		Observer observer;
		glm::mat4 view = observer.getViewMatrix();
		glMultMatrixf(glm::value_ptr(view));

		//on/off światło
		if (LightE)
		{
			glEnable(GL_LIGHTING); // Enable lighting
			glEnable(GL_LIGHT0);   // Enable light source 0
			light->setupLight(GL_LIGHT0); // Set up light properties
		}
		else
		{
			glDisable(GL_LIGHTING); // Disable lighting
			glDisable(GL_LIGHT0);   // Disable light source 0
		}

		//wyświetlanie materiału
		if (MatE)
		{
			//ustawienie materiału dla obiektów
			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			setMaterial(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);
		}
		else
		{
			// Reset material properties to default
			resetMaterial();
		}

		//wyświetlanie prymitywów
		if (PrimE)
		{
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(cubeRotation));

			Point point(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			point.draw();

			Line line({ -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f });
			line.draw();

			Triangle triangle({ -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f });
			triangle.draw();

			glPopMatrix();
		}

		//wyświetlanie kostki
		if (CubE)
		{
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(cubeRotation));
			CUBE cube;
			cube.draw();
			glPopMatrix();
		}

		//wyświetlanie piramidki
		if (PyramidE)
		{
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(cubeRotation));
			PYRAMID pyramid;
			pyramid.draw();
			glPopMatrix();
		}

		//wyświetlanie czajniczka
		if (TeapotE)
		{
			glPushMatrix();
			glTranslatef(0.0f, -0.5f, -3.0f);
			glMultMatrixf(glm::value_ptr(cubeRotation));
			glColor3f(1.0f, 0.5f, 0.0f);
			glutSolidTeapot(0.5);
			glPopMatrix();
		}

		renderText();

		glutSwapBuffers();
	}

	// ustawienie materiałów na domyślne
	static void resetMaterial() 
	{
		glDisable(GL_COLOR_MATERIAL);
		glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
		glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
		glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
	}

	//renderowanie tekstu informującego w lewym górnym rogu
	static void renderText()
	{
		//projekcja tkstu jako 2D
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		glColor3f(1.0f, 1.0f, 1.0f);

		int x = 10;
		int y = WINDOW_HEIGHT - 20;

		//Teksty
		string lightStatus = "LightE: " + string(LightE ? "ON" : "OFF");
		string primStatus = "PrimE: " + string(PrimE ? "ON" : "OFF");
		string cubeStatus = "CubE: " + string(CubE ? "ON" : "OFF");
		string pyramideStatus = "PyramidE: " + string(PyramidE ? "ON" : "OFF");
		string solidEStatus = "SolidE: " + string(SolidE ? "ON" : "OFF");
		string teapotStatus = "TeapotE: " + string(TeapotE ? "ON" : "OFF");
		string materialStatus = "MatE: " + string(MatE ? "ON" : "OFF");

		renderString(x, y, solidEStatus);
		renderString(x, y - 20, materialStatus);
		renderString(x, y - 40, lightStatus);
		renderString(x, y - 60, primStatus);
		renderString(x, y - 80, cubeStatus);
		renderString(x, y - 100, pyramideStatus);
		renderString(x, y - 120, teapotStatus);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glEnable(GL_DEPTH_TEST);
	}

	static void renderString(int x, int y, const string & text) 
	{
		glRasterPos2i(x, y); //ustawienie tekstu
		for (char c : text) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c); //wyrenderowanie tekstu
		}
	}

	static void reshape(int w, int h) 
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
	}

	static void keyboard(unsigned char key, int x, int y) 
	{
		Observer observer;
		observer.processKeyboard(key);
		glutPostRedisplay();
	}

	static void specialKeys(int key, int x, int y) 
	{
		switch (key) {
		case GLUT_KEY_LEFT: cubeRotation = glm::rotate(cubeRotation, glm::radians(CUBE_ROTATION_SPEED), glm::vec3(0.0f, 1.0f, 0.0f)); break;
		case GLUT_KEY_RIGHT: cubeRotation = glm::rotate(cubeRotation, glm::radians(-CUBE_ROTATION_SPEED), glm::vec3(0.0f, 1.0f, 0.0f)); break;
		case GLUT_KEY_UP: cubeRotation = glm::rotate(cubeRotation, glm::radians(CUBE_ROTATION_SPEED), glm::vec3(1.0f, 0.0f, 0.0f)); break;
		case GLUT_KEY_DOWN: cubeRotation = glm::rotate(cubeRotation, glm::radians(-CUBE_ROTATION_SPEED), glm::vec3(1.0f, 0.0f, 0.0f)); break;
		case GLUT_KEY_F1:
			SolidE = !SolidE;
			if (SolidE) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Solid mode
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
			}
			break;
		case GLUT_KEY_F2: MatE = !MatE; break;
		case GLUT_KEY_F3: LightE = !LightE; break;
		case GLUT_KEY_F4: PrimE = !PrimE; break;
		case GLUT_KEY_F5: CubE = !CubE; break;
		case GLUT_KEY_F6: PyramidE = !PyramidE; break;
		case GLUT_KEY_F7: TeapotE = !TeapotE; break;
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

	static void timer(int value) 
	{
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
bool Engine::TeapotE;
bool Engine::MatE;
Light* Engine::light = nullptr;