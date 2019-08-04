#include "Light.h"

void RenLight::InitLight(int attr, float intensity, RenColor c, RenVector4D dir, RenVector4D loc)
{
	att = attr;
	I = intensity;
	color = c;
	direction = dir;
	location = loc;
}