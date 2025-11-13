#include "TestLoadingOBJs.h"

test::TestLoadingOBJs::TestLoadingOBJs() : addDensity(true)
{

}

test::TestLoadingOBJs::~TestLoadingOBJs()
{

}

void test::TestLoadingOBJs::OnRender()
{

}

void test::TestLoadingOBJs::OnUpdate(float deltaTime)
{

}

void test::TestLoadingOBJs::OnImGuiRender()
{

	ImGui::Checkbox("checkbox", &addDensity);


	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

}
