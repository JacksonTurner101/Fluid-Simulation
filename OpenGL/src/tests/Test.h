#pragma once

#include "ImGui/imgui.h"
#include "Renderer.h"

namespace test {

	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnRender() {};
		virtual void OnUpdate(float deltaTime) {};
		virtual void OnImGuiRender() {};
	};
}