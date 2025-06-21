#include "TestGridFluidSim2D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "iostream"
#include "algorithm"

namespace test {

	TestGridFluidSim2D::TestGridFluidSim2D() :
		vb(), ib(), vao(), shader("res/shaders/defaultVert.shader", "res/shaders/densityFrag.shader"),
		fluid(100, 1, 1, 1), fluidTexture(0)
	{
		vb.Bind();
		vao.Bind();
		ib.Bind();
		float vertices[16] = {
			//top left position   UV Coordinates
			-1.0f,1.0f,			  0.0f, 1.0f,//0
			//top right position
			1.0f,1.0f,			  1.0f,1.0f,//1     
			//bottom left position
			-1.0f,-1.0f,		  0.0f,0.0f,//2
			//bottom right position
			1.0f,-1.0f,			  1.0f,0.0f//3
		};

		vb.AddData(vertices, 16 * sizeof(float));

		vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));

		unsigned int indices[] = {
			0,1,3,
			0,3,2
		};

		ib.AddData(indices, 6);

		// creating a texture
		glGenTextures(1, &fluidTexture);
		glBindTexture(GL_TEXTURE_2D, fluidTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, fluid.size, fluid.size, 0, GL_RED, GL_FLOAT, nullptr);

		// Texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

	}


	TestGridFluidSim2D::~TestGridFluidSim2D()
	{

	}

	void TestGridFluidSim2D::OnRender()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fluidTexture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, fluid.size, fluid.size, GL_RED, GL_FLOAT, &fluid.density[0]);

		shader.Bind();
		shader.SetUniform1i("u_DensityTexture", 0);
		vao.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

	}

	void test::TestGridFluidSim2D::OnUpdate(float deltaTime, GLFWwindow* window)
	{

		//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);

			// Convert screen coords to sim coords (assuming windowWidth/windowHeight is known)
			int simX = (int)((xpos / 800) * fluid.size);
			int simY = (int)(((800 - ypos) / 800) * fluid.size);

			simX = std::clamp(simX, 0, fluid.size - 1);
			simY = std::clamp(simY, 0, fluid.size - 1);
			// Inject density at simX, simY

		//}
		
		fluid.FluidCubeAddDensity(simX, simY, 1.0f);

		fluid.FluidCubeStep();
	}



	void TestGridFluidSim2D::OnImGuiRender()
	{

	}

	FluidCube::FluidCube(int size, int diffusion, int viscosity, float dt) :
		size(size), dt(dt), diff(diffusion), visc(viscosity),
		s(size * size, 0.0f),
		density(size * size, 0.0f),
		Vx(size * size, 0.0f),
		Vy(size * size, 0.0f),
		Vx0(size * size, 0.0f),
		Vy0(size * size, 0.0f)

	{
		
	}

	FluidCube::~FluidCube()
	{
	}

	void FluidCube::FluidCubeAddDensity(int x, int y, float amount)
	{
		int N = size;
		density[IX(x, y)] += amount;
	}

	void FluidCube::FluidCubeAddVelocity(int x, int y, float amountX, float amountY)
	{
		int N = size;
		int index = IX(x, y);

		Vx[index] += amountX;
		Vy[index] += amountY;

	}

	void FluidCube::FluidCubeStep()
	{
		int N = size;

		diffuse(1, Vx0, Vx, visc, dt, 4, N);

		project(Vx0, Vy0, Vx, Vy, 4, N);

		advect(1, Vx, Vx0, Vx0, Vy0, dt, N);
		advect(2, Vy, Vy0, Vx0, Vy0, dt, N);

		project(Vx, Vy, Vx0, Vy0, 4, N);

		diffuse(0, s, density, diff, dt, 4, N);
		advect(0, density, s, Vx, Vy, dt, N);
	}

	void FluidCube::set_bnd(int b, std::vector<float>& x, int N)
	{
		
		for (int i = 1; i < N - 1; i++) {
			x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
			x[IX(i, N - 1)] = b == 2 ? -x[IX(i, N - 2)] : x[IX(i, N - 2)];
		}
		
		for (int j = 1; j < N - 1; j++) {
			x[IX(0, j)] = b == 1 ? -x[IX(1, j)] : x[IX(1, j)];
			x[IX(N - 1, j)] = b == 1 ? -x[IX(N - 2, j)] : x[IX(N - 2, j)];
		}
		
		x[IX(0, 0)] = 0.5 * (x[IX(1, 0)] + x[IX(0, 1)]);
		x[IX(0, N - 1)] = 0.5 * (x[IX(1, N - 1)] + x[IX(0, N-2)]);
		x[IX(N - 1, 0)] = 0.5 * (x[IX(N-2, 0)] + x[IX(N - 1, 1)]);
		x[IX(N - 1, N - 1)] = 0.5 * (x[IX(N-2, N - 1)] + x[IX(N - 1, N-2)]);

	}

	void FluidCube::lin_solve(int b, std::vector<float>& x, std::vector<float>& x0, float a, float c, int iter, int N)
	{
		float cRecip = 1.0 / c;
		for (int k = 0; k < iter; k++) {
			for (int j = 1; j < N - 1; j++) {
				for (int i = 1; i < N - 1; i++) {
					x[IX(i, j)] =
					(x0[IX(i, j)]
					+ a * (x[IX(i + 1, j)]
					+ x[IX(i - 1, j)]
					+ x[IX(i, j + 1)]
					+ x[IX(i, j - 1)]
					)) * cRecip;
				}
			}
			
			set_bnd(b, x, N);
		}
	}

	void FluidCube::diffuse(int b, std::vector<float>& x, std::vector<float>& x0, float diff, float dt, int iter, int N)
	{
		float a = dt * diff * (N - 2) * (N - 2);
		lin_solve(b, x, x0, a, 1 + 6 * a, iter, N);
	}

	void FluidCube::project(std::vector<float>& velocX, std::vector<float>& velocY, std::vector<float>& p, std::vector<float>& div, int iter, int N)
	{
		for (int j = 1; j < N - 1; j++) {
			for (int i = 1; i < N - 1; i++) {
				div[IX(i, j)] = -0.5f * (
					velocX[IX(i + 1, j)]
					- velocX[IX(i - 1, j)]
						+ velocY[IX(i, j + 1)]
							- velocY[IX(i, j - 1)]
										) / N;
				p[IX(i, j)] = 0;
			}
		}
		
		set_bnd(0, div, N);
		set_bnd(0, p, N);
		lin_solve(0, p, div, 1, 6, iter, N);

		for (int j = 1; j < N - 1; j++) {
			for (int i = 1; i < N - 1; i++) {
				velocX[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)]
					- p[IX(i - 1, j)]) * N;
				velocY[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)]
					- p[IX(i, j - 1)]) * N;
			}
		}
		
		set_bnd(1, velocX, N);
		set_bnd(2, velocY, N);
	}

	void FluidCube::advect(int b, std::vector<float>& d, std::vector<float>& d0, std::vector<float>& velocX, std::vector<float>& velocY, float dt, int N)
	{
		float i0, i1, j0, j1;

		float dtx = dt * (N - 2);
		float dty = dt * (N - 2);

		float s0, s1, t0, t1;
		float tmp1, tmp2, x, y;

		float Nfloat = N;
		float ifloat, jfloat;
		int i, j;

			for (j = 1, jfloat = 1; j < N - 1; j++, jfloat++) {
				for (i = 1, ifloat = 1; i < N - 1; i++, ifloat++) {
					tmp1 = dtx * velocX[IX(i, j)];
					tmp2 = dty * velocY[IX(i, j)];
					x = ifloat - tmp1;
					y = jfloat - tmp2;

					if (x < 0.5f) x = 0.5f;
					if (x > Nfloat + 0.5f) x = Nfloat + 0.5f;
					i0 = floorf(x);
					i1 = i0 + 1.0f;
					if (y < 0.5f) y = 0.5f;
					if (y > Nfloat + 0.5f) y = Nfloat + 0.5f;
					j0 = floorf(y);
					j1 = j0 + 1.0f;
					

					s1 = x - i0;
					s0 = 1.0f - s1;
					t1 = y - j0;
					t0 = 1.0f - t1;

					int i0i = i0;
					int i1i = i1;
					int j0i = j0;
					int j1i = j1;


					d[IX(i, j)] =
						s0 * (t0 * d0[IX(i0i, j0i)])
						+ (t1 * d0[IX(i0i, j1i)])
						+ s1 * (t0 * d0[IX(i1i, j0i)])
						+ (t1 * d0[IX(i1i, j1i)]);
				}
			}
		set_bnd(b, d, N);
	}
}