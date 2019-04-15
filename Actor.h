#pragma once
#include "Object.h"
#include "Core.h"


class Actor : public Object
{

public:
	Actor() : Object(){}
	Actor(D3DXVECTOR3 &loc) : Object(loc){}
	Actor(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale) : Object(loc, rot, scale){}

	virtual ~Actor(){}

	virtual void beginPlay();
	virtual void tick(float deltaTime);

};