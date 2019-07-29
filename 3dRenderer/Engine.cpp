#include "Engine.h"

#include "RendererLib.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <wingdi.h>

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
	hdc = 0;
}

void RenEngine::RenderInit(HDC h)
{
	hdc = h;
	LoadTriangleObject();
	renCamera.InitCamera(10,10000);
	RenColor ambientColor(255,255,255,1);
	RenVector4D dir(0, 0, 1, 1);
	RenVector4D loc(0, 0, -50, 1);
	AddLight(RENLIGHT_ATT_AMBIENT, ambientColor, dir, loc);
	RenColor directionalColor(255,255,255,1);
	AddLight (RENLIGHT_ATT_DIRECTIONAL, directionalColor, dir, loc);
	//LoadBMP();
}
void RenEngine::RenderLoop()
{
	RenVector4D worldPos(0,0,0,1);
	//LocalToProjectionTransformation(worldPos);
	//TODO: clipping
	GenerateRenderingList();
	PreRendering();
	Rendering();
	ReadKeyInput();
}

void RenEngine::GenerateRenderingList()
{
	RenObject tmpObject;
	for (int i = 0; i < numberOfObjects; i++)
	{
		tmpObject = renObjectList[i];
		for (int j = 0; j < tmpObject.numberOfTriangles; j++)
		{
			RenTriangle tri = tmpObject.triangleList[j];
			RenPrimitive p;
			p.p[0] = tmpObject.pointList[tri.pointIndex[0]];
			p.p[1] = tmpObject.pointList[tri.pointIndex[1]];
			p.p[2] = tmpObject.pointList[tri.pointIndex[2]];
			p.t[0] = tmpObject.textureList[tri.texIndex[0]];
			p.t[1] = tmpObject.textureList[tri.texIndex[1]];
			p.t[2] = tmpObject.textureList[tri.texIndex[2]];
			renRenderingList[numberOfPrimitives] = p;
			numberOfPrimitives++;
		}
	}
}
void RenEngine::PreRendering()
{
}
void RenEngine::RenderExit()
{
}

void RenEngine::AddLight(int att, RenColor c, RenVector4D dir, RenVector4D loc)
{
	renLight[numberOfLights].InitLight (att,c,dir,loc);
	numberOfLights++;
}

void RenEngine::LoadTriangleObject()
{
	char tmpName [] = "testTriangleObj";
	//strcpy(renObjectList[numberOfObjects].name, tmpName);
	RenColor c(255, 0, 0, 1); //red
	RenPoint4D p1(0, 5, 50, 1);
	RenPoint4D p2(5, 0, 50, 1);
	RenPoint4D p3(0, 0, 50, 1);
	p1.SetColor(c);
	p2.SetColor(c);
	p3.SetColor(c);
	renObjectList[numberOfObjects].pointList[0] = p1;
	renObjectList[numberOfObjects].pointList[1] = p2;
	renObjectList[numberOfObjects].pointList[2] = p3;
	RenTexture t1(0, 0);
	RenTexture t2(1, 1);
	RenTexture t3(0, 1);
	renObjectList[numberOfObjects].textureList[0] = t1;
	renObjectList[numberOfObjects].textureList[1] = t2;
	renObjectList[numberOfObjects].textureList[2] = t3;
	RenTriangle tmpTriangle(0,1,2,0,1,2);
	
	tmpTriangle.pointListPtr = (renObjectList[numberOfObjects].transferredPointList);
	CalculateTriangleNormal(&tmpTriangle);

	renObjectList[numberOfObjects].triangleList[0] = tmpTriangle;
	renObjectList[numberOfObjects].numberOfPoints = 3;
	renObjectList[numberOfObjects].maxRadius = 5;
	renObjectList[numberOfObjects].numberOfTriangles = 1;
	
	numberOfObjects ++;
}

void RenEngine::LoadObjectFromFile(char* filename)
{
	//TODO
}

void GenerateRenderingList()
{
}

void PreRendering()
{
	
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
		if (renRenderingList[i].state == PRIMITIVE_STATE_ACTIVE)
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
		RenColor tmpColor(255, 255, 255, 1);
		DrawLine(p1.x, p1.y, p2.x, p2.y, tmpColor);
		DrawLine(p1.x, p1.y, p3.x, p3.y, tmpColor);
		DrawLine(p2.x, p2.y, p3.x, p3.y, tmpColor);
	}
	
	else 
	{
		if (p1.y > p2.y)
			SwapPoint4D(p1, p2);
		if (p1.y > p3.y)
			SwapPoint4D(p1, p3);
		if (p2.y > p3.y)
			SwapPoint4D(p2, p3);
		if (p1.x > p2.x)
			SwapPoint4D(p1, p2);
		if (p3.x > p2.x)
			SwapPoint4D(p2, p3);
		
		if (p1.y == p2.y) 
			DrawPrimitiveFlatTop(p1,p2,p3,pri.t[0],pri.t[1],pri.t[2]);
		else if (p2.y == p3.y)
			DrawPrimitiveFlatButton(p1, p2, p3, pri.t[0], pri.t[1], pri.t[2]);
		else
		{
			//draw general triangle
			float xMid = p1.x + (p3.x - p1.x)*(p2.y - p1.y)/(p3.y - p1.y);
			float uMid = pri.t[0].u + (pri.t[2].u - pri.t[0].u) * (xMid - p1.x) / (p3.x - p1.x);
			float vMid = pri.t[0].v + (pri.t[2].v - pri.t[0].v)*(p2.y - p1.y)/(p3.y - p1.y);
			//TODO: z buffer
			RenPoint4D pMid(xMid, p2.y, 1, 1);
			RenTexture tMid(uMid, vMid);
			DrawPrimitiveFlatButton(p1, p2, pMid, pri.t[0], pri.t[1], tMid);
			DrawPrimitiveFlatTop(pMid, p2, p3, tMid, pri.t[1], pri.t[2]);
		}
	}
		
}

