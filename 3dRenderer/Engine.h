#pragma once
#include "RendererLib.h"
#include "Camera.h"
#include "Light.h"

#define MAX_NUM_TEXTILES 2
#define MAX_NUM_LIGHTS 3
#define MAX_NUM_OBJECTS 4
#define MAX_NUM_PRIMITAVE 1024
#define MAX_NUM_BMP 2

class RenEngine
{
public:
	static RenEngine* GetInstance();

	void RenderInit(HDC h);
	void RenderLoop();
	void RenderExit();

private:

	// test function: load one triangle
	void LoadTriangleObject();

	void LoadObjectFromFile(char* filename);

	void AddLight(int att, RenColor c, RenVector4D dir, RenVector4D loc);
	
	void GenerateRenderingList();
	void PreRendering();

	void Lighting();
	void Rendering();
	void ReadKeyInput();

	void LocalToProjectionTransformation(RenVector4D &worldPos);

	void IsObjectOutOfBoundary();
	void ProjectionToViewPortTransformation();

	void BackFaceDetection();

	void Rasterization();
	void DrawPrimitive(RenPrimitive& pri);
	void DrawPrimitiveFlatTop(RenPoint4D& p1, RenPoint4D& p2, RenPoint4D& p3, RenTexture& t1, RenTexture& t2, RenTexture& t3);
	void DrawPrimitiveFlatButton(RenPoint4D& p1, RenPoint4D& p2, RenPoint4D& p3, RenTexture& t1, RenTexture& t2, RenTexture& t3);
	void DrawLine(float x1, float y1, float x2, float y2, RenColor renColor);
private:
	RenCamera renCamera;
	RenTexture renTexture[MAX_NUM_TEXTURES];
	RenLight renLight[MAX_NUM_LIGHTS];
	RenObject renObjectList[MAX_NUM_OBJECTS];
	RenPrimitive renRenderingList[MAX_NUM_PRIMITAVE];
	//BMP bitmap[MAX_NUM_BMP];
	static RenEngine* instance;
	int numberOfLights = 0;
	int numberOfObjects = 0;
	int numberOfPrimitives = 0;
	HDC hdc;

	RenEngine();
};

