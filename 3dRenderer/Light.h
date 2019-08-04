#pragma once
#include"RendererLib.h"

#define RENLIGHT_ATT_AMBIENT 1
#define RENLIGHT_ATT_DIRECTIONAL 2
class RenLight
{
public:
	int att;
	float I;
	RenColor color;
	RenVector4D direction;
	RenVector4D location;
public:
	void InitLight(int att, float intensity, RenColor c, RenVector4D dir, RenVector4D loc);
};