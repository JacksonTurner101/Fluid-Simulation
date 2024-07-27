#pragma once
#include "Shape.h"

class Circle : public Shape {
public:
	void Draw() override;
	Circle();
	~Circle();

private:
	float vertices[6];
	int indices[3];
};