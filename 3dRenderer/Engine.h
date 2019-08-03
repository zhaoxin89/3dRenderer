#pragma once
#include "RendererLib.h"
#include "Camera.h"
#include "Light.h"
#include <string>

#define MAX_NUM_TEXTURES 8
#define MAX_NUM_LIGHTS 3
#define MAX_NUM_OBJECTS 4
#define MAX_NUM_PRIMITAVE 8192
#define MAX_NUM_BMP 2

#define RENDERING_MODE_WIREFRAME 0
#define RENDERING_MODE_PURE_COLOR 1
#define RENDERING_MODE_TEXTURE 2

using namespace std;

class RenEngine
{
public:
	static RenEngine* GetInstance();

	void RenderInit(int x1, int y1, int w1, int h1);
	void RenderLoop();
	void RenderExit();

	inline void SetDC(HDC h)
	{
		hdc = h;
	}

private:

	// test function: load one triangle
	void LoadTriangleObject();
	void LoadObjectFormModelASE(const string& folderPath, const string& fileName);

	void AddLight(int att, RenColor c, RenVector4D dir, RenVector4D loc);
	
	void GenerateRenderingList();
	void PreRendering();

	void Lighting();
	void Rendering();
	void ReadKeyInput();

	void LocalToWorldTransformation(RenVector4D &worldPos, float radX, float radY, float radZ, float scale);
	void WorldToCameraTransformation();
	void CameraToProjectionTransformation();
	void IsObjectOutOfBoundary();
	void ProjectionToViewPortTransformation();

	void BackFaceDetection();

	void Rasterization();
	void DrawPrimitive(RenPrimitive& pri);
	void DrawPrimitiveFlatTop(RenPoint4D& p1, RenPoint4D& p2, RenPoint4D& p3, RenTextureCoor& t1, RenTextureCoor& t2, RenTextureCoor& t3);
	void DrawPrimitiveFlatButton(RenPoint4D& p1, RenPoint4D& p2, RenPoint4D& p3, RenTextureCoor& t1, RenTextureCoor& t2, RenTextureCoor& t3);
	void DrawLine(float x1, float y1, float x2, float y2, RenColor renColor);
private:
	RenCamera renCamera;
	
	RenLight renLight[MAX_NUM_LIGHTS];
	RenObject renObjectList[MAX_NUM_OBJECTS];
	RenPrimitive renRenderingList[MAX_NUM_PRIMITAVE]; //renderinglist contains all the primitives from all the objects.
	//BMP bitmap[MAX_NUM_BMP];
	static RenEngine* instance;
	int numberOfLights = 0;
	int numberOfObjects = 0;
	int numberOfPrimitives = 0;
	int numberOfMaterials = 0;
	int numberOfTextures = 0;
	int x, y, w, h;

	int renderingMode;
	HDC hdc;

	RenEngine();
};

