#pragma once

#include "Test.h"
#include "glm/gtc/matrix_transform.hpp"


namespace test {
	class TestModelViewProjection : public Test {
	public:
		TestModelViewProjection();
		~TestModelViewProjection();

		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
	private:
		VertexArray vao;
		VertexBuffer vb;
		IndexBuffer ib;
		Shader shader;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 modelMatrix;
		glm::vec3 translation;
		float rotation;
		float scale;

	};
}