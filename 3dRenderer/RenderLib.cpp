#include "RendererLib.h"

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
	RenVector4D v1(tri->pointListPtr[tri->pointIndex[1]], tri->pointListPtr[tri->pointIndex[0]]);
	RenVector4D v2(tri->pointListPtr[tri->pointIndex[2]], tri->pointListPtr[tri->pointIndex[0]]);
	tri->normal = CalculateCrossProduct(v1, v2);
	Vector4DNormalize(&(tri->normal));
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
	COLORREF ret;
	DWORD cTmp;
	cTmp = (DWORD)((c.alpha) << 24) + (DWORD)((c.blue) << 16) + (DWORD)((c.green) << 8) + (DWORD)(c.red);
	ret = (DWORD)cTmp;
	return ret;
}