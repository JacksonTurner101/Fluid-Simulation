#pragma once

#include "Test.h"
#include "glm/gtc/matrix_transform.hpp"


namespace test {
	struct Particle {
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		float size;
	};

	class TestPhysics : public Test {
	public:
		TestPhysics();
		~TestPhysics();

		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
	private:
		void CheckCollisions();
		void UpdatePosition(float deltaTime);

		VertexArray vao;
		VertexBuffer vb;
		IndexBuffer ib;
		Shader shader;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 modelMatrix;
		glm::vec3 translation;
		glm::vec2 boundingBox;
		Particle ball;

	};
}