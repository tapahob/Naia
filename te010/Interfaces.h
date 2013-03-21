#pragma once

class Actor;

//-------------------------------------------------------------------
//	Typedefs
//-------------------------------------------------------------------

typedef unsigned int ActorId;
typedef unsigned int ComponentId;

typedef shared_ptr<Actor> StrongActorPtr;
typedef weak_ptr<Actor> WeakActorPtr;