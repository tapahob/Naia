#pragma once
//-------------------------------------------------------------------
//	Material
//-------------------------------------------------------------------
class Material
{
private:
	Color m_Ambient;
	Color m_Diffuse;
	Color m_Specular;
	Color m_Emissive;

public:
	Material() : m_Ambient(Color(0.10f, 0.10f, 0.10f, 1.0f)), m_Diffuse(g_White), m_Specular(g_White), m_Emissive(g_Black) { }

	void SetAmbient(const Color & color) { m_Ambient = color; }
	const Color GetAmbient() { return m_Ambient; }

	void SetDiffuse(const Color & color) { m_Diffuse = color; }
	const Color GetDiffuse() { return m_Diffuse; }

	void SetSpecular(const Color & color) { m_Specular = color; }
	const Color GetSpecular() { return m_Specular; }

	void SetEmissive(const Color & color) { m_Emissive = color; }
	const Color GetEmissive() { return m_Emissive; }

	void SetAlpha(const float alpha) { m_Diffuse.a = alpha; }
	float GetAlpha() const { return m_Diffuse.a; }
	bool HasAlpha() const { return GetAlpha() != fOPAQUE; }
};