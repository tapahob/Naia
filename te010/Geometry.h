#pragma once

const float NAIA_PI  = 3.14159265358979f;
const float NAIA_2PI = 2 * NAIA_PI;

typedef D3DXVECTOR2 Vec2;

//-------------------------------------------------------------------
//	Vec3 class description
//-------------------------------------------------------------------

class Vec3 : public D3DXVECTOR3
{
public:
	inline float Length() { return D3DXVec3Length(this); }
	inline Vec3* Normalize() { return static_cast<Vec3*>(D3DXVec3Normalize(this, this)); } 
	inline float Dot(const Vec3 & vector) { return D3DXVec3Dot(this, &vector); }
	inline Vec3  Cross(const Vec3 & vector) const;

	Vec3(D3DXVECTOR3 &vector3) { x = vector3.x; y = vector3.y; z =vector3.z; }
	Vec3() : D3DXVECTOR3() { x = 0; y =0; z = 0; }
	Vec3(const float _x, const float _y, const float _z) { x = _x; y = _y; z = _z; }
	Vec3(const double _x, const double _y, const double _z) { x = (float)_x, y = (float)_y, z = (float)_z; }
	inline Vec3(const class Vec4  &vector4);
};

inline Vec3 Vec3::Cross(const Vec3 & vector) const
{
	Vec3 out;
	D3DXVec3Cross(&out, this, &vector);
	return out;
}

//-------------------------------------------------------------------
//	Vec4 class description
//-------------------------------------------------------------------

class Vec4 : public D3DXVECTOR4
{
public:
	inline float Length() { return D3DXVec4Length(this); }
	inline Vec4* Normalize() { return static_cast<Vec4*>(D3DXVec4Normalize(this, this)); }
	inline float Dot(const Vec4 vector4) { return D3DXVec4Dot(this, &vector4); }

	Vec4(D3DXVECTOR4 &vector4) { x = vector4.x; y = vector4.y; z = vector4.z; w = vector4.w; }
	Vec4() : D3DXVECTOR4() { };
	Vec4(const float _x, const float _y, const float _z, const float _w) { x = _x, y = _y, z = _z, w = _w; }
	Vec4(const double _x, const double _y, const double _z, const double _w) { x = (float)_x; y = (float)_y; z = (float)_z; w = (float)_w; }
	Vec4(const Vec3 & vector3) { x = vector3.x; y = vector3.y; z = vector3.z; w = 1.0f; }
};

inline Vec3::Vec3(const Vec4 & vector4) { x = vector4.x; y = vector4.y; z = vector4.z; }


//-------------------------------------------------------------------
//	Globals
//-------------------------------------------------------------------
Vec3 g_Right(1.0f, 0.0f, 0.0f);
Vec3 g_Up(0.0f, 1.0f, 0.0f);
Vec3 g_Forward(0.0f, 0.0f, 1.0f);

Vec4 g_Up4(g_Up.x, g_Up.y, g_Up.z, 0.0f);
Vec4 g_Right4(g_Right.x, g_Right.y, g_Right.z, 0.0f);
Vec4 g_Forward4(g_Forward.x, g_Forward.y, g_Forward.z, 0.0f);

//-------------------------------------------------------------------
//	Vec3List, Vec4List description
//-------------------------------------------------------------------
typedef std::list<Vec3> Vec3List;
typedef std::list<Vec4> Vec4List;

//-------------------------------------------------------------------
//	Quaternion description
//-------------------------------------------------------------------
class Quaternion : public D3DXQUATERNION
{
public:
	void Normalize() { D3DXQuaternionNormalize(this, this); }

	// Spherical linear interpolation. coef [0, 1.0]
	void Slerp(const Quaternion &begin, const Quaternion &end, float coef) { D3DXQuaternionSlerp(this, &begin, &end, coef); }

	void GetAxisAngle(Vec3 & axis, float & angle) const { D3DXQuaternionToAxisAngle(this, &axis, &angle); }

	void Build(const class Mat4x4 &mat);
	void BuildRotYawPitchRoll(const float yawRadians, const float pitchRadians, const float rollRadians) { D3DXQuaternionRotationYawPitchRoll(this, yawRadians, pitchRadians, rollRadians); }
	void BuildAxisAngle(const Vec3 & axis, const float radians) { D3DXQuaternionRotationAxis(this, &axis, radians); }

