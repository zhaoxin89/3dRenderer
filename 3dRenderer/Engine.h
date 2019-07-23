#pragma once
#include "MathLib.h"

#define MAX_NUM_TEXTILES 2
#define MAX_NUM_LIGHTS 3
#define MAX_NUM_OBJECTS 4
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
	private:
		RenCamera renCamera;
		RenTextile renTextile [MAX_NUM_TEXTILES];
		RenLight renLight [MAX_NUM_LIGHTS];
		RenObject renObject [MAX_NUM_OBJECTS];
		static RenEngine* instance;
}

