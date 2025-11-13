#include "TestGridFluidSim2D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "iostream"
#include "algorithm"

namespace test {

	TestGridFluidSim2D::TestGridFluidSim2D() :
		vb(), ib(), vao(), shader("res/shaders/textureVert.shader", "res/shaders/textureFrag.shader"),
		fluidField(100, 0, 0, 0.01), size(100), cellSize(1), addDensity(false), direction(0,0)
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
				grid.push_back(cell);
				isWhite = !isWhite;
			}
			isWhite = !isWhite;
		}
		int width = size * cellSize;
		int height = size * cellSize;
		data = new unsigned char[width * height * 4];
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

		glGenTextures(1, &fluidTexture);
		glBindTexture(GL_TEXTURE_2D, fluidTexture);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		shader.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fluidTexture);
		shader.SetUniform1i("u_Texture", 0);

	}


	TestGridFluidSim2D::~TestGridFluidSim2D()
	{
		delete[] data;
	}

	void TestGridFluidSim2D::OnRender()
	{
		shader.Bind();
		vao.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

	}

	void TestGridFluidSim2D::Reset()
	{
		fluidField.Reset();

		// Reset grid colors to initial checkerboard pattern
		bool isWhite = true;
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				Color color{ 0, 0, 0 };
				if (isWhite) {
					color.red = 255;
					color.green = 255;
					color.blue = 255;
				}
				grid[y * size + x].color = color;
				isWhite = !isWhite;
			}
			isWhite = !isWhite;
		}
	}

	void test::TestGridFluidSim2D::OnUpdate(float deltaTime)
	{
		

		fluidField.FluidCubeStep();

		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				float densityValue = fluidField.density[y * size + x];

				int colorValue = static_cast<int>(std::clamp((densityValue / 1.0f) * 255.0f, 0.0f, 255.0f));

				grid[y * size + x].color.red = colorValue;
				grid[y * size + x].color.green = 0;
				grid[y * size + x].color.blue = 255 - colorValue;

			}
		}

		int width = size * cellSize;
		int height = size * cellSize;
		for (int i = 0; i < size * size; i++) {
			for (int y = 0; y < cellSize; y++) {
				for (int x = 0; x < cellSize; x++) {
					int pixelX = grid[i].x * cellSize + x;
					int pixelY = grid[i].y * cellSize + y;
					int index = (pixelY * width + pixelX) * 4;
					data[index + 0] = grid[i].color.red;
					data[index + 1] = grid[i].color.green;
					data[index + 2] = grid[i].color.blue;
					data[index + 3] = 255;
				}
			}
		}

		glBindTexture(GL_TEXTURE_2D, fluidTexture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
	}



	void TestGridFluidSim2D::OnImGuiRender()
	{
		ImGui::SliderFloat("X direction", &direction.x, -10, 10);
		ImGui::SliderFloat("Y direction", &direction.y, -10, 10);

		ImGui::Checkbox("checkbox", &addDensity);
		if (addDensity) {

			fluidField.FluidCubeAddDensity(50, 50, 2);
			fluidField.FluidCubeAddVelocity(50, 50, direction.x, direction.y);
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Reset Simulation (R)")) {
			Reset();
		}
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

	void FluidCube::Reset()
	{
		std::fill(s.begin(), s.end(), 0.0f);
		std::fill(density.begin(), density.end(), 0.0f);
		std::fill(Vx.begin(), Vx.end(), 0.0f);
		std::fill(Vy.begin(), Vy.end(), 0.0f);
		std::fill(Vx0.begin(), Vx0.end(), 0.0f);
		std::fill(Vy0.begin(), Vy0.end(), 0.0f);
	}

	void FluidCube::FluidCubeAddDensity(int x, int y, float amount)
	{
		int N = size;
		density.at(IX(x, y)) += amount;
	}

	void FluidCube::FluidCubeAddVelocity(int x, int y, float amountX, float amountY)
	{
		int N = size;
		int index = IX(x, y);

		Vx.at(index) += amountX;
		Vy.at(index) += amountY;

	}

	void FluidCube::FluidCubeStep()
	{
		int N = size;

		diffuse(1, Vx0, Vx, visc, dt, 4, N);
		diffuse(2, Vy0, Vy, visc, dt, 4, N);

		project(Vx0, Vy0, Vx, Vy, 4, N);

		advect(1, Vx, Vx0, Vx0, Vy0, dt, N);
		advect(2, Vy, Vy0, Vx0, Vy0, dt, N);

		project(Vx, Vy, Vx0, Vy0, 4, N);

		diffuse(0, s, density, diff, dt, 4, N);
		advect(0, density, s, Vx, Vy, dt, N);
	}

	void FluidCube::set_bnd(int b, std::vector<float>& x, int N)
	{
		
		
		for (int j = 1; j < N - 1; j++) {
			x[IX(0, j)] = b == 1 ? -x[IX(1, j)] : x[IX(1, j)];
			x[IX(N - 1, j)] = b == 1 ? -x[IX(N - 2, j)] : x[IX(N - 2, j)];
		}

		for (int i = 1; i < N - 1; i++) {
			x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
			x[IX(i, N - 1)] = b == 2 ? -x[IX(i, N - 2)] : x[IX(i, N - 2)];
		}

		x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
		x[IX(0, N - 1)] = 0.5f * (x[IX(1, N - 1)] + x[IX(0, N - 2)]);
		x[IX(N - 1, 0)] = 0.5f * (x[IX(N - 2, 0)] + x[IX(N - 1, 1)]);
		x[IX(N - 1, N - 1)] = 0.5f * (x[IX(N - 2, N - 1)] + x[IX(N - 1, N - 2)]);

	}

	void FluidCube::lin_solve(int b, std::vector<float>& x, std::vector<float>& x0, float a, float c, int iter, int N)
	{
		float cRecip = 1.0 / c;
		for (int k = 0; k < iter; k++) {
			for (int j = 1; j < N - 1; j++) {
				for (int i = 1; i < N - 1; i++) {
					x.at(IX(i, j)) =
					(x0.at(IX(i, j))
					+ a * (x.at(IX(i + 1, j))
					+ x.at(IX(i - 1, j))
					+ x.at(IX(i, j + 1))
					+ x.at(IX(i, j - 1))
					)) * cRecip;
				}
			}
			
			set_bnd(b, x, N);
		}
	}

	void FluidCube::diffuse(int b, std::vector<float>& x, std::vector<float>& x0, float diff, float dt, int iter, int N)
	{
		float a = dt * diff * (N - 2) * (N - 2);
		lin_solve(b, x, x0, a, 1 + 4 * a, iter, N);
	}

	void FluidCube::project(std::vector<float>& velocX, std::vector<float>& velocY, std::vector<float>& p, std::vector<float>& div, int iter, int N)
	{
		for (int j = 1; j < N - 1; j++) {
			for (int i = 1; i < N - 1; i++) {
				div[IX(i, j)] = -0.5f * (
					velocX[IX(i + 1, j)]
					- velocX.at(IX(i - 1, j))
						+ velocY[IX(i, j + 1)]
							- velocY[IX(i, j - 1)]
										) / N;
				p[IX(i, j)] = 0;
			}
		}
		
		set_bnd(0, div, N);
		set_bnd(0, p, N);
		lin_solve(0, p, div, 1, 4, iter, N);

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
		float dtx = dt * (N - 2);
		float dty = dt * (N - 2);
		float Nfloat = N;

		for (int j = 1; j < N - 1; j++) {
			for (int i = 1; i < N - 1; i++) {
				float x = i - dtx * velocX[IX(i, j)];
				float y = j - dty * velocY[IX(i, j)];

				if (x < 0.5f) x = 0.5f;
				if (x > Nfloat - 1.5f) x = Nfloat - 1.5f;
				if (y < 0.5f) y = 0.5f;
				if (y > Nfloat - 1.5f) y = Nfloat - 1.5f;

				int i0 = (int)floorf(x);
				int i1 = i0 + 1;
				int j0 = (int)floorf(y);
				int j1 = j0 + 1;

				float s1 = x - i0;
				float s0 = 1.0f - s1;
				float t1 = y - j0;
				float t0 = 1.0f - t1;

				d[IX(i, j)] =
					s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
					s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
			}
		}

		set_bnd(b, d, N);
	}
}