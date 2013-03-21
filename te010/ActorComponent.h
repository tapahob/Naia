#pragma once

class ActorFactory;

//-------------------------------------------------------------------
//	Actor Component
//-------------------------------------------------------------------
class ActorComponent
{
	friend class ActorFactory;

protected:
	StrongActorPtr m_pOwner;

public:
	virtual ~ActorComponent(void) { m_pOwner.reset(); }

	virtual bool VInit(TiXmlElement* pData) =0;
	virtual void VPostInit(void) { }
	virtual void VUpdate(int deltaMs) { }
	virtual void VOnChanged(void) { }

	virtual ComponentId VGetId(void) const { return GetIdFromName(VGetName()); }
	virtual const char* VGetName() const =0;
	
	// TODO!
	static ComponentId GetIdFromName(const char* componentStr)
	{
		return NULL;
	}

private:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};