#pragma once
#pragma warning(disable:4005)

// C runtime headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <map>
#include <memory>

// DirectX headers
#include <d3d11.h>
#include <d3dx11.h>
#include <D3DX10math.h>

// 3rdParty includes
#pragma comment(lib, "../3rdParty/tinyxml_2_6_2/Debugtinyxml/tinyxml.lib")
#include "../3rdParty/tinyxml_2_6_2/tinyxml.h"

using std::tr1::shared_ptr;
using std::tr1::weak_ptr;

// Naia includes
#include "Geometry.h"
#include "string.h"
#include "CriticalSection.h"
#include "Logger.h"
#include "Input.h"
#include "Interfaces.h"
#include "Actor.h"
#include "ActorComponent.h"
#include "TransformComponent.h"
#include "Graphics.h"
#include "Material.h"