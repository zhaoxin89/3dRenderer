#include "Camera.h"
#include "RendererLib.h"
void RenCamera::InitCamera(float zN, float zF)
{
	zn = zN;
	zf = zF;
	up.SetValue(0, 1, 0, 1);
	forward.SetValue(0, 0, 1, 1);
	right.SetValue(1, 0, 0, 1);
	CalculateCameraTrans();
}

void RenCamera::CalculateCameraTrans()
{
	RenMatrix4D m1 = CreateIdentityMatrix();
	m1.M[3][0] = -location.x;
	m1.M[3][1] = -location.y;
	m1.M[3][2] = -location.z;
	//TODO: rotation
	cameraTrans = m1;
}