#include "RendererLib.h"
#include <cassert>
#include <string>
#include "Utils.h"

using namespace std;
float CalculateDotProduct3D(const RenVector3D& u, const RenVector3D& v)
{
	float ret;
	ret = u.x * v.x + u.y * v.y + u.z * v.z;
	return ret;
}

RenVector3D CalculateCrossProduct3D(RenVector3D& u, RenVector3D& v)
{
	RenVector3D ret;
	ret.x = u.y * v.z - v.y * u.z;
	ret.y = v.x * u.z - u.x * v.z;
	ret.z = u.x * v.y - v.x * u.y;
	return ret;
}

RenVector4D CalculateCrossProduct4D(RenVector4D& u, RenVector4D& v)
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
	float tmp = v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w;
	float invsqrt = FastInverseSqrt(tmp);
	v->x *= invsqrt;
	v->y *= invsqrt;
	v->z *= invsqrt;
	v->w *= invsqrt;
}

void Vector3DNormalize(RenVector3D* v)
{
	float tmp = v->x * v->x + v->y * v->y + v->z * v->z;
	float invsqrt = FastInverseSqrt(tmp);
	v->x *= invsqrt;
	v->y *= invsqrt;
	v->z *= invsqrt;
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
void SwapVertex4D(RenVertex4DPtr v1, RenVertex4DPtr v2)
{
	RenVertex4D tmp;
	tmp = *v1;
	*v1 = *v2; 
	*v2 = tmp;
}

void SwapPoint4D(RenPoint4DPtr p1, RenPoint4DPtr p2)
{
	RenPoint4D tmp;
	tmp = *p1;
	*p1 = *p2;
	*p2 = *p1;
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

RenVector3D vector4DTo3D(RenVector4D v3d)
{
	RenVector3D ret;
	ret.SetValue(v3d.x, v3d.y, v3d.z);
	return ret;
}

void RotateAroundXAxis(RenVector4D& p, float rad)
{
	p.y = p.y * cos(rad) - p.z * sin(rad);
	p.z = p.y * sin(rad) + p.z * cos(rad);
}

void RotateAroundYAxis(RenVector4D& p, float rad)
{
	p.x = p.x * cos(rad) + p.z * sin(rad);
	p.z = - p.x * sin(rad) + p.z * cos(rad);
}

void RotateAroundZAxis(RenVector4D& p, float rad)
{
	p.x = p.x * cos(rad) - p.y * sin(rad);
	p.y = p.x * sin(rad) + p.y * cos(rad);
}

void RotateAroundXAxis(RenVector3D& p, float rad)
{
	p.y = p.y * cos(rad) - p.z * sin(rad);
	p.z = p.y * sin(rad) + p.z * cos(rad);
}

void RotateAroundYAxis(RenVector3D& p, float rad)
{
	p.x = p.x * cos(rad) + p.z * sin(rad);
	p.z = -p.x * sin(rad) + p.z * cos(rad);
}

void RotateAroundZAxis(RenVector3D& p, float rad)
{
	p.x = p.x * cos(rad) - p.y * sin(rad);
	p.y = p.x * sin(rad) + p.y * cos(rad);
}
void Interpolate(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y, float c[3])
{

	float a = (y2 - y3) * (x1 - x3) - (x2 - x3) * (y1 - y3);
	float U = ((y2 - y3) * (x - x3) - (y - y3) * (x2 - x3)) / a;
	float V = ((y - y3) * (x1 - x3) - (y1 - y3) * (x - x3)) / a;

	c[0] = U;
	c[1] = V;
	c[2] = 1 - U - V;
}
