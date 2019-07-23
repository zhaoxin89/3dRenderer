#include "Engine.h"

#include "MathLib.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

void LoadObject();

void GenerateRenderingList();

void PreRendering();

void Rendering();

void ReadKeyInput();

void RenderLoop()
{
	LoadObject();
	GenerateRenderingList();
	PreRendering();
	Rendering();
	ReadKeyInput();
}

void LoadObject(RenObjectPtr pObj)
{
	char tmpName [] = "testTriangleObj";
	strcpy(pObj->name, tmpName);
	RenPoint4D p1(-5, 0, 5, 1);
	RenPoint4D p2(0, 5, 5, 1);
	RenPoint4D p3(5, 0, 5, 1);
	pObj->pointList[0] = p1;
	pObj->pointList[1] = p2;
	pObj->pointList[2] = p3;

}

void GenerateRenderingList()
{
}

void PreRendering()
{
}

void Rendering()
{
}

void ReadKeyInput()
{
}
