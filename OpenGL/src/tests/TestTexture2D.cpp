#include "TestTexture2D.h"
#include <iostream>

namespace test {

	TestTexture2D::TestTexture2D()
		: shader("res/shaders/textureVert.shader", "res/shaders/textureFrag.shader"),
		vao(),
		vb(),
		ib(),
		size(20)
	{
		vb.Bind();
		vao.Bind();
		ib.Bind();

		float vertices[16] = {
			//top left position   UV Coordinates
			-0.5f,0.5f,			  0.0f, 1.0f,//0
			//top right position
			0.5f,0.5f,			  1.0f,1.0f,//1     
			//bottom left position
			-0.5f,-0.5f,		  0.0f,0.0f,//2
			//bottom right position
			0.5f,-0.5f,			  1.0f,0.0f//3
		};

		vb.AddData(vertices, 16 * sizeof(float));

		vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));

		unsigned int indices[] = {
			0,1,3,
			0,3,2
		};

		ib.AddData(indices, 6);

		std::vector<GridCell> grid(size * size);
		int cellSize = 20;

		bool isWhite = true;
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				Color color{ 0,0,0 };
				if (isWhite) {
					color.red = 255;
					color.green = 255;
					color.blue = 255;
				}
				GridCell cell = { x,y,cellSize,color };
				grid[y * size + x] = cell;
				isWhite = !isWhite;
			}
			isWhite = !isWhite;
		}
		int width = size * cellSize;
		int height = size * cellSize;
		data = new unsigned char[width * height * 4];
		// Fill it with some pattern
		for (int i = 0; i < size * size; i++) {
			GridCell cell = grid[i];
			for (int y = 0; y < cellSize; y++) {
				for (int x = 0; x < cellSize; x++) {
					int pixelX = cell.x * cellSize + x;
					int pixelY = cell.y * cellSize + y;
					int index = (pixelY * width + pixelX) * 4;
					data[index + 0] = cell.color.red;
					data[index + 1] = cell.color.green;
					data[index + 2] = cell.color.blue;
					data[index + 3] = 255;
				}
			}
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		shader.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		shader.SetUniform1i("u_Texture", 0);

	}

	TestTexture2D::~TestTexture2D()
	{

	}
	void TestTexture2D::OnRender()
	{
		shader.Bind();
		vao.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}
	void TestTexture2D::OnUpdate(float deltaTime)
	{

		//unsigned char* newPixelData = new unsigned char[width * height * 4]; // 4 channels (RGBA)
		//for (int y = 0; y < height; ++y) {
		//	for (int x = 0; x < width; ++x) {
		//		int index = (y * width + x) * 4;
		//		newPixelData[index + 0] = y * width % 10 && x % 10 < 5 ? 255 : 2;
		//		newPixelData[index + 1] = y * width % 10 && x % 10 < 5 ? 255 : 255;
		//		newPixelData[index + 2] = y * width % 10 && x % 10 < 5 ? 255 : 134;
		//		newPixelData[index + 3] = 255;  // A
		//	}
		//}
		//glBindTexture(GL_TEXTURE_2D, texture);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, newPixelData);

	}
	void TestTexture2D::OnImGuiRender()
	{

	}
}