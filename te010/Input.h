#pragma once
class Input
{
public:
	Input(void);
	Input(const Input&);
	~Input(void);

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};

// Global pointer
static Input* g_Input = 0;

