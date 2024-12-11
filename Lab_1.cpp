#pragma comment(lib, "freeglut.lib")
#include <gl/glut.h>
#include <gl/freeglut.h>
#include <iostream>
#include <functional>
#include <string>
#include <vector>


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
    #include <vector>
#include <GL/freeglut.h>

class Primitive {
protected:
    std::vector<float> vertices; // Tablica wierzcho�k�w (x, y, z)
    std::vector<float> colors;   // Tablica kolor�w (r, g, b)

public:
    Primitive() = default;

    // Metoda do ustawiania wierzcho�k�w
    void setVertices(const std::vector<float>& vertexData) {
        vertices = vertexData;
    }

    // Metoda do ustawiania kolor�w
    void setColors(const std::vector<float>& colorData) {
        colors = colorData;
    }

    // Rysowanie prymitywu za pomoc� glDrawArrays
    virtual void draw(GLenum mode) const {
        if (vertices.empty() || colors.empty()) {
            return; // Brak danych do rysowania
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        // Przypisz tablice wierzcho�k�w i kolor�w
        glVertexPointer(3, GL_FLOAT, 0, vertices.data());
        glColorPointer(3, GL_FLOAT, 0, colors.data());

        // Rysuj prymityw
        glDrawArrays(mode, 0, vertices.size() / 3);

        // Wy��cz tablice
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
};

// Klasa dla punktu
class Point : public Primitive 
{
public:
    Point(float x, float y, float z, float r, float g, float b)
    {
        vertices = {x, y, z};
        colors = {r, g, b};
    }

    void draw() const
    {
        Primitive::draw(GL_POINTS);
    }
};

// Klasa dla linii
class Line : public Primitive
{
public:
    Line(const std::vector<float>& vertexData, const std::vector<float>& colorData)
    {
        setVertices(vertexData);
        setColors(colorData);
    }

    void draw() const 
    {
        Primitive::draw(GL_LINES);
    }
};

// Klasa dla tr�jk�ta
class Triangle : public Primitive {
public:
    Triangle(const std::vector<float>& vertexData, const std::vector<float>& colorData) {
        setVertices(vertexData);
        setColors(colorData);
    }

    void draw() const
    {
        Primitive::draw(GL_TRIANGLES);
    }
};


    ~Engine() {
        // Sprz�tanie (FreeGLUT sam zwalnia pami��)
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

#include <vector>
#include <GL/freeglut.h>

class Primitive {
protected:
    vector<float> vertices; // Tablica wierzcho�k�w (x, y, z)
    vector<float> colors;   // Tablica kolor�w (r, g, b)

public:
    Primitive() = default;

    // Metoda do ustawiania wierzcho�k�w
    void setVertices(const vector<float>& vertexData)
    {
        vertices = vertexData;
    }

    // Metoda do ustawiania kolor�w
    void setColors(const vector<float>& colorData)
    {
        colors = colorData;
    }

    // Rysowanie prymitywu za pomoc� glDrawArrays
    virtual void draw(GLenum mode) const 
    {
        if (vertices.empty() || colors.empty()) 
        {
            return; // Brak danych do rysowania
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        // Przypisz tablice wierzcho�k�w i kolor�w
        glVertexPointer(3, GL_FLOAT, 0, vertices.data());
        glColorPointer(3, GL_FLOAT, 0, colors.data());

        // Rysuj prymityw
        glDrawArrays(mode, 0, vertices.size() / 3);

        // Wy��cz tablice
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
};

// Klasa dla punktu
class Point : public Primitive 
{
public:
    Point(float x, float y, float z, float r, float g, float b) 
    {
        vertices = { x, y, z };
        colors = { r, g, b };
    }

    void draw() const{
        Primitive::draw(GL_POINTS);
    }
};

// Klasa dla linii
class Line : public Primitive 
{
public:
    Line(const vector<float>& vertexData, const vector<float>& colorData)
    {
        setVertices(vertexData);
        setColors(colorData);
    }

    void draw() const
    {
        Primitive::draw(GL_LINES);
    }
};

// Klasa dla tr�jk�ta
class Triangle : public Primitive 
{
public:
    Triangle(const vector<float>& vertexData, const std::vector<float>& colorData)
    {
        setVertices(vertexData);
        setColors(colorData);
    }

    void draw() const 
    {
        Primitive::draw(GL_TRIANGLES);
    }
};

// Klasa dla sze�cianu
class Cube
{
private:
    vector<float> vertices =
    {
        // Wierzcho�ki sze�cianu (8 wierzcho�k�w)
        -0.5f, -0.5f, -0.5f,  // 0: Lewy dolny ty�
         0.5f, -0.5f, -0.5f,  // 1: Prawy dolny ty�
         0.5f,  0.5f, -0.5f,  // 2: Prawy g�rny ty�
        -0.5f,  0.5f, -0.5f,  // 3: Lewy g�rny ty�
        -0.5f, -0.5f,  0.5f,  // 4: Lewy dolny prz�d
         0.5f, -0.5f,  0.5f,  // 5: Prawy dolny prz�d
         0.5f,  0.5f,  0.5f,  // 6: Prawy g�rny prz�d
        -0.5f,  0.5f,  0.5f   // 7: Lewy g�rny prz�d
    };

    vector<unsigned int> indices = 
    {
        // Tylna �cianka
        0, 1, 2,  2, 3, 0,
        // Przednia �cianka
        4, 5, 6,  6, 7, 4,
        // Lewa �cianka
        0, 4, 7,  7, 3, 0,
        // Prawa �cianka
        1, 5, 6,  6, 2, 1,
        // Dolna �cianka
        0, 1, 5,  5, 4, 0,
        // G�rna �cianka
        3, 2, 6,  6, 7, 3
    };

    vector<float> colors = 
    {
        // Kolory dla ka�dego wierzcho�ka (RGB)
        1.0f, 0.0f, 0.0f,  // Czerwony
        0.0f, 1.0f, 0.0f,  // Zielony
        0.0f, 0.0f, 1.0f,  // Niebieski
        1.0f, 1.0f, 0.0f,  // ��ty
        1.0f, 0.0f, 1.0f,  // Fioletowy
        0.0f, 1.0f, 1.0f,  // Turkusowy
        1.0f, 1.0f, 1.0f,  // Bia�y
        0.0f, 0.0f, 0.0f   // Czarny
    };

public:
    void draw() const 
{
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        // Przypisanie wierzcho�k�w i kolor�w
        glVertexPointer(3, GL_FLOAT, 0, vertices.data());
        glColorPointer(3, GL_FLOAT, 0, colors.data());

        // Rysowanie za pomoc� indeks�w
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

        // Wy��czenie tablic
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
};


Engine* Engine::instance = nullptr;

// Przyk�ad u�ycia
void renderScene()
{
    /*
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 2.0f, // Kamera ustawiona 2 jednostki od obiektu
        0.0f, 0.0f, 0.0f, // Punkt docelowy
        0.0f, 1.0f, 0.0f); // Wektor "up"

    glColor3f(1.0f, 0.0f, 0.0f); // Kolor tr�jk�ta (czerwony)
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();
    */
    //-------------------------------------
    /*
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f,  // Kamera
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);

    Point point(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    point.draw();

    // Rysowanie linii
    vector<float> lineVertices = { -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f };
    vector<float> lineColors = { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    Line line(lineVertices, lineColors);
    line.draw();

    // Rysowanie tr�jk�ta
    vector<float> triangleVertices = 
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    vector<float> triangleColors = 
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    Triangle triangle(triangleVertices, triangleColors);
    triangle.draw();
    ----------------------------------------------------------------
    */

    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 3.0f,  // Kamera ustawiona 3 jednostki od obiektu
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);
    // Rysowanie sze�cianu
    Cube cube;
    cube.draw();
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
            engine.setClearColor(0.3f, 0.3f, 0.3f); // Zmiana t�a na szaer
            glutPostRedisplay(); // Od�wie� okno
            break;
        case 114://R
            cout << "Czerowne okno" << endl;
            engine.setClearColor(1.0f, 0.0f, 0.0f); // Zmiana t�a na czerwone
            glutPostRedisplay(); // Od�wie� okno
            break;
        case 103://G
            cout << "Zielone okno" << endl;
            engine.setClearColor(0.0f, 1.0f, 0.0f); // Zmiana t�a na zielone
            glutPostRedisplay(); // Od�wie� okno
            break;
        case 98://B
            cout << "Niebieskie okno" << endl;
            engine.setClearColor(0.0f, 0.0f, 1.0f); // Zmiana t�a na niebieskie
            glutPostRedisplay(); // Od�wie� okno
            break;

        default:
            cout<< "Nacisnieo klawisz " << (char)key << " kod " << (int)key << "\n";
            break;
        }
   
        });
    engine.startMainLoop(renderScene);
    return 0;
}
