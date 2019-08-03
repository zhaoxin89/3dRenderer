#include "Engine.h"

#include "RendererLib.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <wingdi.h>
#include <cassert>
#include <vector>

#include "Utils.h"

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
	x = y = w = h = 0;
	hdc = 0;
	renderingMode = RENDERING_MODE_TEXTURE;
}

void RenEngine::RenderInit(int x1, int y1, int w1, int h1)
{
	x = x1;
	y = y1;
	w = w1;
	h = h1;
	//LoadTriangleObject();
	LoadObjectFormModelASE("res/ASEModels","box.ASE");
	LocalTransformation(0, 0, 0, 1);
	RenVector3D cameraLoc(0, 0, 0);
	renCamera.InitCamera(1,10000, cameraLoc);
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

	RenVector4D worldPos(0,0,20,1);
	LocalToWorldTransformation(worldPos);
	WorldToCameraTransformation();
	CameraToProjectionTransformation();
	//TODO: clipping
	GenerateRenderingList();
	ProjectionToViewPortTransformation();
	PreRendering();
	Rendering();
	ReadKeyInput();
}

void RenEngine::GenerateRenderingList()
{
	RenObjectPtr tmpObjPtr;
	RenTriangle tri;
	RenPrimitive p;
	for (int i = 0; i < numberOfObjects; i++)
	{
		tmpObjPtr = &(renObjectList[i]);
		for (int j = 0; j < tmpObjPtr->numberOfTriangles; j++)
		{
			tri = tmpObjPtr->triangleList[j];

			p.v[0].p = tmpObjPtr->transferredPointList[(int)tri.pointIndex.p1];
			p.v[1].p = tmpObjPtr->transferredPointList[(int)tri.pointIndex.p2];
			p.v[2].p = tmpObjPtr->transferredPointList[(int)tri.pointIndex.p3];
			p.v[0].t = tmpObjPtr->textureCoorList[(int)tri.texIndex.x];
			p.v[1].t = tmpObjPtr->textureCoorList[(int)tri.texIndex.y];
			p.v[2].t = tmpObjPtr->textureCoorList[(int)tri.texIndex.z];
			p.state = PRIMITIVE_STATE_ACTIVE;
			p.objetcIndex = i;
			renRenderingList[j] = p;
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
	RenPoint4D p1(0, 10, 50, 1);
	RenPoint4D p2(10, 0, 50, 1);
	RenPoint4D p3(0, 0, 50, 1);
	p1.SetColor(c);
	p2.SetColor(c);
	p3.SetColor(c);
	renObjectList[numberOfObjects].pointList[0] = p1;
	renObjectList[numberOfObjects].pointList[1] = p2;
	renObjectList[numberOfObjects].pointList[2] = p3;
	RenTextureCoor t1(0, 0);
	RenTextureCoor t2(1, 1);
	RenTextureCoor t3(0, 1);
	renObjectList[numberOfObjects].textureCoorList[0] = t1;
	renObjectList[numberOfObjects].textureCoorList[1] = t2;
	renObjectList[numberOfObjects].textureCoorList[2] = t3;
	RenTriangle tmpTriangle(0,1,2,0,1,2);
	
	tmpTriangle.pointListPtr = (renObjectList[numberOfObjects].transferredPointList);
	CalculateTriangleNormal(&tmpTriangle);

	renObjectList[numberOfObjects].triangleList[0] = tmpTriangle;
	renObjectList[numberOfObjects].numberOfPoints = 3;
	renObjectList[numberOfObjects].maxRadius = 5;
	renObjectList[numberOfObjects].numberOfTriangles = 1;
	
	numberOfObjects ++;
}

void RenEngine::LoadObjectFormModelASE(const string& folderPath, const string& fileName)
{
		//------------------full path of file
		string fileFullPath = folderPath + "/" + fileName;
		//open file
		FILE* fp = NULL;
		fp = fopen(fileFullPath.c_str(), "r");// write
		if (fp == NULL) {
			cout << "Open " << fileFullPath << " failed!" << endl;
			assert(false);
		}
		//------------------read file
		vector<RenVector3D> posList;
		vector<RenVector3D> faceList;
		int materialRef = 0;
		vector<RenTextureCoor> tvertexList;
		vector<RenVector3D> tfaceList;

		vector<RenVector3D> faceNormList;
		vector<RenVector3D> faceV0NormList;
		vector<RenVector3D> faceV1NormList;
		vector<RenVector3D> faceV2NormList;

		vector<RenVector3D> ambientList;
		vector<RenVector3D> diffuseList;
		vector<float> shineStrengthList;
		vector<float> shineList;
		vector<RenVector3D> specularList;

		vector<string> texFileNameList;

		{
			fscanStr(fp, "*3DSMAX_ASCIIEXPORT");
			fscanInt(fp);
			fscanStr(fp, "*COMMENT");
			fscanOneQuotation(fp);
			fscanStr(fp, "*SCENE");
			fscanStr(fp, "{");
			fscanStr(fp, "*SCENE_FILENAME");
			fscanOneQuotation(fp);
			fscanStr(fp, "*SCENE_FIRSTFRAME");
			fscanInt(fp);
			fscanStr(fp, "*SCENE_LASTFRAME");
			fscanInt(fp);
			fscanStr(fp, "*SCENE_FRAMESPEED");
			fscanInt(fp);
			fscanStr(fp, "*SCENE_TICKSPERFRAME");
			fscanInt(fp);
			fscanStr(fp, "*SCENE_BACKGROUND_STATIC");
			fscanVector3(fp);
			fscanStr(fp, "*SCENE_AMBIENT_STATIC");
			fscanVector3(fp);
			fscanStr(fp, "}");
			fscanStr(fp, "*MATERIAL_LIST");
			fscanStr(fp, "{");
			fscanStr(fp, "*MATERIAL_COUNT");
			int materialCount = fscanInt(fp);
			for (int materialIndex = 0; materialIndex < materialCount; materialIndex++) {
				fscanStr(fp, "*MATERIAL");
				int materialID = fscanInt(fp);
				assert(materialID == materialIndex);
				fscanStr(fp, "{");
				fscanStr(fp, "*MATERIAL_NAME"); fscanOneQuotation(fp);
				fscanStr(fp, "*MATERIAL_CLASS"); fscanOneQuotation(fp);
				
				fscanStr(fp, "*MATERIAL_AMBIENT"); RenVector3D ambient = fscanVector3(fp);
				fscanStr(fp, "*MATERIAL_DIFFUSE"); RenVector3D diffuse = fscanVector3(fp);
				fscanStr(fp, "*MATERIAL_SPECULAR"); RenVector3D specular = fscanVector3(fp);
				fscanStr(fp, "*MATERIAL_SHINE"); float shine = fscanFloat(fp);
				fscanStr(fp, "*MATERIAL_SHINESTRENGTH"); float shineStrength = fscanFloat(fp);
				fscanStr(fp, "*MATERIAL_TRANSPARENCY"); fscanFloat(fp);
				fscanStr(fp, "*MATERIAL_WIRESIZE"); fscanFloat(fp);
				fscanStr(fp, "*MATERIAL_SHADING"); fscanStr(fp);
				fscanStr(fp, "*MATERIAL_XP_FALLOFF"); fscanFloat(fp);
				fscanStr(fp, "*MATERIAL_SELFILLUM"); fscanFloat(fp);
				fscanStr(fp, "*MATERIAL_FALLOFF"); fscanStr(fp);
				fscanStr(fp, "*MATERIAL_XP_TYPE"); fscanStr(fp);
				fscanStr(fp, "*MAP_DIFFUSE"); fscanStr(fp, "{");
				fscanStr(fp, "*MAP_NAME"); fscanOneQuotation(fp);
				fscanStr(fp, "*MAP_CLASS"); fscanOneQuotation(fp);
				fscanStr(fp, "*MAP_SUBNO"); fscanInt(fp);
				fscanStr(fp, "*MAP_AMOUNT"); fscanFloat(fp);
				fscanStr(fp, "*BITMAP"); string texFilePathWithQuotation = fscanOneQuotation(fp);
				string texFilePath = texFilePathWithQuotation.substr(1, (int)texFilePathWithQuotation.size() - 2);
				string dividerChars;
				dividerChars.push_back('/');
				dividerChars.push_back('\\');
				assert((int)dividerChars.size() == 2);
				string texFileName = divideStr(texFilePath, dividerChars).back();
				fscanStr(fp, "*MAP_TYPE"); fscanStr(fp);
				fscanStr(fp, "*UVW_U_OFFSET"); fscanFloat(fp);
				fscanStr(fp, "*UVW_V_OFFSET"); fscanFloat(fp);
				fscanStr(fp, "*UVW_U_TILING"); fscanFloat(fp);
				fscanStr(fp, "*UVW_V_TILING"); fscanFloat(fp);
				fscanStr(fp, "*UVW_ANGLE"); fscanFloat(fp);
				fscanStr(fp, "*UVW_BLUR"); fscanFloat(fp);
				fscanStr(fp, "*UVW_BLUR_OFFSET"); fscanFloat(fp);
				fscanStr(fp, "*UVW_NOUSE_AMT"); fscanFloat(fp);
				fscanStr(fp, "*UVW_NOISE_SIZE"); fscanFloat(fp);
				fscanStr(fp, "*UVW_NOISE_LEVEL"); fscanInt(fp);
				fscanStr(fp, "*UVW_NOISE_PHASE"); fscanFloat(fp);
				fscanStr(fp, "*BITMAP_FILTER"); fscanStr(fp);
				fscanStr(fp, "}");
				fscanStr(fp, "}");

				ambientList.push_back(ambient);
				diffuseList.push_back(diffuse);
				shineStrengthList.push_back(shineStrength);
				shineList.push_back(shine);
				specularList.push_back(specular);
				texFileNameList.push_back(texFileName);

			}
			fscanStr(fp, "}");

			fscanStr(fp, "*GEOMOBJECT");
			fscanStr(fp, "{");
			fscanStr(fp, "*NODE_NAME"); string nodeName = fscanOneQuotation(fp);
			fscanStr(fp, "*NODE_TM");
			fscanStr(fp, "{");
			fscanStr(fp, "*NODE_NAME"); fscanOneQuotation(fp);
			fscanStr(fp, "*INHERIT_POS"); fscanVector3(fp);
			fscanStr(fp, "*INHERIT_ROT"); fscanVector3(fp);
			fscanStr(fp, "*INHERIT_SCL"); fscanVector3(fp);
			fscanStr(fp, "*TM_ROW0"); fscanVector3(fp);
			fscanStr(fp, "*TM_ROW1"); fscanVector3(fp);
			fscanStr(fp, "*TM_ROW2"); fscanVector3(fp);
			fscanStr(fp, "*TM_ROW3"); fscanVector3(fp);
			fscanStr(fp, "*TM_POS"); fscanVector3(fp);
			fscanStr(fp, "*TM_ROTAXIS"); fscanVector3(fp);
			fscanStr(fp, "*TM_ROTANGLE"); fscanFloat(fp);
			fscanStr(fp, "*TM_SCALE"); fscanVector3(fp);
			fscanStr(fp, "*TM_SCALEAXIS"); fscanVector3(fp);
			fscanStr(fp, "*TM_SCALEAXISANG"); fscanFloat(fp);


			fscanStr(fp, "}");
			fscanStr(fp, "*MESH");
			fscanStr(fp, "{");
			fscanStr(fp, "*TIMEVALUE"); fscanInt(fp);
			fscanStr(fp, "*MESH_NUMVERTEX"); int vertexCount = fscanInt(fp);
			fscanStr(fp, "*MESH_NUMFACES"); int faceCount = fscanInt(fp);
			fscanStr(fp, "*MESH_VERTEX_LIST");
			fscanStr(fp, "{");
			for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++) {
				fscanStr(fp, "*MESH_VERTEX");    int vID = fscanInt(fp);	RenVector3D pos = fscanVector3(fp); 
				assert(vertexIndex == vID);
				posList.push_back(pos);
			}
			fscanStr(fp, "}");

			fscanStr(fp, "*MESH_FACE_LIST");
			fscanStr(fp, "{");
			for (int faceIndex = 0; faceIndex < faceCount; faceIndex++) {

				fscanStr(fp, "*MESH_FACE");
				string faceIDStr = fscanStr(fp);
				int faceID = strToNumber(faceIDStr.substr(0, (int)faceIDStr.size() - 1));
				fscanStr(fp, "A:"); int vIDA = fscanInt(fp);
				fscanStr(fp, "B:"); int vIDB = fscanInt(fp);
				fscanStr(fp, "C:"); int vIDC = fscanInt(fp);
				fscanStr(fp, "AB:"); fscanInt(fp);
				fscanStr(fp, "BC:"); fscanInt(fp);
				fscanStr(fp, "CA:"); fscanInt(fp);
				fscanStr(fp, "*MESH_SMOOTHING"); fscanInt(fp);
				fscanStr(fp, "*MESH_MTLID"); fscanInt(fp);

				RenVector3D face(vIDA, vIDB, vIDC);
				faceList.push_back(face);

			}
			fscanStr(fp, "}");
			fscanStr(fp, "*MESH_NUMTVERTEX"); int tvertexCount = fscanInt(fp);
			fscanStr(fp, "*MESH_TVERTLIST");
			fscanStr(fp, "{");
			for (int tvertexIndex = 0; tvertexIndex < tvertexCount; tvertexIndex++) {
				fscanStr(fp, "*MESH_TVERT"); int tvertexID = fscanInt(fp);	RenVector3D texCoord = fscanVector3(fp);
				assert(tvertexIndex == tvertexID);
				tvertexList.push_back(RenTextureCoor(texCoord.x, texCoord.y));
			}
			fscanStr(fp, "}");
			fscanStr(fp, "*MESH_NUMTVFACES"); int tfaceCount = fscanInt(fp);
			fscanStr(fp, "*MESH_TFACELIST");
			fscanStr(fp, "{");
			for (int tfaceIndex = 0; tfaceIndex < tfaceCount; tfaceIndex++) {
				fscanStr(fp, "*MESH_TFACE");
				int tfaceID = fscanInt(fp);
				assert(tfaceIndex == tfaceID);
				int tvID0 = fscanInt(fp);
				int tvID1 = fscanInt(fp);
				int tvID2 = fscanInt(fp);
				RenVector3D tface(tvID0, tvID1, tvID2);
				tfaceList.push_back(tface);
			}
			fscanStr(fp, "}");
			fscanStr(fp, "*MESH_NUMCVERTEX"); int cvertexCount = fscanInt(fp);
			if (cvertexCount != 0) {
				//currently we only support cvertexCount==0 !
				assert(false);
			}
			fscanStr(fp, "*MESH_NORMALS");
			fscanStr(fp, "{");
			for (int faceIndex = 0; faceIndex < faceCount; faceIndex++) {
				fscanStr(fp, "*MESH_FACENORMAL"); int faceID = fscanInt(fp);	RenVector3D faceNorm = fscanVector3(fp);	
				assert(faceIndex == faceID);
				fscanStr(fp, "*MESH_VERTEXNORMAL"); int vID0 = fscanInt(fp);	RenVector3D norm0 = fscanVector3(fp);  
				fscanStr(fp, "*MESH_VERTEXNORMAL"); int vID1 = fscanInt(fp);	RenVector3D norm1 = fscanVector3(fp);  
				fscanStr(fp, "*MESH_VERTEXNORMAL"); int vID2 = fscanInt(fp);	RenVector3D norm2 = fscanVector3(fp);  
				faceNormList.push_back(faceNorm);
				faceV0NormList.push_back(norm0);
				faceV1NormList.push_back(norm1);
				faceV2NormList.push_back(norm2);
			}
			fscanStr(fp, "}");
			fscanStr(fp, "}");
			fscanStr(fp, "*PROP_MOTIONBLUR"); fscanInt(fp);
			fscanStr(fp, "*PROP_CASTSHADOW"); fscanInt(fp);
			fscanStr(fp, "*PROP_RECVSHADOW"); fscanInt(fp);
			fscanStr(fp, "*MATERIAL_REF"); materialRef = fscanInt(fp);
			fscanStr(fp, "}");

		}
		//convert data format and fill data to mesh
		{
			RenObjectPtr tmpObj = &(renObjectList[numberOfObjects]);
			//----material and texture
			const int materialCount = (int)ambientList.size();
			
			for (int materialIndex = 0; materialIndex < materialCount; materialIndex++) {
				tmpObj->materialList[materialIndex].ambient = vector3DTo4D(ambientList[materialIndex]);
				tmpObj->materialList[materialIndex].diffuse = vector3DTo4D(diffuseList[materialIndex]);
				tmpObj->materialList[materialIndex].specular = vector3DTo4D(specularList[materialIndex]);
				tmpObj->materialList[materialIndex].shininess = shineStrengthList[materialIndex] / shineList[materialIndex];

				string texFilePath = folderPath + "/" + texFileNameList[materialIndex];
				bool initTexSucc = tmpObj->textureList[materialIndex].bmp.readBMP(texFilePath.c_str());
				assert(initTexSucc);
				numberOfMaterials++;
				numberOfTextures++;

			}

			//----convert mesh
			
			int pointCnt = (int)posList.size();
			renObjectList[numberOfObjects].numberOfPoints = pointCnt;
			for (int posIndex = 0; posIndex < pointCnt; posIndex++)
			{
				tmpObj->pointList[posIndex].x = posList[posIndex].x;
				tmpObj->pointList[posIndex].y = posList[posIndex].y;
				tmpObj->pointList[posIndex].z = posList[posIndex].z;
				tmpObj->pointList[posIndex].w = 1;

				tmpObj->transferredPointList[posIndex].x = posList[posIndex].x;
				tmpObj->transferredPointList[posIndex].y = posList[posIndex].y;
				tmpObj->transferredPointList[posIndex].z = posList[posIndex].z;
				tmpObj->transferredPointList[posIndex].w = 1;
			}
			int faceCnt = (int)faceList.size();
			renObjectList[numberOfObjects].numberOfTriangles = faceCnt;
			
			RenVector3D tmpV;
			RenVector3D v0N, v1N, v2N;
			for (int faceIndex = 0; faceIndex < faceCnt; faceIndex++)
			{
				tmpV = faceList[faceIndex];
				tmpObj->triangleList[faceIndex].pointIndex.SetValue(tmpV);
				
				tmpV = tfaceList[faceIndex];
				tmpObj->triangleList[faceIndex].texIndex.SetValue(tmpV);
				
				tmpV = faceNormList[faceIndex];
				tmpObj->triangleList[faceIndex].normal.SetValue(tmpV);

				v0N = faceV0NormList[faceIndex];
				v1N = faceV1NormList[faceIndex];
				v2N = faceV2NormList[faceIndex];
				tmpObj->triangleList[faceIndex].pointNormal[0].SetValue(v0N);
				tmpObj->triangleList[faceIndex].pointNormal[1].SetValue(v1N);
				tmpObj->triangleList[faceIndex].pointNormal[2].SetValue(v2N);
			}
			int textureCoorCnt = (int)tvertexList.size();
			for (int tIndex = 0; tIndex < textureCoorCnt; tIndex++)
			{
				renObjectList[numberOfObjects].textureCoorList[tIndex].u = tvertexList[tIndex].u;
				renObjectList[numberOfObjects].textureCoorList[tIndex].v = tvertexList[tIndex].v;
			}
			numberOfObjects++;
			numberOfPrimitives += tmpObj->numberOfTriangles;
		}
		
		//--------------------------close file
		fclose(fp);
	
}

void RenEngine::ReadKeyInput()
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
	RenVertex4D v1 = pri.v[0];
	RenVertex4D v2 = pri.v[1];
	RenVertex4D v3 = pri.v[2];
	
	if (renderingMode == RENDERING_MODE_WIREFRAME)
	{
		RenColor tmpColor(255, 255, 255, 1);
		DrawLine(v1.p.x, v1.p.y, v2.p.x, v2.p.y, tmpColor);
		DrawLine(v1.p.x, v1.p.y, v3.p.x, v3.p.y, tmpColor);
		DrawLine(v2.p.x, v2.p.y, v3.p.x, v3.p.y, tmpColor);
	}
	
	else 
	{
		if (v1.p.y > v2.p.y)
			SwapVertex4D(&v1, &v2);
		if (v1.p.y > v3.p.y)
			SwapVertex4D(&v1, &v3);
		if (v2.p.y > v3.p.y)
			SwapVertex4D(&v2, &v3);
		
		if (v1.p.y == v2.p.y) 
			DrawPrimitiveFlatTop(v1.p,v2.p,v3.p,v1.t,v2.t,v3.t, pri.objetcIndex);
		else if (v2.p.y == v3.p.y)
			DrawPrimitiveFlatButton(v1.p, v2.p, v3.p, v1.t, v2.t, v3.t, pri.objetcIndex);
		else
		{
			//draw general triangle
			float xMid = v1.p.x + (v3.p.x - v1.p.x)*(v2.p.y - v1.p.y)/(v3.p.y - v1.p.y);
			float uMid = v1.t.u + (v3.t.u - v1.t.u)*(xMid - v1.p.x) / (v3.p.x - v1.p.x);
			float vMid = v1.t.v + (v3.t.v - v1.t.v)*(v2.p.y - v1.p.y)/(v3.p.y - v1.p.y);
			//TODO: z buffer
			RenPoint4D pMid(xMid, v2.p.y, 1, 1); // TODO: z buffer
			RenTextureCoor tMid(uMid, vMid);
			DrawPrimitiveFlatButton(v1.p, v2.p, pMid, v1.t, v2.t, tMid, pri.objetcIndex);
			DrawPrimitiveFlatTop(pMid, v2.p, v3.p, tMid, v2.t, v3.t, pri.objetcIndex);
		}
	}
		
}

