#pragma once

class CriticalSection
{
public:
	CriticalSection()
	{
		InitializeCriticalSection( &m_cs);
	}

	~CriticalSection()
	{
		DeleteCriticalSection ( &m_cs);
	}

	void Lock()
	{
		EnterCriticalSection( &m_cs);
	}

	void Unlock()
	{
		LeaveCriticalSection( &m_cs);
	}

protected:
	mutable CRITICAL_SECTION m_cs;
};