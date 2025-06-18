#pragma once

#include "Test.h"
#include "glm/glm.hpp" 
#include <array>

namespace test {

	struct Vertex {
		glm::vec2 position;
		glm::vec2 uvCoord;
	};

	class TestBatchRendering : public Test {
	public:
		TestBatchRendering();
		~TestBatchRendering();

		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
		std::array<int, 6> CreateQuadIndices(int quadIndex);
		std::array<Vertex, 4> CreateQuadVertices(float posX, float posY);
	private:
		VertexArray vao;
		VertexBuffer vb;
		IndexBuffer ib;
		Shader shader;
		glm::mat4 mvpMatrix;
		glm::vec2 ball1Pos;
		glm::vec2 ball2Pos;

		std::vector<glm::vec2> ballPositions;
		int numOfBalls;
	};
}