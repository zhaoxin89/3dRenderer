#pragma once

#include <stdio.h>
#include <stdlib.h>
#define MAX_OBJECT_POINTS 256
#define MAX_NUM_TRIANGLES 64
#define MAX_NUM_TEXTURES 2


#define PRIMITIVE_STATE_ACTIVE 0
#define PRIMITIVE_STATE_NON_ACTIVE 1

#define PRIMITIVE_MODE_WIREFRAME 0
#define PRIMITIVE_MODE_PURE_COLOR 1
#define PRIMITIVE_MODE_TEXTURE 2

typedef struct RenColor
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;

	RenColor(unsigned char r = 255, unsigned char g = 0, unsigned char b = 0, unsigned char a = 1)
	{
		red = r; green = g; blue = b; alpha = a;
	}
}RenColor, * RenColorPtr;

typedef struct RenPoint2D
{
	float x, y;
	RenPoint2D(float x1, float y1)
	{
		x = x1; y = y1;
	}
}RenPoint2D, * RenPoint2DPtr;

typedef struct RenPoint3D
{
	float x, y, z;
	RenPoint3D(float x1 = 0, float y1 = 0, float z1 = 0)
	{
		x = x1; y = y1; z = z1; 
	}
}RenPoint3D, RenVector3D, *RenVector3DPtr, * RenPoint3DPtr;

typedef struct RenPoint4D
{
	float x, y, z, w;
	RenColor color;
	RenPoint4D(float x1 = 1, float y1 = 0, float z1 = 0, float w1 = 1)
	{ 
		x = x1; y = y1; z = z1; w = w1;
	}
	RenPoint4D(RenPoint4D& p1, RenPoint4D& p2)
	{
		x = p2.x - p1.x;
		y = p2.y - p1.y;
		z = p2.z - p1.z;
		w = 1;
	}
	void SetValue(float x1, float y1, float z1, float w1)
	{
		x = x1; y = y1; z = z1; w = w1;
	}
	void SetColor(RenColor c)
	{
		color = c;
	}
}RenPoint4D, * RenPoint4DPtr, RenVector4D, * RenVector4DPtr;

typedef struct RenLine2D
{
	RenPoint2D p1, p2;
}RenLine2D, * RenLine2DPtr;

typedef struct RenLine3D
{
	RenPoint3D p1, p2;
}RenLine3D, * RenLine3DPtr;

typedef struct RenLine4D
{
	RenPoint4D p1, p2;
}RenLine4D, * RenLine4DPtr;

typedef struct RenMatrix4D
{
	float M[4][4];
}RenMatrix4D, * RenMatrix4DPtr;

typedef struct RenTriangle
{
	int pointIndex[3];
	int texIndex[3];
	RenVector4D normal;
	RenPoint4DPtr pointListPtr;
	RenTexturePtr textureListPtr;
	float angleWithSunLight;
	RenTriangle (int pi1 = 0, int pi2 = 1, int pi3 = 2, int ti1 = 0, int ti2 = 1, int ti3 = 2)
	{
		pointIndex[0] = pi1; pointIndex[1] = pi2; pointIndex[2] = pi3;
		texIndex[0] = ti1; texIndex[1] = ti2; texIndex[2] = ti3;
		normal.SetValue(1, 0, 0,1);
		pointListPtr = 0;
		textureListPtr = 0;
		angleWithSunLight = 0;
	}

}RenTriangle, * RenTrianglePtr;

typedef struct RenPrimitive
{
	RenPoint4D p[3];
	RenTexture t[3];
	RenColor c; // used for wireframe mode
	RenVector3D normal;
	//TODO: 
	//BitMapPtr *map;
	int renderMode;
	int state;
	float angleWithSunLight;
}RenPrimitive, * RenPrimitivePtr;

typedef struct RenTexture
{
	float u, v;
	RenTexture (float u1 = 0, float v1 = 0)
	{
		u = u1; v = v1;
	}
}RenTexture, *RenTexturePtr;

typedef struct RenObject
{
	char name[64];
	RenPoint4D pointList[MAX_OBJECT_POINTS];
	RenPoint4D transferredPointList[MAX_OBJECT_POINTS];
	RenTexture textureList[MAX_NUM_TEXTURES];
	RenTriangle triangleList[MAX_NUM_TRIANGLES];
	//RenTriangle transferredTriangleList[MAX_NUM_TRIANGLES];
	int numberOfPoints;
	int numberOfTriangles;
	float maxRadius;
}RenObject, * RenObjectPtr;

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