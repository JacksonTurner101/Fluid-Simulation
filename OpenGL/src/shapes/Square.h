#pragma once
#include "Shape.h"

class Square : public Shape {
public:
	void Draw() override;
	Square();
private:
	float m_Vertices[8];
	unsigned int m_Indices[6];
};