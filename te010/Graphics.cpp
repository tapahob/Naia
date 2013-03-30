#include "precompiled.h"
#include "Application.h"
#include <d3dcompiler.h>
#include "d3dx11effect.h"


Graphics::Graphics(void)
{
}


Graphics::~Graphics(void)
{
}

bool Graphics::CreateDeviceAndSwapChain()
{
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferCount                        = 1;
	sd.BufferDesc.Width                   = Application::instance()->m_Width;
	sd.BufferDesc.Height                  = Application::instance()->m_Height;
	sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator   = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow                       = Application::instance()->m_Hwnd;
	sd.SampleDesc.Count                   = 1;
	sd.SampleDesc.Quality                 = 0;
	sd.Windowed                           = !Application::instance()->m_IsFullScreen;

	HRESULT r = 0;
	r = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_Device, NULL, &m_Context);
	if (FAILED(r))
	{
		NAIA_FATAL("Unnable to create device and swap chain.");
	}

	/*r = m_Device->QueryInterface(__uuidof(ID3D11InfoQueue), (LPVOID*)&m_MessageQueue);
	if (FAILED(r))
	{
		NAIA_ERROR("Unnable to create D3D message queue");
	}
	m_MessageQueue->SetMuteDebugOutput(false);
	m_MessageQueue->SetMessageCountLimit(-1);*/

	r = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_BackBuffer);
	if (FAILED(r))
	{
		NAIA_FATAL("Unnable to get the back buffer");
	}

	r = m_Device->CreateRenderTargetView(m_BackBuffer, NULL, &m_RenderTargetView);
	if (FAILED(r))
	{
		NAIA_FATAL("Unnable to create the RTV");
	}

	m_Context->OMSetRenderTargets(1, &m_RenderTargetView, NULL);
	return true;
}

bool Graphics::CreateViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)Application::instance()->m_Width;
	vp.Height = (float)Application::instance()->m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_Context->RSSetViewports(1, &vp);
	return true;
}

