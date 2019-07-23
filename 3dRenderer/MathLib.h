#pragma once

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
}RenMatrix4D, * RenMatrix4D;

typedef struct RenTriangle
{
	RenPoint3D[3];
}RenTriangle, * RenTriangle;