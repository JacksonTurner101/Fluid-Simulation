#pragma once

#include "Test.h"

namespace test {
	class TestCircleDraw : public Test {
	public:
		TestCircleDraw();
		~TestCircleDraw();

		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
	private:
		VertexArray vao;
		VertexBuffer vb;
		IndexBuffer ib;
		Shader shader;
		glm::mat4 mvpMatrix;
	};
}