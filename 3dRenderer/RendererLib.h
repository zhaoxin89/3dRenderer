#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#define MAX_OBJECT_POINTS 8192
#define MAX_NUM_TRIANGLES 8192
#define MAX_NUM_TEXTURES 8
#define MAX_NUM_TEXTURE_COOR 8192
#define MAX_NUM_MATIRIALS 8

#define PRIMITIVE_STATE_ACTIVE 0
#define PRIMITIVE_STATE_NON_ACTIVE 1

#define BACKFACE_DETECTION_VALIDATED 0
#define BACKFACE_DETECTION_REMOVED 1

typedef struct RenColor
{
	unsigned char alpha;
	unsigned char blue;
	unsigned char green;
	unsigned char red;

	RenColor(unsigned char r = 255, unsigned char g = 0, unsigned char b = 0, unsigned char a = 1)
	{
		red = r; green = g; blue = b; alpha = a;
	}
	void setValue(unsigned char a, unsigned char b, unsigned g, unsigned r)
	{
		alpha = a; blue = b; green = g; red = r;
	}
}RenColor, * RenColorPtr;

typedef struct RenPoint2D
{
	float x, y;
	RenPoint2D(float x1, float y1)
	{
		x = x1; y = y1;
	}
	RenPoint2D(float f[2])
	{
		x = f[0]; y = f[1]; 
	}
}RenPoint2D, RenVector2D, * RenVector2DPtr, * RenPoint2DPtr;

typedef struct RenPoint3D
{
	union
	{
		struct
		{
			float x, y, z;
		};
		struct
		{
			float p1, p2, p3;
		};
	};
	RenPoint3D(float x1 = 0, float y1 = 0, float z1 = 0)
	{
		x = x1; y = y1; z = z1; 
	}
	RenPoint3D(float *fp)
	{
		x = *fp; y = *(fp + 1); z = *(fp + 2);
	}
	void SetValue(float x1, float y1, float z1)
	{
		x = x1; y = y1; z = z1;
	}
	void SetValue(const RenPoint3D &p)
	{
		x = p.x; y = p.y; z = p.z;
	}
}RenPoint3D, RenVector3D, *RenVector3DPtr, * RenPoint3DPtr;

typedef struct RenMatrix4D
{
	float M[4][4];
}RenMatrix4D, * RenMatrix4DPtr;

typedef struct RenPoint4D
{
	float x, y, z, w;
	RenColor color;
	RenVector3D normal;
	RenPoint4D(float x1 = 1, float y1 = 0, float z1 = 0, float w1 = 1)
	{ 
		x = x1; y = y1; z = z1; w = w1;
	}
	RenPoint4D(float f[4])
	{
		x = f[0]; y = f[1]; z = f[2]; w = f[3];
	}

	RenPoint4D(RenPoint4D& p1, RenPoint4D& p2)
	{
		x = p2.x - p1.x;
		y = p2.y - p1.y;
		z = p2.z - p1.z;
		w = 1;
	}
	
	void SetValue(float x1, float y1, float z1, float w1)
	{
		x = x1; y = y1; z = z1; w = w1;
	}
	void SetColor(RenColor c)
	{
		color = c;
	}
	RenPoint4D operator * (const RenMatrix4D m)
	{
		RenPoint4D p;
		p.x = this->x * m.M[0][0] + this->y * m.M[1][0] + this->z * m.M[2][0] + this->w * m.M[3][0];
		p.y = this->x * m.M[0][1] + this->y * m.M[1][1] + this->z * m.M[2][1] + this->w * m.M[3][1];
		p.z = this->x * m.M[0][2] + this->y * m.M[1][2] + this->z * m.M[2][2] + this->w * m.M[3][2];
		p.w = this->x * m.M[0][3] + this->y * m.M[1][3] + this->z * m.M[2][3] + this->w * m.M[3][3];
		return p;
	}
}RenPoint4D, * RenPoint4DPtr, RenVector4D, * RenVector4DPtr;


typedef struct RenLine2D
{
	RenPoint2D p1, p2;
}RenLine2D, * RenLine2DPtr;

typedef struct RenLine3D
{
	RenPoint3D p1, p2;
}RenLine3D, * RenLine3DPtr;

typedef struct RenLine4D
{
	RenPoint4D p1, p2;
}RenLine4D, * RenLine4DPtr;

typedef struct RenTextureCoor
{
	float u, v;
	RenTextureCoor(float u1 = 0, float v1 = 0)
	{
		u = u1; v = v1;
	}
}RenTextureCoor, * RenTextureCoorPtr;

typedef struct RenVertex4D
{

	RenPoint4D p;
	RenTextureCoor t;
	RenVector3D normal;
	RenColor color;
}RenVertex4D, * RenVertex4DPtr;

