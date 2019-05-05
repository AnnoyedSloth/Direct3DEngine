#include "Object.h"

Object::Object()
{
	location = Vector3(0, 0, 0);
	rotation = Vector3(0, 0, 0);
	scale = Vector3(0, 0, 0);
}

Object::Object(D3DXVECTOR3 &loc)
{
	location = loc;
	rotation = Vector3(0, 0, 0);
	scale = Vector3(0, 0, 0);
}

Object::Object(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale)
{
	location = loc;
	rotation = rot;
	this->scale = scale;
}

Object::~Object()
{
}