	Quaternion(D3DXQUATERNION & quaternion) : D3DXQUATERNION(quaternion) { }
	Quaternion() { }

	static const Quaternion g_Identity;
};

inline Quaternion operator* (const Quaternion &a, const Quaternion &b)
{
	Quaternion out;
	D3DXQuaternionMultiply(&out, &a, &b);
}

//-------------------------------------------------------------------
//	Mat4x4 description
//-------------------------------------------------------------------
class Mat4x4 : public D3DXMATRIX
{
public:
	inline void SetPosition(Vec3 const & position);
	inline void SetPosition(Vec4 const & position);
	inline void SetScale(Vec3 const & scale);

	inline Vec3   GetPosition() const;
	inline Vec3   GetDirection() const;
	inline Vec3   GetUp() const;
	inline Vec3   GetRight() const;
	inline Vec3   GetYawPitchRoll() const;
	inline Vec3   GetScale() const;
	inline Vec4   Xform(Vec4 & vector) const;
	inline Vec3   Xform(Vec3 & vector) const;
	inline Mat4x4 Inverse() const;

	Mat4x4(D3DXMATRIX & matrix) { memccpy(&m, &matrix.m, sizeof(matrix.m)); }
	Mat4x4() : D3DXMATRIX() { };

	static const Mat4x4 g_Identity;

	inline void BuildTranslation(const Vec3 & vector);
	inline void BuildTranslation(const float x, const float y, const float z);
	inline void BuildRotationX(const float radians) { D3DXMatrixRotationX(this, radians); }
	inline void BuildRotationY(const float radians) { D3DXMatrixRotationY(this, radians); }
	inline void BuildRotationZ(const float radians) { D3DXMatrixRotationZ(this, radians); }
	inline void BuildYawPitchRoll(const float yawRadians, const float pitchRadians, const float rollRadians) { D3DXMatrixRotationYawPitchRoll(this, yawRadians, pitchRadians, rollRadians); }
	inline void BuildRotationQuat(const Quaternion & quaternion) { D3DXMatrixRotationQuaternion(this, &quaternion); }
	inline void BuildRotationLookAt(const Vec3 & eye, const Vec3 & at, const Vec3 & up) { D3DXMatrixLookAtRH(this, &eye, &at, &up); }
	inline void BuildScale(const float x, const float y, const float z);
};


inline void Mat4x4::SetPosition(Vec3 const & position)
{
	m[3][0] = position.x;
	m[3][1] = position.y;
	m[3][2] = position.z;
	m[3][3] = 1.0f;
}

inline void Mat4x4::SetPosition(Vec4 const & position)
{
	m[3][0] = position.x;
	m[3][1] = position.y;
	m[3][2] = position.z;
	m[3][3] = position.w;
}

inline void Mat4x4::SetScale(Vec3 const & scale)
{
	m[0][0] = scale.x;
	m[1][1] = scale.y;
	m[2][2] = scale.z;
}

inline Vec3 Mat4x4::GetPosition() const
{
	return Vec3(m[3][0], m[3][1], m[3][2]);
}

inline Vec3 Mat4x4::GetDirection() const
{
	Mat4x4 justRot = *this;
	justRot.SetPosition(Vec3(0.f,0.f,0.f));
	Vec3 forward = justRot.Xform(g_Forward);
	return forward;
}

inline Vec3 Mat4x4::GetRight() const
{
	Mat4x4 justRot = *this;
	justRot.SetPosition(Vec3(0.f,0.f,0.f));
	Vec3 right = justRot.Xform(g_Right);
	return right;
}

inline Vec3 Mat4x4::GetUp() const
{
	Mat4x4 justRot = *this;
	justRot.SetPosition(Vec3(0.f, 0.f, 0.f));
	Vec3 up = justRot.Xform(g_Up);
	return up;
}


inline Vec3 Mat4x4::GetYawPitchRoll() const
{
	float yaw, pitch, roll;

	pitch = asin(-_32);

	double threshold = 0.001;
	double test = cos(pitch);

	if(test > threshold)
	{
		roll = atan2(_12, _22);
		yaw = atan2(_31, _33);
	}
	else
	{
		roll = atan2(-_21, _11);
		yaw = 0.0;
	}

	return Vec3(yaw, pitch, roll);
}

