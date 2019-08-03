#pragma once
#include <stdio.h>
#include <iostream>
#include "RendererLib.h"

class RenCamera
{
public:
	RenVector3D location;
	RenVector3D up, forward, right;
	float zn, zf;
	RenMatrix4D cameraTrans;
public:
	void InitCamera(float zN, float zF, RenVector3D loc);
	void CalculateCameraTrans();
	void UpdateLocation(RenVector3D& loc);
	void UpdateUp(RenVector3D& u);
	void UpdateForward(RenVector3D& f);
	void UpdateRight(RenVector3D& r);
	void Yaw(int speed);
	void Pitch(int speed);
};