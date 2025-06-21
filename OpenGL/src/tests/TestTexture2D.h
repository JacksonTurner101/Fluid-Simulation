#pragma once

#include "Test.h"

namespace test {

	struct Color {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	};

	struct GridCell {
		int x;
		int y;
		int size;
		Color color;
	};

	class TestTexture2D : Test {

	public:

		TestTexture2D();
		~TestTexture2D();
		void OnRender() override;
		void OnUpdate(float deltaTime);
		void OnImGuiRender() override;

	private:

		VertexBuffer vb;
		VertexArray vao;
		IndexBuffer ib;
		Shader shader;
		//int width;
		//int height;
		unsigned char* data;
		unsigned int texture;
		float timeValue;

	};
}