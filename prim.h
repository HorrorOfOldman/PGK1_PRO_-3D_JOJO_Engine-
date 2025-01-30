#pragma once
#include "includy.h"
#include "GameObject.h"

/**
* @class POINT
* @brief Klasa dziedzicząca po Primitive
* reprezentuje punkt
*/
class Point : public Primitive
{
public:
	Point(float x, float y, float z, float r, float g, float b)
	{
		setVertices({ x, y, z });
		setColors({ r, g, b });
	}

	void draw() const override {
		glPointSize(5.0f); // Set point size for better visibility
		Primitive::draw(GL_POINTS);
	}
};

/**
* @class Line
* @brief Klasa dziedzicząca po Primitive
* reprezentuje prostą
*/
class Line : public Primitive
{
public:
	Line(const vector<float>& vertexData, const vector<float>& colorData)
	{
		setVertices(vertexData);
		setColors(colorData);
	}

	void draw() const override {
		Primitive::draw(GL_LINES);
	}
};

/**
* @class Triangle
* @brief Klasa dziedzicząca po Primitive
* reprezentuje trójkąt
*/
class Triangle : public Primitive
{
public:
	Triangle(const vector<float>& vertexData, const std::vector<float>& colorData)
	{
		setVertices(vertexData);
		setColors(colorData);
	}

	void draw() const override {
		Primitive::draw(GL_TRIANGLES);
	}
};