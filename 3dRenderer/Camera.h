#pragma once
#include <stdio.h>
#include <iostream>
#include "RendererLib.h"

class RenCamera
{
private:
	RenPoint4D location;
	RenVector4D up, forward, right;
	float zn, zf;
	RenMatrix4D cameraTrans;
public:
	void InitCamera(float zN, float zF);
	void CalculateCameraTrans();
	void UpdateLocation(RenPoint4D& loc);
	void UpdateUp(RenVector4D& u);
	void UpdateForward(RenVector4D& f);
	void UpdateRight(RenVector4D& r);
};