void RenEngine::DrawPrimitiveFlatTop (RenPoint4D &p1, RenPoint4D &p2, RenPoint4D &p3, RenTexture &t1, RenTexture &t2, RenTexture &t3)
{
	//assume that p1.y = p2.y
	float dy = p3.y - p1.y;
	float xStart = p1.x;
	float xEnd = p2.x;
	float yStart = p1.y;
	float yEnd = p3.y;
	float dxdyl = (p3.x - p1.x)/dy;
	float dxdyr = (p3.x - p2.x)/dy;
	float uStart = t1.u;
	float uEnd = t2.u;
	float vStart = t1.v;
	float vEnd = t3.v;
	float dudx = (uStart - uEnd)/(xStart - xEnd);
	float dvdy = (vStart - vEnd)/(yStart - yEnd);
	float iu = uStart;
	float iv = vStart;

	RenColor tmpColor;
	for (float iy = yStart; iy < yEnd; iy++)
	{
		for (float ix = xStart; ix < xEnd; ix ++)
		{
			//tmpColor = map->data[iu][iv];
			//COLORREF color = BMPColorToCOLORREF(tmpColor);
			//SetPixel (hdc, ix, iy,color);
			iu += dudx;
		}
		xStart += dxdyl;
		xEnd += dxdyr;
		uStart += dxdyl*dudx;
		iv += dvdy;
	}
}

void RenEngine::DrawPrimitiveFlatButton (RenPoint4D &p1, RenPoint4D &p2, RenPoint4D &p3, RenTexture &t1, RenTexture &t2, RenTexture &t3)
{
	//assume that p3.y = p2.y
	float dy = p3.y - p1.y;
	float xStart = p1.x;
	float xEnd = p1.x;
	float yStart = p1.y;
	float yEnd = p3.y;
	float dxdyl = (p3.x - p1.x)/dy;
	float dxdyr = (p2.x - p1.x)/dy;
	float uStart = t1.u;
	float uEnd = t1.u;
	float vStart = t1.v;
	float vEnd = t3.v;
	float dudx = (t2.u - t1.u)/(p2.x - p1.x);
	float dvdy = (t3.v - t1.v)/(p3.y - p1.y);
	float iu = uStart;
	float iv = vStart;

	RenColor tmpColor;
	for (float iy = yStart; iy < yEnd; iy++)
	{
		for (float ix = xStart; ix < xEnd; ix ++)
		{
			//tmpColor = map->data[iu][iv];
			//COLORREF color = BMPColorToCOLORREF(tmpColor);
			//SetPixel (hdc, ix, iy,color);
			iu += dudx;
		}
		xStart += dxdyl;
		xEnd += dxdyr;
		uStart += dxdyl*dudx;
		iv += dvdy;
	}
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
//RenColor BitMapFindColor (BitMap *map, float x, float y)
//{
	
//}

void RenEngine::LocalToProjectionTransformation(RenVector4D &worldPos)
{
	for (int i = 0; i < numberOfObjects; i ++)
	{
		RenObject tmpObject = renObjectList[i];
		for (int j = 0; j < tmpObject.numberOfPoints; j ++)
		{
			// local to world
			tmpObject.transferredPointList[j].x = tmpObject.pointList[j].x + worldPos.x;
			tmpObject.transferredPointList[j].y = tmpObject.pointList[j].y + worldPos.y;
			tmpObject.transferredPointList[j].z = tmpObject.pointList[j].z + worldPos.z;
			//TODO: transform normal vector, from local to world space
			for (int k = 0; k < tmpObject.numberOfTriangles; k++)
			{
				CalculateTriangleNormal(&(tmpObject.triangleList[k]));
			}
			//TODO: lighting
			// world to camera
			tmpObject.transferredPointList[j] = tmpObject.transferredPointList[j] * renCamera.cameraTrans;
			// camera to projection
			//TODO: z--> 1/z
			tmpObject.transferredPointList[j].x = (tmpObject.transferredPointList[j].x * renCamera.zn) / tmpObject.transferredPointList[j].z;
			tmpObject.transferredPointList[j].y = (tmpObject.transferredPointList[j].y * renCamera.zn) / tmpObject.transferredPointList[j].z;
		}
	}
}

void RenEngine::IsObjectOutOfBoundary()
{
}

void RenEngine::ProjectionToViewPortTransformation()
{
	
}

void RenEngine::BackFaceDetection()
{
}

void RenEngine::Lighting()
{
	RenObject tmpObject;
	for (int i = 0; i < numberOfObjects; i++)
	{
		tmpObject = renObjectList[i];
		for (int j = 0; j < numberOfLights; j++)
		{
			if (renLight[j].att == RENLIGHT_ATT_AMBIENT)
			{
				for (int k = 0; k<tmpObject.numberOfTriangles; k ++)
				{
					//tmpObject.triangleList[k].color *= renLight[j].Ia;
				}
			}
			else if (renLight[j].att == RENLIGHT_ATT_DIRECTIONAL)
			{
				for (int k = 0; k < tmpObject.numberOfTriangles; k++)
				{
					//tmpObject.triangleList[k].color = tmpObject.triangleList[k].color * renLight[j].Id * DotProduct(&(-renLight[j].direction), &(tmpObject.triangleList[k].normal));
				}
			}
		}
	}
}

void RenEngine::Rendering()
{
	Rasterization();
}

void RenEngine::ReadKeyInput()
{
}

