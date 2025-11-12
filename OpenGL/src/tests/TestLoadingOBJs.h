#pragma once

#include "Test.h"

namespace test {

	class Object {

	};

	class TestLoadingOBJs : public Test {
	public:
		TestLoadingOBJs();
		~TestLoadingOBJs();

		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
	private:
		
	};

}