typedef struct RenBMP
{
	unsigned char *buffer;
	int width;
	int height;
	RGBQUAD* pColorTable;
	int biBitCount;
	int lineByte;
	bool readBMP(const char* name)
	{
		FILE* fp = fopen(name, "rb");
		if (fp == 0) return 0;
		fseek(fp, sizeof(BITMAPFILEHEADER),0);
		BITMAPINFOHEADER head;
		fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
		this->width = head.biWidth;
		height = head.biHeight;
		biBitCount = head.biBitCount;
		lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
		if (biBitCount == 8)
		{
			pColorTable = new RGBQUAD[256];
			fread(pColorTable, sizeof(RGBQUAD), 256, fp);
		}
		buffer = new unsigned char[lineByte * height];
		fread(buffer, 1, lineByte * height, fp);
		fclose(fp);
		return 1;
	}
	RenColor getPixelColor(int i, int j)
	{
		RenColor ret;
		if (i < 0 || i > width || j < 0 || j > height)
		{
			ret.setValue(1, 255, 255, 255);
			return ret;
		}
		unsigned char* base = buffer + i * lineByte + j * 3;
		unsigned char b = *(base);
		unsigned char g = *(base + 1);
		unsigned char r = *(base + 2);
		ret.setValue(1, b, g, r);
		return ret;
	}
}RenBMP, *RenBMPPtr;

typedef struct RenTexture
{
	RenBMP bmp;
}RenTexture, * RenTexturePtr;

typedef struct RenMaterial
{
	RenVector4D ambient;
	RenVector4D diffuse;
	RenVector4D specular;
	float shininess;
} RenMaterial, *RenMaterialPtr;

typedef struct RenTriangle
{
	RenVector3D pointIndex;
	RenVector3D texIndex;
	RenVector3D normal;
	RenVector3D originalNormal;
	RenVector3D pointNormal[3];//for Goulaud shading
	RenPoint4DPtr pointListPtr;
	RenTextureCoorPtr textureCoorListPtr;
	int state;
	float angNormalDiffuseLight;

	RenTriangle (int pi1 = 0, int pi2 = 1, int pi3 = 2, int ti1 = 0, int ti2 = 1, int ti3 = 2)
	{
		pointIndex.SetValue(pi1, pi2, pi3); 
		texIndex.SetValue(ti1, ti2, ti3); 
		normal.SetValue(1, 0, 0);
		originalNormal.SetValue(1, 0, 0);
		pointListPtr = 0;
		textureCoorListPtr = 0;
		angNormalDiffuseLight = 0;
		state = BACKFACE_DETECTION_VALIDATED;
	}
	void resetNormal()
	{
		normal = originalNormal;
	}
}RenTriangle, * RenTrianglePtr;

typedef struct RenPrimitive
{
	RenVertex4D v[3];
	RenColor c; // used for wireframe mode
	//RenVector3D normal;
	//TODO: 
	//BitMapPtr *map;
	//int renderMode;
	float angNormalDiffuseLight;
	int state;
	int objetcIndex; // which object does this primitive belong to?
	//float angleWithSunLight;
}RenPrimitive, * RenPrimitivePtr;

typedef struct RenObject
{
	char name[64];
	RenPoint4D pointList[MAX_OBJECT_POINTS];
	RenPoint4D transferredPointList[MAX_OBJECT_POINTS];
	RenTriangle triangleList[MAX_NUM_TRIANGLES];
	RenTextureCoor textureCoorList[MAX_NUM_TEXTURE_COOR];
	RenTexture textureList[MAX_NUM_TEXTURES];
	RenMaterial materialList[MAX_NUM_MATIRIALS];
	//RenTriangle transferredTriangleList[MAX_NUM_TRIANGLES];
	int numberOfPoints;
	int numberOfTriangles;
	int numberOfMaterials;
	float maxRadius;
	int renderingMode;
	void setRenderingMode(int rm)
	{
		renderingMode = rm;
	}
}RenObject, * RenObjectPtr;

float CalculateDotProduct3D(const RenVector3D& u, const RenVector3D& v);
RenVector3D CalculateCrossProduct3D(RenVector3D& u, RenVector3D& v);
RenVector4D CalculateCrossProduct4D(RenVector4D& u, RenVector4D& v);

void Vector3DNormalize(RenVector3D* v);
void Vector4DNormalize(RenVector4D* v);
void CalculateTriangleNormal(RenTriangle* tri);

//vector<string> divideStr(const string& str, const string& dividerChars);
RenVector4D fscanVector4(FILE* fp);
RenVector3D fscanVector3(FILE* fp);
RenVector2D fscanVector2(FILE* fp);


RenMatrix4D CreateIdentityMatrix();
void SwapVertex4D(RenVertex4DPtr v1, RenVertex4DPtr v2);
void SwapPoint4D(RenPoint4DPtr p1, RenPoint4DPtr p2);

COLORREF RenColorToCOLORREF(RenColor c);

RenVector4D vector3DTo4D(RenVector3D v3d);
RenVector3D vector4DTo3D(RenVector4D v3d);

void RotateAroundXAxis(RenVector4D& p, float rad);
void RotateAroundYAxis(RenVector4D& p, float rad);
void RotateAroundZAxis(RenVector4D& p, float rad);

void RotateAroundXAxis(RenVector3D& p, float rad);
void RotateAroundYAxis(RenVector3D& p, float rad);
void RotateAroundZAxis(RenVector3D& p, float rad);

void Interpolate(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y, float c[3]);