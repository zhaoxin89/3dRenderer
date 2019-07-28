#include "Light.h"

void RenLight::InitLight(int attr, RenColor c, RenVector4D dir, RenVector4D loc)
{
	att = attr;
	color = c;
	direction = dir;
	location = loc;
}