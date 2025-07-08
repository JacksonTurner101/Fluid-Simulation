#pragma once

#include "Test.h"
#include "glm/glm.hpp" 
#include <array>
#include <glfw3.h>


#define IX(x, y) ((x) + (y) * (N))

namespace test {

	struct Color {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	};

	struct GridCell {
		int x;
		int y;
		int size;
		Color color;
	};

	struct FluidCube {
		int size;
		float dt;
		float diff;
		float visc;

		std::vector<float> s; //velocity field
		std::vector<float> density;

		std::vector<float> Vx;
		std::vector<float> Vy;

		std::vector<float> Vx0;
		std::vector<float> Vy0;

		FluidCube(int size, int diffusion, int viscosity, float dt);
		~FluidCube();

		void FluidCubeAddDensity(int x, int y, float amount);
		void FluidCubeAddVelocity(int x, int y, float amountX, float amountY);
		void FluidCubeStep();
		static void set_bnd(int b, std::vector<float>& x, int N);
		static void lin_solve(int b, std::vector<float>& x, std::vector<float>& x0, float a, float c, int iter, int N);
		static void diffuse(int b, std::vector<float>& x, std::vector<float>& x0, float diff, float dt, int iter, int N);
		static void project(std::vector<float>& velocX, std::vector<float>& velocY, std::vector<float>& p, std::vector<float>& div, int iter, int N);
		static void advect(int b, std::vector<float>& d, std::vector<float>& d0, std::vector<float>& velocX, std::vector<float>& velocY, float dt, int N);

	};

	class TestGridFluidSim2D : public Test {
	public:
		TestGridFluidSim2D();
		~TestGridFluidSim2D();

		void OnRender() override;
		void OnUpdate(float deltaTime);
		void OnImGuiRender() override;

	private:
		VertexArray vao;
		VertexBuffer vb;
		IndexBuffer ib;
		Shader shader;
		FluidCube fluidField;
		unsigned int fluidTexture;
		unsigned char* data;
		std::vector<GridCell> grid;
		int size;
		int cellSize;
		bool addDensity;
		glm::vec2 direction;

	};
}