#pragma once

#include "MathLib.h"
typedef struct RenCamera
{
	float forward, up, right;
	RenMatrix4D cameraMatrix;
}RenCamera, * RenCameraPtr;