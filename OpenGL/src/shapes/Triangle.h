#pragma once
#include "Shape.h"

class Triangle : public Shape {
public:
	void Draw() override;
	Triangle();
private:
	float m_Vertices[6];
	unsigned int m_Indices[3];
};