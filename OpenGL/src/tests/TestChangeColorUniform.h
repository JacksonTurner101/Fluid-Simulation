
#pragma once

#include "Test.h"

namespace test {
	class TestChangeColorUniform : public Test {
	public:
		TestChangeColorUniform();
		~TestChangeColorUniform();

		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
	private:
		VertexArray vao;
		VertexBuffer vb;
		IndexBuffer ib;
		Shader shader;
		float m_Color[4];
		float changeValR;
		float changeValG;
		float changeValB;
	};
}