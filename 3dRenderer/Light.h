#pragma once
#include"RendererLib.h"

#define RENLIGHT_ATT_AMBIENT 1
#define RENLIGHT_ATT_DIRECTIONAL 2
class RenLight
{
private:
	int att;
	RenColor color;
	RenVector4D direction;
	RenVector4D location;
public:

};