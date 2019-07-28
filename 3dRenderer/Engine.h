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

	void RenderInit();
	void RenderLoop();
	void RenderExit();

private:
	void LoadObject(RenMatrix4D& worldPos);
	void GenerateRenderingList();
	void PreRendering();
	void Rendering();
	void ReadKeyInput();

	void LocalToProjectionTransformation();

	void IsObjectOutOfBoundary();
	void ProjectionToViewPortTransformation();

	void BackFaceDetection();

	void Rasteration();
	void DrawTriangleList();
	void DrawTriangleFlatButton();
	void DrawTriangleFlatTop();

private:
	RenCamera renCamera;
	RenTexture renTexture[MAX_NUM_TEXTURES];
	RenLight renLight[MAX_NUM_LIGHTS];
	RenObject renObjectList[MAX_NUM_OBJECTS];
	RenPrimitive renRenderingList[MAX_NUM_PRIMITAVE];
	BMP bitmap[MAX_NUM_BMP];
	static RenEngine* instance;
	int numberOfObjects;
	int numberOfPrimitives;
	HDC hdc;

	RenEngine();
};

