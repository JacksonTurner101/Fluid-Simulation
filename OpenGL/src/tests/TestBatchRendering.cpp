#include "TestBatchRendering.h"

test::TestBatchRendering::TestBatchRendering() :
	vb(),ib(),vao(), shader("res/shaders/vertex.shader", "res/shaders/fragment.shader")
{
	vb.ReserveSpace(sizeof(Vertex) * 1000, GL_DYNAMIC_DRAW);
	vao.Bind();
	


	//Index buffer still needs to be created
}

test::TestBatchRendering::~TestBatchRendering()
{

}

void test::TestBatchRendering::OnRender()
{

}

void test::TestBatchRendering::OnUpdate(float deltaTime)
{
	//use glBufferSubData here for updating vertices every frame

}

void test::TestBatchRendering::OnImGuiRender()
{

}
