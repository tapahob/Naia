#include "precompiled.h"
#include "Application.h"

//-------------------------------------------------------------------
//	Construction
//-------------------------------------------------------------------
Input::Input(void)
{
}

Input::Input(const Input & other)
{
}

Input::~Input(void)
{
}

//-------------------------------------------------------------------
//	Initializer
//-------------------------------------------------------------------
void Input::Initialize(void)
{
	NAIA_INFO("Initializing the input system");
	// all the keys are released by default
	for(int i=0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	g_Input = this;
	return;
}

//-------------------------------------------------------------------
//	Modifiers
//-------------------------------------------------------------------

void Input::KeyDown(unsigned int key)
{
	m_keys[key] = true;
	return;
}

void Input::KeyUp(unsigned int key)
{
	m_keys[key] = false;
	return;
}

//-------------------------------------------------------------------
//	Accessors
//-------------------------------------------------------------------

bool Input::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}