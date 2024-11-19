#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>

#include <fstream>
#include "textureclass.h"

using namespace std;
using namespace DirectX;

class SkyDomeClass
{
private:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};


	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

public:
	SkyDomeClass();
	SkyDomeClass(const SkyDomeClass&);
	~SkyDomeClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
	bool LoadTexture(ID3D11Device*, const WCHAR*);
private:
	bool LoadSkyDomeModel(const WCHAR* filename);
	void ReleaseSkyDomeModel();

	bool InitializeBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadModel(const WCHAR* filename);
	bool ReadFileCounts(const WCHAR*);
	bool LoadDataStructures(const WCHAR*, int, int, int, int);
private:
	ModelType* m_model = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	int m_textureCount = 0;
	int m_normalCount = 0;
	int m_faceCount = 0;
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	TextureClass* m_Texture;
};