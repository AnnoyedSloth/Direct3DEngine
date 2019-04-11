#pragma once
#include "Core.h"
#include "Object.h"

class Actor : public Object
{

public:
	Actor() : Object(){}
	Actor(D3DXVECTOR3 &loc) : Object(loc){}
	Actor(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale) : Object(loc, rot, scale){}

	virtual ~Actor(){}

	virtual void BeginPlay();
	virtual void Tick(float deltaTime);

};