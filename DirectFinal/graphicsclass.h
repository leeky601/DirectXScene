////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"
#include "SkyDomeClass.h"
#include "SkyDomeShader.h"
#include "ParticleSystemClass.h"
#include "ParticleShaderClass.h"
#include <vector>

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool Frame(int, int, int, int, bool, float);

	CameraClass* Getcamera();

private:
	void CreateBoundingVolumes(std::vector<XMFLOAT3>& ,float& boundingSphere,	XMVECTOR& objectCenterOffset);
	bool Render(float,float,float,float,float);
	bool MainRender();

	bool RenderSkyDome();

	bool BoundingSphereCollision(float firstObjBoundingSphere,
		XMVECTOR firstObjCenterOffset,
		XMMATRIX& firstObjWorldSpace,
		float secondObjBoundingSphere,
		XMVECTOR secondObjCenterOffset,
		XMMATRIX& secondObjWorldSpace);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Tree, * m_Model_bilboard, * m_Model_plane, *m_Fox, *m_Wolf, *m_Bizon, *m_Giraf, *m_DeerA;
	
	
	LightShaderClass* m_LightShader;
	LightClass* m_Light, * m_Light1;

	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap, *m_Title;


	TextClass* m_Text;

	SkyDomeClass* m_SkyDome = nullptr;
	SkyDomeShaderClass* m_SkyDomeShader = nullptr;

	XMMATRIX m_baseViewMatrix;

	ParticleShaderClass* m_ParticleShader = nullptr;
	ParticleSystemClass* m_ParticleSystem = nullptr;
	std::vector<XMFLOAT3> wolfVertPosArray, foxVertPosArray, deerVertPosArray;
	float foxBoundingSphere = 0.0f;
	float deerBoundingSphere = 0.0f;
	float wolfBoundingSphere = 0.0f;
	XMVECTOR foxCenterOffset;
	XMVECTOR wolfCenterOffset;
	XMVECTOR deerCenterOffset;

	XMMATRIX foxWorld;
	XMMATRIX wolfWorld;
	XMMATRIX deerWorld;

	bool foxeaten = 0;
	bool deereaten = 0;
};

#endif