inline Vec3 Mat4x4::GetScale() const
{
	return Vec3(m[0][0], m[1][1], m[2][2]);
}

inline Vec4 Mat4x4::Xform(Vec4 & vector) const
{
	Vec4 temp;
	D3DXVec4Transform(&temp, &vector, this);
	return temp;
}

inline Vec3 Mat4x4::Xform(Vec3 & vector) const
{
	Vec4 temp(vector);
	Vec4 out;
	D3DXVec4Transform(&out, &temp, this);
	return Vec3(out.x, out.y, out.z);
}

inline Mat4x4 Mat4x4::Inverse() const
{
	Mat4x4 out;
	D3DXMatrixInverse(&out, NULL, this);
	return out;
}

inline void Mat4x4::BuildTranslation(const Vec3 & position)
{
	*this = Mat4x4::g_Identity;
	m[3][0] = position.x;
	m[3][1] = position.y;
	m[3][2] = position.z;
}

inline void Mat4x4::BuildTranslation(const float x, const float y, const float z)
{
	*this = Mat4x4::g_Identity;
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
}

inline void Mat4x4::BuildScale(const float x, const float y, const float z)
{
	*this = Mat4x4::g_Identity;
	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
}

inline Mat4x4 operator * (const Mat4x4 &a, const Mat4x4 &b)
{
	Mat4x4 out;
	D3DXMatrixMultiply(&out, &a, &b);

	return out;
}

inline void Quaternion::Build(const class Mat4x4 &mat)
{
	D3DXQuaternionRotationMatrix(this, &mat);
}

//-------------------------------------------------------------------
//	Vecrtex types definitions
//-------------------------------------------------------------------

struct SimpleVertex
{
	Vec3 Pos;
	Vec3 Normal;
};

struct Vertex_UnlitTextured
{
	Vec3 Pos;
	Vec3 Normal;
	Vec2 Uv;
};

const D3D11_INPUT_ELEMENT_DESC VertexLayout_UnlitTextured[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

struct Vertex_UnlitTexturedColored
{
	Vec3 Pos;
	Vec3 Normal;
	Vec3 Diffuse;
	Vec2 Uv;
};

const D3D11_INPUT_ELEMENT_DESC VertexLayout_UnlitTexturedColored[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

//-------------------------------------------------------------------
//	Plane descriptions
//-------------------------------------------------------------------
class Plane : public D3DXPLANE
{
public:
	inline void Normalize();

	inline void Init(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3);
};

inline void Plane::Normalize()
{
	float mag;
	mag = sqrt(a*a + b*b + c*c);
	a = a / mag;
	b = b / mag;
	c = c / mag;
	d = d / mag;
}

inline void Plane::Init(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3)
{
	D3DXPlaneFromPoints(this, &p1, &p2, p3);
	Normalize();
}

//-------------------------------------------------------------------
//	Frustum definition
//-------------------------------------------------------------------
class Frustum
{
public:
	enum Side { Near, Far, Top, Right, Bottom, Left, NumPlanes };

	Plane m_Planes[NumPlanes];
	Vec3  m_NearClip[4];
	Vec3  m_FarClip[4];

	float m_Fov;
	float m_Aspect;
	float m_Near;
	float m_Far;

public:
	Frustum();

	void Init(const float fov, const float aspect, const float near, const float far);


	inline void Inside(const Vec3 & point, const float radius) const;
	inline void Inside(const Vec3 & point) const;

	const Plane & Get(Side side) { return m_Planes[side]; }

	void SetFOV(float fov) { m_Fov = fov; Init(m_Fov, m_Aspect, m_Near, m_Far); }
	void SetAspect(float aspect) { m_Aspect = aspect; Init(m_Fov, m_Aspect, m_Near, m_Far); }
	void SetNear(float near) { m_Near = near; Init(m_Fov, m_Aspect, m_Near, m_Far); }
	void SetFar(float far) { m_Far = far; Init(m_Fov, m_Aspect, m_Near, m_Far); }

	void Render();
};