#pragma once

#include "TestChangeColorUniform.h"

namespace test {

	test::TestChangeColorUniform::TestChangeColorUniform()
		: m_Color{ 0.5f,0.0f,1.0f, 1.0f },
		shader("res/shaders/defaultVert.shader", "res/shaders/changingColorsFrag.shader"),
		vao(),
		vb(),
		ib(),
		changeValR{ 1.0f },
		changeValG{ 1.0f },
		changeValB{ 1.0f }
	{

		vb.Bind();
		vao.Bind();
		ib.Bind();
		float vertices[8] = {
			//top left position 
			-0.5f,0.5f,
			//top right position
			0.5f,0.5f,
			//bottom left position
			-0.5f,-0.5f,
			//bottom right position
			0.5f,-0.5f
		};

		vb.AddData(vertices, 8 * sizeof(float));

		vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

		unsigned int indices[] = {
			0,1,3,
			0,3,2
		};

		ib.AddData(indices,6);

	}

	test::TestChangeColorUniform::~TestChangeColorUniform()
	{

	}

	void test::TestChangeColorUniform::OnUpdate(float deltaTime)
	{

		

		if (m_Color[0] <= 0.0f) {
			m_Color[0] = 0.0f;
			changeValR = 1;
		}
		else if (m_Color[0] >= 1.0f) {
			m_Color[0] = 1.0f;
			changeValR = -1;
		}

		if (m_Color[1] <= 0.0f) {
			m_Color[1] = 0.0f;
			changeValG = 1;
		}
		else if (m_Color[1] >= 1.0f) {
			m_Color[1] = 1.0f;
			changeValG = -1;
		}

		if (m_Color[2] <= 0.0f) {
			m_Color[2] = 0.0f;
			changeValB = 1;
		}
		else if (m_Color[2] >= 1.0f) {
			m_Color[2] = 1.0f;
			changeValB = -1;
		}

		m_Color[0] += changeValR * deltaTime;
		m_Color[1] += changeValG * deltaTime;
		m_Color[2] += changeValB * deltaTime;


		shader.Bind();
		shader.SetUniform4f("u_color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

	}

	void test::TestChangeColorUniform::OnRender()
	{
		shader.Bind();
		vao.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void test::TestChangeColorUniform::OnImGuiRender()
	{

	}

}