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
}

void RenEngine::RenderInit(int x1, int y1, int w1, int h1)
{
	x = x1;
	y = y1;
	w = w1;
	h = h1;
	LoadTriangleObject();
	renCamera.InitCamera(1,10000);
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
	LocalToProjectionTransformation(worldPos);
	//TODO: clipping
	GenerateRenderingList();
	ProjectionToViewPortTransformation();
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
			p.p[0] = tmpObject.transferredPointList[tri.pointIndex[0]];
			p.p[1] = tmpObject.transferredPointList[tri.pointIndex[1]];
			p.p[2] = tmpObject.transferredPointList[tri.pointIndex[2]];
			p.t[0] = tmpObject.textureCoorList[tri.texIndex[0]];
			p.t[1] = tmpObject.textureCoorList[tri.texIndex[1]];
			p.t[2] = tmpObject.textureCoorList[tri.texIndex[2]];
			p.renderMode = PRIMITIVE_MODE_WIREFRAME;
			p.state = PRIMITIVE_STATE_ACTIVE;
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

void RenEngine::LoadModelASE(const string& folderPath, const string& fileName)
{
		RenTexturePtr textureList = renTextureList;
		RenMaterialPtr materialList = renMaterialList;

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
			//----material and texture
			const int materialIDBase = (int)m_materialList.size();
			const int textureIDBase = (int)m_textureCoorList.size();
			const int materialCount = (int)ambientList.size();
			for (int materialIndex = 0; materialIndex < materialCount; materialIndex++) {
				Cc3dMaterial* material = new Cc3dMaterial();
				material->m_ambient = ambientList[materialIndex];
				material->m_diffuse = diffuseList[materialIndex];
				material->m_shininess = shineStrengthList[materialIndex] / shineList[materialIndex];//i guess it like that, not sure!!!
				material->m_specular = specularList[materialIndex];
				m_materialList.push_back(material);
				Ctexture* texture = new Ctexture();
				string texFilePath = folderPath + "/" + texFileNameList[materialIndex];
				bool initTexSucc = texture->initWithFile(texFilePath);
				assert(initTexSucc);
				m_textureCoorList.push_back(texture);

			}



			//----convert mesh
			vector<Cvert> vList;
			vector<Cc3dIDTriangle> IDtriList;
			int faceCount = (int)faceList.size();
			for (int faceIndex = 0; faceIndex < faceCount; faceIndex++) {
				const Cc3dIDTriangle& face = faceList[faceIndex];
				const Cc3dIDTriangle& tface = tfaceList[faceIndex];
				for (int i = 0; i < 3; i++) {
					const int vID = face.vID(i);
					const int tvID = tface.vID(i);
					Cvert v;
					v.m_pos = posList[vID];
					v.m_norm;
					if (i == 0) {
						v.m_norm = faceV0NormList[faceIndex];
					}
					else if (i == 1) {
						v.m_norm = faceV1NormList[faceIndex];
					}
					else if (i == 2) {
						v.m_norm = faceV2NormList[faceIndex];
					}
					v.m_materialID = materialIDBase + materialRef;
					v.m_textureID = textureIDBase + materialRef;
					v.m_texCoord = tvertexList[tvID];
					vList.push_back(v);
				}
				int newVID0 = faceIndex * 3 + 0;
				int newVID1 = faceIndex * 3 + 1;
				int newVID2 = faceIndex * 3 + 2;
				IDtriList.push_back(Cc3dIDTriangle(newVID0, newVID1, newVID2));
			}//got vList and IDtriList
			//fill vList and IDtriList to mesh
			mesh->m_IDtriList = IDtriList;
			for (int i = 0; i < (int)vList.size(); i++) {
				mesh->pushBackVert(vList[i]);
			}//got mesh

		}





		//--------------------------close file
		fclose(fp);
	
}

void RenEngine::LoadObjectFromFile(char* filename)
{
	//TODO
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
			RenTextureCoor tMid(uMid, vMid);
			DrawPrimitiveFlatButton(p1, p2, pMid, pri.t[0], pri.t[1], tMid);
			DrawPrimitiveFlatTop(pMid, p2, p3, tMid, pri.t[1], pri.t[2]);
		}
	}
		
}

void RenEngine::DrawPrimitiveFlatTop (RenPoint4D &p1, RenPoint4D &p2, RenPoint4D &p3, RenTextureCoor &t1, RenTextureCoor &t2, RenTextureCoor &t3)
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

void RenEngine::DrawPrimitiveFlatButton (RenPoint4D &p1, RenPoint4D &p2, RenPoint4D &p3, RenTextureCoor &t1, RenTextureCoor &t2, RenTextureCoor &t3)
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
	//COLORREF color = RGB(0, 255, 0);
	if ((abs(x2-x1))<(abs(y2-y1)))
	{
		float ix = x1;
		float dx = (x2 - x1)/(y2 - y1);
		int sign = y1 < y2 ? 1: -1;
		for (int iy = y1; iy != (int)y2; iy+=sign)
		{
			SetPixel(hdc, ix, iy, color);
			ix += dx;
		}
	}
	else
	{
		float iy = y1;
		float dy = (y2-y1)/(x2-x1);
		int sign = x1 < x2 ? 1 : -1;
		for (int ix = x1; ix != (int)x2; ix += sign)
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
		RenObjectPtr tmpObject = &(renObjectList[i]);
		for (int j = 0; j < tmpObject->numberOfPoints; j ++)
		{
			// local to world
			tmpObject->transferredPointList[j].x = tmpObject->pointList[j].x + worldPos.x;
			tmpObject->transferredPointList[j].y = tmpObject->pointList[j].y + worldPos.y;
			tmpObject->transferredPointList[j].z = tmpObject->pointList[j].z + worldPos.z;
			//TODO: transform normal vector, from local to world space
			for (int k = 0; k < tmpObject->numberOfTriangles; k++)
			{
				CalculateTriangleNormal(&(tmpObject->triangleList[k]));
			}
			//TODO: lighting
			// world to camera
			tmpObject->transferredPointList[j] = tmpObject->transferredPointList[j] * renCamera.cameraTrans;
			// camera to projection
			//TODO: z--> 1/z
			tmpObject->transferredPointList[j].x = (tmpObject->transferredPointList[j].x * renCamera.zn) / tmpObject->transferredPointList[j].z;
			tmpObject->transferredPointList[j].y = (tmpObject->transferredPointList[j].y * renCamera.zn) / tmpObject->transferredPointList[j].z;
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
			tmpPri->p[j].x = (tmpPri->p[j].x) * (w - 1) / (2 * renCamera.zn) + (w - 1) / 2;
			tmpPri->p[j].y = (tmpPri->p[j].y * (w/h)) * (1 - h) / (2 * renCamera.zn) + (h - 1) / 2;
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

