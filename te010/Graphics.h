#pragma once
#include <d3dx11effect.h>

class Graphics
{
private:
	Color                   m_ClearColor;
	IDXGISwapChain*         m_SwapChain;
	ID3D11Device*           m_Device;
	ID3D11DeviceContext*    m_Context;
	ID3D11InfoQueue*	    m_MessageQueue;
	ID3D11Texture2D*	    m_BackBuffer;
	ID3D11RenderTargetView* m_RenderTargetView;
	ID3DX11Effect*			m_Effect;
	ID3DX11EffectTechnique* m_Technique;
	ID3D11InputLayout*		m_InputLayout;

	ID3D11Buffer*			m_cbNeverChanges;
	ID3D11Buffer*			m_cbChangesOnResize;
	ID3D11Buffer*			m_cbChangesEveryFrame;

	Mat4x4 m_WorldMatrix;
	Mat4x4 m_ViewMatrix;
	Mat4x4 m_ProjectionMatrix;

private:
	bool CreateDeviceAndSwapChain();
	bool CreateViewport();
	bool CreateDefaultShader();
	bool CompileD3DShader(char* filePath, char* entry, char* shaderModel, ID3DBlob** buffer);

	void SetViewMatrix(Mat4x4 & matrix) { m_ViewMatrix = matrix; }
	void SetProjectionMatrix(Mat4x4 & matrix) { m_ProjectionMatrix = matrix; }

public:
	Graphics(void);
	~Graphics(void);

	bool Initialize(void);
	void Render(void);
	void Destroy(void);

	void SetWorldMatrix(Mat4x4 & matrix) { m_WorldMatrix = matrix; }
};

static Graphics* g_Graphics = 0;

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

//-------------------------------------------------------------------
//	Constant Buffers
//-------------------------------------------------------------------
struct CBNeverChanges
{
	D3DXMATRIX View;
};

struct CBChangeOnResize
{
	D3DXMATRIX Projection;
};

struct CBChangesEveryFrame
{
	D3DXMATRIX World;
};