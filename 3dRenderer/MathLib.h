#pragma once

#define MAX_OBJECT_POINTS 256

typedef struct RenPoint2D
{
	float x, y;
}RenPoint2D, * RenPoint2DPtr;

typedef struct RenPoint3D
{
	float x, y, z;
}RenPoint3D, * RenPoint3DPtr;

typedef struct RenPoint4D
{
	float x, y, z, w;
	RenPoint4D(int x1, int y1, int z1, int w1)
	{
		x = x1; y = y1; z = z1; w = w1;
	}
}RenPoint4D, * RenPoint4DPtr;

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

}RenTriangle, * RenTrianglePtr;
typedef struct RenTextile
{
	int u, v;
}RenTextile, *RenTextilePtr;

typedef struct RenObject
{
	char name[64];
	RenPoint4D pointList[MAX_OBJECT_POINTS];
	RenTrianglePtr triangleList;
	RenTrianglePtr transferredTriangleList;
	RenTextilePtr textileList;
}RenObject, * RenObjectPtr;