#pragma comment(lib, "freeglut.lib")
#include <gl/glut.h>
#include <gl/freeglut.h>
#include <iostream>
#include <functional>
#include <string>

using namespace std;

class Engine 
{
public:
    Engine(int argc, char** argv, const std::string& title, int width, int height)
        : windowWidth(width), windowHeight(height), fps(60), clearColor{ 0.0f, 0.0f, 0.0f, 1.0f } 
    {
        // Inicjacja biblioteki
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(width, height);
        glEnable(GL_DEPTH_TEST);
        glutCreateWindow(title.c_str());
    }

    void setFullscreen(bool enable) {
        if (enable) 
        {
            glutFullScreen();
        }
        else 
        {
            glutReshapeWindow(windowWidth, windowHeight);
        }
    }

    void setFPS(int targetFPS) 
    {
        fps = targetFPS;
    }

    void setClearColor(float r, float g, float b, float a = 1.0f) 
    {
        clearColor[0] = r;
        clearColor[1] = g;
        clearColor[2] = b;
        clearColor[3] = a;
    }

    void setProjection(bool perspective, float fov = 60.0f, float nearPlane = 1.0f, float farPlane = 100.0f) 
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (perspective)
        {
            gluPerspective(fov, static_cast<float>(windowWidth) / windowHeight, nearPlane, farPlane);
        }
        else
        {
            glOrtho(0.0, windowWidth, 0.0, windowHeight, nearPlane, farPlane);
        }
        glMatrixMode(GL_MODELVIEW);
    }

    void setKeyboardCallback(function<void(unsigned char, int, int)> callback) 
    {
        keyboardCallback = callback;
    }

    void setMouseCallback(function<void(int, int, int, int)> callback)
    {
        mouseCallback = callback;
    }

    void startMainLoop(function<void()> renderCallback) 
    {
        this->renderCallback = renderCallback;

        glutDisplayFunc([]() 
            {
            instance->render();
            });
        glutIdleFunc([]() {
            instance->idle();
            });
        glutKeyboardFunc([](unsigned char key, int x, int y) 
            {
            if (instance->keyboardCallback) {
                instance->keyboardCallback(key, x, y);
            }
            });
        glutMouseFunc([](int button, int state, int x, int y) 
            {
            if (instance->mouseCallback) 
            {
                instance->mouseCallback(button, state, x, y);
            }
            });

        instance = this;
        glutMainLoop();
    }

    ~Engine() {
        // Sprz¹tanie (FreeGLUT sam zwalnia pamiêæ)
        std::cout << "Shutting down engine..." << std::endl;
    }

private:
    static Engine* instance;
    int windowWidth, windowHeight, fps;
    float clearColor[4];
    std::function<void()> renderCallback;
    std::function<void(unsigned char, int, int)> keyboardCallback;
    std::function<void(int, int, int, int)> mouseCallback;

    void render()
    {
        glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (renderCallback)
        {
            renderCallback();
        }
        glutSwapBuffers();
    }

    void idle()
    {
        static int lastTime = glutGet(GLUT_ELAPSED_TIME);
        int currentTime = glutGet(GLUT_ELAPSED_TIME);
        int elapsed = currentTime - lastTime;

        if (elapsed > 1000 / fps)
        {
            glutPostRedisplay();
            lastTime = currentTime;
        }
    }
};

Engine* Engine::instance = nullptr;

// Przyk³ad u¿ycia
void renderScene()
{
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 2.0f, // Kamera ustawiona 2 jednostki od obiektu
        0.0f, 0.0f, 0.0f, // Punkt docelowy
        0.0f, 1.0f, 0.0f); // Wektor "up"

    glColor3f(1.0f, 0.0f, 0.0f); // Kolor trójk¹ta (czerwony)
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();
}


int main(int argc, char** argv)
{
    Engine engine(argc, argv, "FreeGLUT Engine", 800, 600);
    engine.setClearColor(0.1f, 0.1f, 0.1f);//szare okno
    engine.setProjection(true);
    engine.setFPS(60);
    engine.setKeyboardCallback([&engine](unsigned char key, int x, int y)
        {
        switch (key)
        {
        case 27://ESC
            cout << "Okno zniknie" << endl;
            exit(0);
            break;
        case 113://Q
            cout << "Szare okno" << endl;
            engine.setClearColor(0.3f, 0.3f, 0.3f); // Zmiana t³a na szaer
            glutPostRedisplay(); // Odœwie¿ okno
            break;
        case 114://R
            cout << "Czerowne okno" << endl;
            engine.setClearColor(1.0f, 0.0f, 0.0f); // Zmiana t³a na czerwone
            glutPostRedisplay(); // Odœwie¿ okno
            break;
        case 103://G
            cout << "Zielone okno" << endl;
            engine.setClearColor(0.0f, 1.0f, 0.0f); // Zmiana t³a na zielone
            glutPostRedisplay(); // Odœwie¿ okno
            break;
        case 98://B
            cout << "Niebieskie okno" << endl;
            engine.setClearColor(0.0f, 0.0f, 1.0f); // Zmiana t³a na niebieskie
            glutPostRedisplay(); // Odœwie¿ okno
            break;

        default:
            cout<< "Nacisnieo klawisz " << (char)key << " kod " << (int)key << "\n";
            break;
        }
   
        });
    engine.startMainLoop(renderScene);
    return 0;
}
