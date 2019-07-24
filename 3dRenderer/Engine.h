#pragma once
#include "MathLib.h"

#define MAX_NUM_TEXTILES 2
#define MAX_NUM_LIGHTS 3
#define MAX_NUM_OBJECTS 4
#define MAX_NUM_PRIMITAVE 1024
void RenderLoop();

class RenEngine
{
	public:
		void LoadObject();
		void GenerateRenderingList();
		void PreRendering();
		void Rendering();
		void ReadKeyInput();
		static RenEngine* GetInstance();
		
		void Rasteration();
		void DrawTriangleList();
		void DrawTriangleFlatButton();
		void DrawTriangleFlatTop();
	private:
		RenCamera renCamera;
		RenTextile renTextile [MAX_NUM_TEXTILES];
		RenLight renLight [MAX_NUM_LIGHTS];
		RenObject renObjectList [MAX_NUM_OBJECTS];
		RenPrimitave renRenderingList[MAX_NUM_PRIMITAVE];
		static RenEngine* instance;
		int numberOfObjects;
		int numberOfPrimitives;
		HDC hdc;
}

