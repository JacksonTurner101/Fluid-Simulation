#pragma once

#include "Test.h"

namespace test {

	struct Vertex {
		glm::vec3 position;
		glm::vec2 uvCoord;
	};

	class TestBatchRendering : public Test {
	public:
		TestBatchRendering();
		~TestBatchRendering();

		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
	private:
		VertexArray vao;
		VertexBuffer vb;
		IndexBuffer ib;
		Shader shader;
		
	};
}