#pragma once

class Graphics
{

public:
	Graphics(void);
	~Graphics(void);

	bool Initialize(void);
	void Render(void);
	void Destroy(void);
};

static Graphics* g_Graphics = NULL;

//----------------------------------------------------------------------------------------
// Render Passes
//----------------------------------------------------------------------------------------
enum RenderPass
{
	RenderPass_0,
	RenderPass_Static = RenderPass_0,
	RenderPass_Actor,
	RenderPass_Sky,
	RenderPass_NotRendered,
	RenderPass_Last
};