#include "precompiled.h"
#include "Graphics.h"


Graphics::Graphics(void)
{
}


Graphics::~Graphics(void)
{
}

bool Graphics::Initialize(void)
{
	g_Graphics = this;
	return true;
}

void Graphics::Destroy(void)
{
	if(g_Graphics)
	{
		delete g_Graphics;
		g_Graphics = 0;
	}
}

void Graphics::Render(void)
{

}