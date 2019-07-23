#include "Engine.h"

#include "MathLib.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

RenEngine* RenEngine::instance = 0;

RenEngine* RenEngine::GetInstance()
{
	if (instance == 0)
	{
		instance = new RenEngine();
	}
	return instance;
}

RenEngine::RenEngine()
{
	renCamera.InitCamera();
	loadObject();
	addLight();
}

void RenEngine::RenderLoop()
{
	GenerateRenderingList();
	PreRendering();
	Rendering();
	ReadKeyInput();
}

void RenEngine::LoadObject()
{
	char tmpName [] = "testTriangleObj";
	strcpy(renObject.name, tmpName);
	RenPoint4D p1(0, 5, 50, 1);
	RenPoint4D p2(5, 0, 50, 1);
	RenPoint4D p3(0, 0, 50, 1);
	renObject.pointList[0] = p1;
	renObject.pointList[1] = p2;
	renObject.pointList[2] = p3;
	RenTextile t1(0, 0);
	RenTextile t2(1, 1);
	RenTextile t3(0, 1);
	renObject.textileList[0] = t1;
	renObject.textileList[1] = t2;
	renObject.textileList[2] = t3;
	RenTiangle tmpTriangle(0,1,2,0,1,2);
	renObject.triangleList[0] = tmpTriangle;
}

void GenerateRenderingList()
{
}

void PreRendering()
{
	
}

void Rendering()
{
	LocalToWorld();
	Lighting();
	WorldToCamera();
	CameraToViewPort();
	Rasteration();
}

void ReadKeyInput()
{
	//update camera
	// if left --> camera.location.x -= 5;
	//etc.
	
}
