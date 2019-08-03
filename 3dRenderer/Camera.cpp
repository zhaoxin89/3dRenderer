#include "Camera.h"
#include "RendererLib.h"
void RenCamera::InitCamera(float zN, float zF, RenVector3D loc)
{
	zn = zN;
	zf = zF;
	location = loc;
	up.SetValue(0, 1, 0);
	forward.SetValue(0, 0, 1);
	right.SetValue(1, 0, 0);
	CalculateCameraTrans();
}

void RenCamera::CalculateCameraTrans()
{
	Vector3DNormalize(&forward);
	up = CalculateCrossProduct3D(right, forward);
	Vector3DNormalize(&up);
	right = CalculateCrossProduct3D(forward, up);
	Vector3DNormalize(&right);
	
	memset(&cameraTrans, 0, sizeof(cameraTrans));
	cameraTrans.M[0][0] = right.x; cameraTrans.M[1][0] = right.y; cameraTrans.M[2][0] = right.z;
	cameraTrans.M[0][1] = up.x; cameraTrans.M[1][1] = up.y; cameraTrans.M[2][1] = up.z;
	cameraTrans.M[0][2] = forward.x; cameraTrans.M[1][2] = forward.y; cameraTrans.M[2][2] = forward.z;

	cameraTrans.M[3][0] = -CalculateDotProduct3D(location, right);
	cameraTrans.M[3][1] = -CalculateDotProduct3D(location, up);
	cameraTrans.M[3][2] = -CalculateDotProduct3D(location, forward);

	cameraTrans.M[3][3] = 1;
}

void RenCamera::Yaw(int speed)
{
	//RotateAroundXAxis(forward, speed / 100);
	//RotateAroundXAxis(right, speed / 100);
}
void RenCamera::Pitch(int speed)
{
	//RotateAroundXAxis(forward, speed / 100);
	//RotateAroundXAxis(up, speed / 100);
}