void RenEngine::DrawPrimitiveFlatTop (RenPoint4D &p1, RenPoint4D &p2, RenPoint4D &p3, RenTextureCoor &t1, RenTextureCoor &t2, RenTextureCoor &t3, int objIndex)
{
	
	//assume that p1.y = p2.y
	if (p1.x > p2.x)
	{
		RenPoint4D tmpP;
		SWAP(p1, p2, tmpP);
		RenTextureCoor tmpT;
		SWAP(t1, t2, tmpT);
	}
	t1.u = 0; t1.v = 0;
	t2.u = 1; t2.v = 0;
	t3.u = 1; t3.v = 1;
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
	int bmpW = renObjectList[objIndex].textureList[0].bmp.width;
	int bmpH = renObjectList[objIndex].textureList[0].bmp.height;
	for (float iy = yStart; iy < yEnd; iy++)
	{
		iu = uStart;
		for (float ix = xStart; ix < xEnd; ix ++)
		{
			tmpColor = renObjectList[objIndex].textureList[0].bmp.getPixelColor(iu* bmpW,iv* bmpH);
			COLORREF color = RenColorToCOLORREF(tmpColor);
			SetPixel (hdc, ix, iy, color);
			iu += dudx;
		}
		xStart += dxdyl;
		xEnd += dxdyr;
		uStart += dxdyl*dudx;
		iv += dvdy;
	}
	
}

