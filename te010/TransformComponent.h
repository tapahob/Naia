#pragma once
class TransformComponent : public ActorComponent
{
private:
	Mat4x4 m_transform;

public:

	static const char* g_Name;
	virtual const char* VGetName() const { return g_Name; }

	TransformComponent(void) : m_transform(Mat4x4::g_Identity) { }
	virtual bool VInit(TiXmlElement* pData) override;
	
	// transform funcs
	Mat4x4 GetTransform(void) const { return m_transform; }
	void   SetTransform(const Mat4x4 & newTransform) { m_transform = newTransform; }
	Vec3   GetPosition(void) { return m_transform.GetPosition(); }
	void   SetPosition(const Vec3 & newPosition) { m_transform.SetPosition(newPosition); }
	Vec3   GetLookAt(void) { return m_transform.GetDirection(); }

};

