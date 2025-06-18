#include "TestBatchRendering.h"
#include "glm/gtc/matrix_transform.hpp"
#include "iostream"

namespace test {

	test::TestBatchRendering::TestBatchRendering() :
		vb(), ib(), vao(), shader("res/shaders/circleVert.shader", "res/shaders/circleFrag.shader"),
		mvpMatrix(1.0f), ball1Pos(0,0), ball2Pos(-3,0), numOfBalls(10)
	{
		//const int numOfQuads = 2;
		const unsigned int numOfVertices = numOfBalls * 4;
		vb.ReserveSpace(sizeof(Vertex) * numOfVertices, GL_DYNAMIC_DRAW);
		vao.Bind();
		ib.Bind();

		vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
		vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uvCoord));

		//const unsigned int numOfIndices = numOfQuads * 6;
		//unsigned int indices[numOfIndices]{};

		const unsigned int numOfIndices = numOfBalls * 6;
		std::vector<unsigned int> indices(numOfIndices);

		for (int i = 0; i < numOfBalls; i++) {
			auto quadIndices = CreateQuadIndices(i);
			memcpy(indices.data() + (i * 6), quadIndices.data(), quadIndices.size() * sizeof(int));
		}

		ib.AddData(indices.data(), numOfIndices);

		glm::mat4 proj = glm::ortho(-60.0f, 60.0f, -40.0f, 40.0f, -1.0f, 1.0f);
		mvpMatrix *= proj;

		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvpMatrix);
	}

	test::TestBatchRendering::~TestBatchRendering()
	{

	}

	void test::TestBatchRendering::OnRender()
	{
		shader.Bind();
		vao.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	std::array<Vertex, 4> test::TestBatchRendering::CreateQuadVertices(float posX, float posY) {

		//top left
		Vertex v1{};
		v1.position = { -1.0f + posX,1.0f + posY };
		v1.uvCoord = { -1.0f, 1.0f };

		//top right
		Vertex v2{};
		v2.position = { 1.0f + posX,1.0f + posY };
		v2.uvCoord = { 1.0f,1.0f };

		//bottom left
		Vertex v3{};
		v3.position = { -1.0f + posX,-1.0f + posY };
		v3.uvCoord = { -1.0f,-1.0f };

		//bottom right
		Vertex v4{};
		v4.position = { 1.0f + posX,-1.0f + posY };
		v4.uvCoord = { 1.0f,-1.0f };

		return { v1,v2,v3,v4 };
	}

	void test::TestBatchRendering::OnUpdate(float deltaTime)
	{
		std::vector<Vertex> vertices(4 * numOfBalls);
		for (int i = 0; i < numOfBalls; i++) {
			auto ball = CreateQuadVertices((i * 5),0);
			memcpy(vertices.data() + (i * ball.size()), ball.data(), ball.size() * sizeof(Vertex));
		}

		/*
		auto q1 = CreateQuadVertices(ball1Pos.x, ball1Pos.y);
		auto q2 = CreateQuadVertices(ball2Pos.x, ball2Pos.y);

		memcpy(vertices.data(), q1.data(), q1.size() * sizeof(Vertex));
		memcpy(vertices.data() + q1.size(), q2.data(), q2.size() * sizeof(Vertex));
		*/

		vb.Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	}

	

	void test::TestBatchRendering::OnImGuiRender()
	{
		//ImGui::SliderFloat("ball 1 position x", &ball1Pos.x, -40, 40);
		//ImGui::SliderFloat("ball 1 position y", &ball1Pos.y, -40, 40);
		//ImGui::SliderFloat("ball 2 position x", &ball2Pos.x, -50, 50);
		//ImGui::SliderFloat("ball 2 position y", &ball2Pos.y, -40, 40);

	}
	std::array<int, 6> TestBatchRendering::CreateQuadIndices(int quadIndex)
	{
		int idx1 = 0 + (quadIndex * 4);
		int idx2 = 1 + (quadIndex * 4);
		int idx3 = 3 + (quadIndex * 4);
		int idx4 = 0 + (quadIndex * 4);
		int idx5 = 3 + (quadIndex * 4);
		int idx6 = 2 + (quadIndex * 4);

		return {idx1, idx2, idx3 , idx4, idx5, idx6};
	}
}