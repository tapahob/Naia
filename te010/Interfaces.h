#pragma once

class Actor;

//-------------------------------------------------------------------
//	Typedefs
//-------------------------------------------------------------------

typedef D3DXCOLOR Color;

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

typedef shared_ptr<Actor> StrongActorPtr;
typedef weak_ptr<Actor> WeakActorPtr;

//-------------------------------------------------------------------
//	Constants
//-------------------------------------------------------------------

const float fOPAQUE      = 1.0f;
const float fTRANSPARENT = 0.0f;

static Color g_White       ( 1.0f , 1.0f,  1.0f,  fOPAQUE );	
static Color g_Black       ( 0.0f , 0.0f,  0.0f,  fOPAQUE );
static Color g_Cyan        ( 0.0f , 1.0f,  1.0f,  fOPAQUE );	
static Color g_Red         ( 1.0f , 0.0f,  0.0f,  fOPAQUE );
static Color g_Green       ( 0.0f , 1.0f,  0.0f,  fOPAQUE );
static Color g_Blue        ( 0.0f , 0.0f,  1.0f,  fOPAQUE );
static Color g_Yellow      ( 1.0f , 1.0f,  0.0f,  fOPAQUE );	
static Color g_Gray40      ( 0.4f , 0.4f,  0.4f,  fOPAQUE );
static Color g_Gray25      ( 0.25f, 0.25f, 0.25f, fOPAQUE );
static Color g_Gray65      ( 0.65f, 0.65f, 0.65f, fOPAQUE );
static Color g_Transparent ( 1.0f  , 0.0f,  1.0f, fTRANSPARENT );

const ActorId     INVALID_ACTOR_ID     = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

