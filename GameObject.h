#pragma once
#include "includy.h"

/**
* @class GameObject
* @brief Klasa nadrzędna, po której dziedziczą pozostałe klasy, tworząc hierarchię
*/
class GameObject
{
public:
	virtual ~GameObject() = default; // Wirtualny destruktor
	virtual void update() = 0;       // Metoda abstrakcyjna do aktualizacji
};

/**
* @class UpdatableObject
* @brief Klasa dziedzicząca po GameObject
* Odpowiada za obiekty które będą się aktualizować wraz z każdą następną klatką programu
*/
class UpdatableObject : public virtual GameObject
{
public:
	virtual void update() override = 0; // Wirtualna metoda aktualizacji
};

/**
* @class DrawableObject
* @brief Klasa dziedzicząca po GameObject
* Odpowiada za obiekty które będą rysowane w programie
*/
class DrawableObject : public virtual GameObject
{
public:
	virtual void draw() const = 0; // Wirtualna metoda do rysowania obiektu
};

/**
* @class TransformableObject
* @brief Klasa dziedzicząca po GameObject
* Odpowiada za obiekty które będą podawane transformacji w programie
*/
class TransformableObject : public virtual GameObject
{
public:
	virtual void translate(float dx, float dy, float dz) = 0;
	virtual void rotate(float angle, float x, float y, float z) = 0;
	virtual void scale(float sx, float sy, float sz) = 0;
};

/**
* @class ShapeObject
* @brief Klasa dziedzicząca po GameObject
* Odpowiada za obiekty 3D
*/
class ShapeObject : public DrawableObject, public TransformableObject {
protected:
	float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
	float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;
	float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;

public:
	void translate(float dx, float dy, float dz) override {
		posX += dx; posY += dy; posZ += dz;
	}

	void rotate(float angle, float x, float y, float z) override {
		if (x) rotX += angle;
		if (y) rotY += angle;
		if (z) rotZ += angle;
	}

	void scale(float sx, float sy, float sz) override {
		scaleX *= sx; scaleY *= sy; scaleZ *= sz;
	}

	void draw() const override {
		// Provide a default implementation (or leave it pure virtual if derived classes must implement it)
	}
};

/**
* @class Primitive
* @brief Klasa dziedzicząca po DrawableObject i TransformableObject
* Odpowiada za obiekty prymitywy typu linia, punkt, trójkąt
*/
class Primitive : public DrawableObject, public TransformableObject
{
protected:
	vector<float> vertices; // Tablica wierzchołków (x, y, z)
	vector<float> colors;   // Tablica kolorów (r, g, b)

public:
	Primitive() = default;

	// Metoda do ustawiania wierzchołków
	void setVertices(const vector<float>& vertexData)
	{
		vertices = vertexData;
	}

	// Metoda do ustawiania kolorów
	void setColors(const vector<float>& colorData)
	{
		colors = colorData;
	}

	// Implementacja metody update() z GameObject
	void update() override {
		// Default implementation (can be overridden in derived classes)
	}

	// Implementacja metody draw() z DrawableObject
	void draw() const override {
		draw(GL_TRIANGLES); // Default mode for draw()
	}

	// Rysowanie prymitywu za pomocą glDrawArrays
	virtual void draw(GLenum mode) const
	{
		if (vertices.empty() || colors.empty())
		{
			return; // Brak danych do rysowania
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		// Przypisz tablice wierzchołków i kolorów
		glVertexPointer(3, GL_FLOAT, 0, vertices.data());
		glColorPointer(3, GL_FLOAT, 0, colors.data());

		// Rysuj prymityw
		glDrawArrays(mode, 0, vertices.size() / 3);

		// Wyłącz tablice
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	// Implementacja metod z TransformableObject
	void translate(float dx, float dy, float dz) override {
		for (size_t i = 0; i < vertices.size(); i += 3) {
			vertices[i] += dx;
			vertices[i + 1] += dy;
			vertices[i + 2] += dz;
		}
	}

	void rotate(float angle, float x, float y, float z) override {
		// Implementacja rotacji (można użyć macierzy transformacji)
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z));
		for (size_t i = 0; i < vertices.size(); i += 3) {
			glm::vec4 vertex(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
			vertex = rotationMatrix * vertex;
			vertices[i] = vertex.x;
			vertices[i + 1] = vertex.y;
			vertices[i + 2] = vertex.z;
		}
	}

	void scale(float sx, float sy, float sz) override {
		for (size_t i = 0; i < vertices.size(); i += 3) {
			vertices[i] *= sx;
			vertices[i + 1] *= sy;
			vertices[i + 2] *= sz;
		}
	}
};