bool Graphics::CreateDefaultShader()
{
	HRESULT r = 0;

	ID3DBlob* vsBuffer = 0;
	ID3DBlob* buffer = 0;

	ID3DBlob* Errors = 0;

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	bool b = CompileD3DShader("..\\te010\\DefaultShader.fx", 0, "fx_5_0", &buffer);

	if (!b)
	{
		if (buffer)
		{
			buffer->Release();
		}
		NAIA_FATAL("Unnable to compile default shader");
	}

	r = D3DX11CreateEffectFromMemory(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0 , m_Device, &m_Effect);
	if (FAILED(r))
	{
		NAIA_FATAL("Unnable to create effect");
	}

	m_Technique = m_Effect->GetTechniqueByName("DefaultTechnique");

	if (!m_Technique)
	{
		NAIA_FATAL("Can not find DefaultTechnique in default shader");
	}

	D3D11_INPUT_ELEMENT_DESC defaultLayout[] =
	{
		{"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT,    0,  0,                             D3D11_INPUT_PER_VERTEX_DATA,    0},
		{"NORMAL",   0,  DXGI_FORMAT_R32G32B32_FLOAT,    0,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA,    0},
		{"COLOR",    0,  DXGI_FORMAT_R32G32B32_FLOAT,    0,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA,    0},
		{"TEXCOORD", 0,  DXGI_FORMAT_R32G32B32_FLOAT,    0,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA,    0}
	};

	unsigned int totalLayoutElements = ARRAYSIZE(defaultLayout);

	D3DX11_PASS_DESC descPASS;
	m_Technique->GetPassByIndex(0)->GetDesc(&descPASS);

	r = m_Device->CreateInputLayout(defaultLayout, totalLayoutElements, descPASS.pIAInputSignature, descPASS.IAInputSignatureSize, &m_InputLayout);
	if (FAILED(r))
	{
		NAIA_FATAL("Failed to create input layout");
	}

	m_Context->IASetInputLayout(m_InputLayout);

	Mat4x4 world = Mat4x4::g_Identity;
	SetWorldMatrix(world);

	Mat4x4 view = Mat4x4::g_Identity;
	Vec3 eye(6.0f, 6.0f, 6.0f);
	Vec3 at(0.f,0.f,0.f);
	Vec3 up(0.f,1.0f,0.f);
	D3DXMatrixLookAtLH(&view, &eye, &at, &up);
	SetViewMatrix(view);

	Mat4x4 proj = Mat4x4::g_Identity;
	D3DXMatrixPerspectiveFovLH(&proj, NAIA_PI/2, Application::instance()->m_Width/(float)Application::instance()->m_Height, 0.01f, 100.0f);
	SetProjectionMatrix(proj);

#pragma region Create constant buffers

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBNeverChanges);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0; 
	r = m_Device->CreateBuffer(&bd, NULL, &m_cbNeverChanges);
	if (FAILED(r))
	{
		NAIA_FATAL("Can not create constant buffer (Never changes)");
	}

	bd.ByteWidth = sizeof(CBChangeOnResize);
	r = m_Device->CreateBuffer(&bd, NULL, &m_cbChangesOnResize);
	if (FAILED(r))
	{
		NAIA_FATAL("Can not create constant buffer (Changes on resize)");
	}

	bd.ByteWidth = sizeof(CBChangesEveryFrame);
	r = m_Device->CreateBuffer(&bd, NULL, &m_cbChangesEveryFrame);
	if (FAILED(r))
	{
		NAIA_FATAL("Can not create constant buffer (Changes every frame)");
	}
#pragma endregion

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.View = view;
	m_Context->UpdateSubresource(m_cbNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.Projection = proj;
	m_Context->UpdateSubresource(m_cbChangesOnResize, 0, NULL, &cbChangesOnResize, 0, 0);

	CBChangesEveryFrame cbChangesEveryFrame;
	cbChangesEveryFrame.World = world;
	m_Context->UpdateSubresource(m_cbChangesEveryFrame, 0, NULL, &cbChangesEveryFrame, 0, 0);

	return true;
}

bool Graphics::Initialize()
{
	g_Graphics   = this;
	m_ClearColor = g_White;

	CreateDeviceAndSwapChain();
	CreateViewport();
	CreateDefaultShader();

	return true;
}

void Graphics::Destroy(void)
{

	if (m_Context) m_Context->ClearState();
	if (m_cbChangesEveryFrame) m_cbChangesEveryFrame->Release();
	if (m_cbChangesOnResize) m_cbChangesOnResize->Release();
	if (m_cbNeverChanges) m_cbNeverChanges->Release();
	if (m_InputLayout) m_InputLayout->Release();
	if (m_RenderTargetView) m_RenderTargetView->Release();
	if (m_BackBuffer) m_BackBuffer->Release();
	if (m_Effect) m_Effect->Release();
	if (m_SwapChain) m_SwapChain->Release();
	if (m_Context) m_Context->Release();
	if (m_Device) m_Device->Release();

	if(g_Graphics)
	{
		delete g_Graphics;
		g_Graphics = 0;
	}
}

void Graphics::Render(void)
{
	m_Context->ClearRenderTargetView(m_RenderTargetView, m_ClearColor);

	m_SwapChain->Present(0, 0);
}

bool Graphics::CompileD3DShader( char* filePath, char* entry, char* shaderModel, ID3DBlob** buffer )
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	ID3DBlob* errorBuffer =0 ;
	HRESULT result = 0;

	result = D3DX11CompileFromFileA(filePath, 0, 0 , entry, shaderModel, shaderFlags, 0, 0, buffer, &errorBuffer, 0);

	if (FAILED(result))
	{
		if (errorBuffer != 0)
		{
			NAIA_ERROR("Unnable to compile shader!");
			NAIA_ERROR((char*) errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}

		return false;
	}

	if (errorBuffer != 0)
		errorBuffer->Release();

	return true;
}


