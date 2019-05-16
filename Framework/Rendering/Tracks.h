#pragma once

#include "Core/Core.h"
#include "AnimDefine.h"

using namespace std;

class Tracks
{

protected:
	Keys keys;
	D3DXMATRIXA16 matAni;

	float getAlpha(float f1, float f2, float frame) { return (frame - f1) / (f2 - f1); }

	void getKeys(float frame, std::vector<KeyPos>& tkeys, KeyPos*& k1, KeyPos*& k2);
	void getKeys(float frame, std::vector<KeyRot>& tkeys, KeyRot*& k1, KeyRot*& k2);
	void getKeys(float frame, std::vector<KeyScl>& tkeys, KeyScl*& k1, KeyScl*& k2);
	bool getPosKey(float frame, Vector3* pv);
	bool getRotKey(float frame, Quaternion* pq);
	bool getSclKey(float frame, Vector3* pv);

public:
	Tracks(Track& keys);
	~Tracks();

	D3DXMATRIXA16* animate(float frame);
	
};