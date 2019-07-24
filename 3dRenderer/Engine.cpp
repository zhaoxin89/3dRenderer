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

void RenEngine::Rasterization()
{
	RenPrimitive tmpPrimitive;
	for (int i = 0; i < numberOfPrimitives; i++)
	{
		if (renRenderingList[i].state == PRIMITAVE_STATE_ACTIVE)
		{
			tmpPrimitive = renRenderingList[i];
			DrawPrimitive(tmpPrimitive);
		}
	}
}

void RenEngine::DrawPrimitive (RenPrimitive &pri)
{
	RenPoint4D p1 = pri.p[0];
	RenPoint4D p2 = pri.p[1];
	RenPoint4D p3 = pri.p[2];
	
	if (pri.renderMode == PRIMITIVE_MODE_WIREFRAME)
	{
		DrawLine(p1.x, p1.y, p2.x, p2.y, pri.color);
		DrawLine(p1.x, p1.y, p3.x, p3.y, pri.color);
		DrawLine(p2.x, p2.y, p3.x, p3.y, pri.color);
	}
	
	else 
	{
		if (p1.y > p2.y)
			SwapPoint4D(&p1, &p2);
		if (p1.y > p3.y)
			SwapPoint4D(&p1, &p3);
		if (p2.y > p3.y)
			SwapPoint4D(&p2, &p3);
		if (p1.x > p2.x)
			SwapPoint4D(&p1, &p2);
		if (p3.x > p2.x)
			SwapPoint4D(&p2, &p3);
		
		

		if (p1.y == p2.y) 
			DrawPrimitiveFlatTop();
		else if (p2.y == p3.y)
			DrawPrimitiveFlatButton();
		else
		{
			//draw general triangle
		}
	}
		
}

void DrawPrimitiveFlatTop (RenPoint4D &p1, RenPoint4D &p2, RenPoint4D &p3, int mode)
{
	//assume that p1.y = p2.y
	float dy = p3.y - p1.y;
	float xStart = p1.x;
	float xEnd = p2.x;
	float yStart = p1.y;
	float yEnd = p3.y;
	float dxdyl = (p3.x - p1.x)/dy;
	float dxdyr = (p3.x - p2.x)/dy;

	RenColor tmpColor;
	for (float iy = yStart; iy < yEnd; iy++)
	{
		for (float ix = xStart; ix < xEnd; ix ++)
		{
			COLORREF color = RenColorToCOLORREF(tmpColor);
			SetPixel (hdc, ix, iy,color);
		}
		xStart += dxdyl;
		xEnd += dxdyr;
	}
}

void DrawPrimitiveFlatButton (RenPoint4D &p1, RenPoint4D &p2, RenPoint4D &p3)
{
	
}

void RenEngine::DrawLine (float x1, float y1, float x2, float y2, RenColor renColor)
{
	COLORREF color = RenColorToCOLORREF(renColor);
	if ((x2-x1)<(y2-y1))
	{
		float ix = x1;
		float dx = (x2 - x1)/(y2 - y1);
		for (int iy = y1; iy < y2; iy++)
		{
			SetPixel(hdc, ix, iy, color);
			ix += dx;
		}
	}
	else
	{
		float iy = y1;
		float dy = (y2-y1)/(x2-x1);
		for (int ix = x1; ix < x2; ix ++)
		{
			SetPixel(hdc, ix, iy, color);
			iy += dy;
		}
	}
}
RenColor BitMapFindColor (BitMap *map, float x, float y)
{
	
}