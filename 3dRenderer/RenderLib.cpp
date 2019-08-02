#include "RendererLib.h"
#include <cassert>
#include <string>

using namespace std;
RenVector4D CalculateCrossProduct(RenVector4D& u, RenVector4D& v)
{
	RenVector4D ret;
	ret.x = u.y * v.z - v.y * u.z;
	ret.y = u.x * v.z - v.x * u.z;
	ret.z = u.x * v.y - v.x * u.y;
	ret.w = 1;
	return ret;
}

void Vector4DNormalize(RenVector4D* v)
{
	float l = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x /= l;
	v->y /= l;
	v->z /= l;
}
void CalculateTriangleNormal(RenTriangle* tri)
{

}

RenMatrix4D CreateIdentityMatrix()
{
	RenMatrix4D ret;
	memset(&ret, 0, sizeof(ret));
	for (int i = 0; i < 4; i++)
		ret.M[i][i] = 1;
	return ret;
}
void SwapPoint4D(RenPoint4D p1, RenPoint4D p2)
{
	RenPoint4D tmp;
	tmp = p1;
	p1 = p2;
	p2 = p1;
}

COLORREF RenColorToCOLORREF(RenColor c)
{
	return RGB(c.red, c.green, c.blue);
}

RenVector4D fscanVector4(FILE* fp) {
	assert(fp);
	float arr[4];
	for (int i = 0; i < 4; i++) {
		fscanf(fp, "%f", &arr[i]);
	}
	return RenVector4D(arr);
}
RenVector3D fscanVector3(FILE* fp) {
	assert(fp);
	float arr[3];
	for (int i = 0; i < 3; i++) {
		fscanf(fp, "%f", &arr[i]);
	}
	RenVector3D ret;
	ret.SetValue(arr[0], arr[1], arr[2]);
	return ret;
}
RenVector2D fscanVector2(FILE* fp) {
	assert(fp);
	float arr[2];
	for (int i = 0; i < 2; i++) {
		fscanf(fp, "%f", &arr[i]);
	}
	return RenVector2D(arr);
}

RenVector4D vector3DTo4D(RenVector3D v3d)
{
	RenVector4D ret;
	ret.SetValue(v3d.x, v3d.y, v3d.z, 1);
	return ret;
}