void RenEngine::DrawPrimitiveFlatButton (RenPoint4D &p1, RenPoint4D &p2, RenPoint4D &p3, RenTextureCoor &t1, RenTextureCoor &t2, RenTextureCoor &t3, int objIndex)
{
	//assume that p3.y = p2.y
	t1.u = 0; t1.v = 0;
	t2.u = 0; t2.v = 1;
	t3.u = 1; t3.v = 1;
	if (p2.x > p3.x)
	{
		RenPoint4D tmpP;
		SWAP(p2, p3, tmpP);
		RenTextureCoor tmpT;
		SWAP(t2, t3, tmpT);
	}
	float dy = p3.y - p1.y;
	float xStart = p1.x;
	float xEnd = p1.x;
	float yStart = p1.y;
	float yEnd = p3.y;
	float dxdyl = (p2.x - p1.x)/dy;
	float dxdyr = (p3.x - p1.x)/dy;
	float uStart = t1.u;
	float uEnd = t1.u;
	float vStart = t1.v;
	float vEnd = t3.v;
	float dudx = (t3.u - t2.u)/(p3.x - p2.x);
	float dvdy = (t2.v - t1.v)/(p2.y - p1.y);
	float iu = uStart;
	float iv = vStart;

	RenColor tmpColor;
	int bmpW = renObjectList[objIndex].textureList[0].bmp.width;
	int bmpH = renObjectList[objIndex].textureList[0].bmp.height;
	for (float iy = yStart; iy < yEnd; iy++)
	{
		iu = uStart;
		for (float ix = xStart; ix < xEnd; ix ++)
		{
			
			tmpColor = renObjectList[objIndex].textureList[0].bmp.getPixelColor(iu* bmpW, iv* bmpH); // to be verified
			COLORREF color = RenColorToCOLORREF(tmpColor);
			SetPixel(hdc, ix, iy, color);
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
	//COLORREF color = RGB(0, 255, 0);
	if ((abs(x2-x1))<(abs(y2-y1)))
	{
		if (y2 < y1)
		{
			float tmp;
			SWAP(x1, x2, tmp);
			SWAP(y1, y2, tmp);
		}
		float dx = (x2 - x1) / (y2 - y1);
		float ix = ceil(x1) - dx*(y1 - ceil(y1));
		
		for (int iy = ceil(y1); iy <= (ceil(y2)-1); iy++)
		{
			SetPixel(hdc, ceil(ix), iy, color);
			ix += dx;
		}
	}
	else
	{
		if (x2 < x1)
		{
			float tmp;
			SWAP(x1, x2, tmp);
			SWAP(y1, y2, tmp);
		}
		float dy = (y2 - y1) / (x2 - x1);
		float iy = ceil(y1) - dy*(y1 - ceil(y1));

		for (int ix = ceil(x1); ix <= ceil(x2)-1; ix ++)
		{
			SetPixel(hdc, ix, ceil(iy), color);
			iy += dy;
		}
	}
}
//RenColor BitMapFindColor (BitMap *map, float x, float y)
//{
	
//}
void RenEngine::LocalTransformation(float radX, float radY, float radZ, float scale)
{
	for (int i = 0; i < numberOfObjects; i++)
	{
		RenObjectPtr tmpObject = &(renObjectList[i]);
		for (int j = 0; j < tmpObject->numberOfPoints; j++)
		{
			RotateAroundXAxis(tmpObject->pointList[j], radX);
			RotateAroundYAxis(tmpObject->pointList[j], radY);
			RotateAroundZAxis(tmpObject->pointList[j], radZ);

			tmpObject->pointList[j].x /= scale;
			tmpObject->pointList[j].y /= scale;
			tmpObject->pointList[j].z /= scale;
		}
	}
}
void RenEngine::LocalToWorldTransformation(RenVector4D& worldPos)
{
	for (int i = 0; i < numberOfObjects; i ++)
	{
		RenObjectPtr tmpObject = &(renObjectList[i]);
		for (int j = 0; j < tmpObject->numberOfPoints; j ++)
		{
			// local to world
			tmpObject->transferredPointList[j].x = tmpObject->pointList[j].x + worldPos.x;
			tmpObject->transferredPointList[j].y = tmpObject->pointList[j].y + worldPos.y;
			tmpObject->transferredPointList[j].z = tmpObject->pointList[j].z + worldPos.z;
		}
	}
}

void RenEngine::WorldToCameraTransformation()
{
	for (int i = 0; i < numberOfObjects; i++)
	{
		RenObjectPtr tmpObj = &(renObjectList[i]);
		
		//TODO: lighting
		// world to camera
		for (int j = 0; j < tmpObj->numberOfPoints; j++)
		{
			tmpObj->transferredPointList[j] = tmpObj->transferredPointList[j] * renCamera.cameraTrans;
		}
	}
}
void RenEngine::CameraToProjectionTransformation()
{
	for (int i = 0; i < numberOfObjects; i++)
	{
		RenObjectPtr tmpObj = &(renObjectList[i]);
		// camera to projection
		//TODO: z--> 1/z
		for (int j = 0; j < tmpObj->numberOfPoints; j++)
		{
			tmpObj->transferredPointList[j].x = (tmpObj->transferredPointList[j].x * renCamera.zn) / tmpObj->transferredPointList[j].z;
			tmpObj->transferredPointList[j].y = (tmpObj->transferredPointList[j].y * renCamera.zn) / tmpObj->transferredPointList[j].z;
		}
	}
}

void RenEngine::IsObjectOutOfBoundary()
{

}

void RenEngine::ProjectionToViewPortTransformation()
{
	for (int i = 0; i < numberOfPrimitives; i++)
	{
		RenPrimitivePtr tmpPri = &(renRenderingList[i]);
		for (int j = 0; j < 3; j++)
		{
			tmpPri->v[j].p.x = (tmpPri->v[j].p.x) * (w - 1) / (2 * renCamera.zn) + (w - 1) / 2;
			tmpPri->v[j].p.y = (tmpPri->v[j].p.y * (w/h)) * (1 - h) / (2 * renCamera.zn) + (h - 1) / 2;
		}								
	}
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

void RenEngine::MoveForward(int speed)
{
	renCamera.location.z += speed;
	renCamera.CalculateCameraTrans();
}
void RenEngine::MoveBackward(int speed)
{
	renCamera.location.z -= speed;
	renCamera.CalculateCameraTrans();
}
void RenEngine::MoveLeft(int speed)
{
	renCamera.location.x -= speed;
	renCamera.CalculateCameraTrans();
}
void RenEngine::MoveRight(int speed)
{
	renCamera.location.x += speed;
	renCamera.CalculateCameraTrans();
}

void RenEngine::MoveUp(int speed)
{
	renCamera.location.y += speed;
	renCamera.CalculateCameraTrans();
}

void RenEngine::MoveDown(int speed)
{
	renCamera.location.y -= speed;
	renCamera.CalculateCameraTrans();
}
