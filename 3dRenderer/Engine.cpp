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
	RenColor c(255, 0, 0, 1);
	RenPoint4D p1(0, 5, 50, 1, c);
	RenPoint4D p2(5, 0, 50, 1, c);
	RenPoint4D p3(0, 0, 50, 1, c);
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
	tmpTriangle.normal = CalculateTriangleNormal(&tmpTriangle);
	renObject.triangleList[0] = tmpTriangle;
	renObject.numberOfTriangles = 1;
	
	numberOfObjects ++;
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
	Rasteration(RenTriangle* triangleList, int number);
}

void ReadKeyInput()
{
	//update camera
	// if left --> camera.location.x -= 5;
	//etc.
	
}

void RenEngine::Rasteration()
{
	RenTriangle tmpTriangle;
	RenObject tmpObject;
	for (int j = 0; j < numberOfObjects; j ++)
	{
		tmpObject = renObjectList[j];
		for (int i = 0; i < tmpObject.numberOfTriangles; i++)
		{
			tmpTriangle = tmpObject.triangleList[number];
			tmpObject.pointList[tmpTriangle.pointIndex[0]]
			DrawTriangle (&tmpTriangle);
		}
	}
}

void DrawTriangle (RenTriangle t)
{
	RenPoint4D p1 = t.pointListPtr[t.pointIndex[0]];
	RenPoint4D p2 = t.pointListPtr[t.pointIndex[1]];
	RenPoint4D p3 = t.pointListPtr[t.pointIndex[2]];
	if (p1.y > p2.y)
		SwapPoint4D(&p1, &p2);
	if (p1.y > p3.y)
		SwapPoint4D(&p1, &p3);
	if (p2.y > p3.y)
		SwapPoint4D(&p2, &p3);
	if (p1.y == p2.y) 
		DrawTriangleFlatTop();
	else if (p2.y == p3.y)
		DrawTriangleFlatButton();
	else
	{
		//draw general triangle
	}
		
}

void DrawTriangleFlatTop (RenPoint3D p1, RenPoint3D p2, RenPoint3D p3, RenColor c)
{
	
}

void DrawTriangleFlatButton (RenPoint3D p1, RenPoint3D p2, RenPoint3D p3, RenColor c)
{
	
}

RenColor BitMapFindColor (BitMap *map, float x, float y)
{
	
}