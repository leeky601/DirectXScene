////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
using namespace DirectX;


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Tree = 0;
	m_Model_bilboard = 0;
	m_Model_plane = 0;
	m_Fox = 0;
	m_Bizon = 0;
	m_Wolf = 0;
	m_DeerA = 0;
	m_Giraf = 0;
	

	m_LightShader = 0;
	m_Light = 0;
	m_Light1 = 0;

	m_TextureShader = 0;
	m_Bitmap = 0;
	m_Title = 0;

	m_Text = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 3.0f, -30.0f);	// for cube
//	m_Camera->SetPosition(0.0f, 0.5f, -3.0f);	// for chair
		
	// Create the model object.
	m_Model_bilboard = new ModelClass;
	m_Model_plane = new ModelClass;
	m_Tree = new ModelClass;
	m_Fox = new ModelClass;
	m_Bizon = new ModelClass;
	m_Wolf = new ModelClass;
	m_DeerA = new ModelClass;
	m_Giraf = new ModelClass;

	if(!m_Model_bilboard)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model_bilboard->Initialize(m_D3D->GetDevice(), L"./data/bird2.obj", L"./data/BirdUV.dds");
	m_Model_plane->Initialize(m_D3D->GetDevice(), L"./data/plane.obj", L"./data/green-grass-textures.dds");
	m_Tree->Initialize(m_D3D->GetDevice(), L"./data/tree.obj", L"./data/Spooky1Twig_Diffuse4.dds");
	m_Fox->Initialize(m_D3D->GetDevice(), L"./data/fox.obj", L"./data/01foxtexture.dds");
	foxVertPosArray = m_Fox->GetVertPosArray();
	CreateBoundingVolumes(foxVertPosArray, foxBoundingSphere, foxCenterOffset);
	m_Wolf->Initialize(m_D3D->GetDevice(), L"./data/Wolf.obj", L"./data/25dogtexture.dds");
	wolfVertPosArray = m_Wolf->GetVertPosArray();
	CreateBoundingVolumes(wolfVertPosArray, wolfBoundingSphere, wolfCenterOffset);
	m_Bizon->Initialize(m_D3D->GetDevice(), L"./data/bizon.obj", L"./data/04bizontexture.dds");
	/*wolfCenterOffset = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	wolfBoundingSphere = 1.0f;*/
	m_DeerA->Initialize(m_D3D->GetDevice(), L"./data/deer09.obj", L"./data/09deertexture.dds");
	deerVertPosArray = m_DeerA->GetVertPosArray();
	CreateBoundingVolumes(deerVertPosArray, deerBoundingSphere, deerCenterOffset);
	m_Giraf->Initialize(m_D3D->GetDevice(), L"./data/giraf.obj", L"./data/18giraftexture.dds");

	
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
//	m_Light->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
//	m_Light->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
//	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
//	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light->SetDirection(1.0f, 0.0f, -1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	m_Light1 = new LightClass;
	if (!m_Light1)
	{
		return false;
	}
	// Initialize the first light object.
	/*m_Light1->SetDirection(0.0f, -1.0f, 0.0f);*/
	m_Light1->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	m_Title = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/sky.dds", screenWidth, screenHeight);
	m_Title->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title.dds", screenWidth, screenHeight);
	
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
//	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(m_baseViewMatrix);
	

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, m_baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// 스카이 돔 객체를 생성합니다.
	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
	{
		return false;
	}

	// 스카이 돔 객체를 초기화 합니다.
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// 스카이 돔 쉐이더 객체를 생성합니다.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
	{
		return false;
	}

	// 스카이 돔 쉐이더 객체를 초기화 합니다.
	result = m_SkyDomeShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	// 파티클 셰이더 개체를 만듭니다.
	m_ParticleShader = new ParticleShaderClass;
	if (!m_ParticleShader)
	{
		return false;
	}

	// 파티클 셰이더 개체를 초기화합니다.
	if (!m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	// 파티클 시스템 객체를 만듭니다.
	m_ParticleSystem = new ParticleSystemClass;
	if (!m_ParticleSystem)
	{
		return false;
	}

	// 파티클 시스템 객체를 초기화합니다.
	if (!m_ParticleSystem->Initialize(m_D3D->GetDevice(), L"../DirectFinal/data/star.dds"))
	{
		return false;
	}

	return true;
}

CameraClass* GraphicsClass::Getcamera()
{
	return m_Camera;
}


void GraphicsClass::Shutdown()
{
	// Release the model object.
	if(m_Model_bilboard)
	{
		m_Model_bilboard->Shutdown();
		delete m_Model_bilboard;
		m_Model_bilboard = 0;
	}

	if (m_Model_plane)
	{
		m_Model_plane->Shutdown();
		delete m_Model_plane;
		m_Model_plane = 0;
	}

	if (m_Tree)
	{
		m_Tree->Shutdown();
		delete m_Tree;
		m_Tree = 0;
	}

	if (m_Fox)
	{
		m_Fox->Shutdown();
		delete m_Fox;
		m_Fox = 0;
	}


	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	
	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	if (m_Title)
	{
		m_Title->Shutdown();
		delete m_Title;
		m_Title = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// 파티클 시스템 객체를 해제합니다.
	if (m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// particle shader 객체를 해제한다.
	if (m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}
	return;
}

//bool GraphicsClass::Frame()
//{
//	bool result;
//
//	static float rotation = 0.0f;
//
//
//	// Update the rotation variable each frame.
//	rotation += XM_PI * 0.005f;
//	if (rotation > 360.0f)
//	{
//		rotation -= 360.0f;
//	}
//
//	// Render the graphics scene.
//	result = Render(rotation);
//	if(!result)
//	{
//		return false;
//	}
//
//	return true;
//}

bool GraphicsClass::Frame(int fps, int cpu, int mouseX, int mouseY, bool Main, float frameTime)
{
	bool result;
	static float rotation = 0.0f;
	int faceCount;
	static float rotation_f = -90.0f;
	static float xpos_f = 0.0f;
	static float ypos_d = 1.5f;
	static int up = 1;
	static float rotation_g = 0.0f;
	static float increase = 0.0f;
	
	ypos_d += up * 0.1f;

	if (ypos_d < 1.5f) {
		up = 1;
	}

	if (ypos_d > 5.5f) {
		up = -1;
	}

	

	if (rotation_f < -80.0f) {
		xpos_f += 0.08f;
		if (xpos_f > 20.0f) {
			rotation_f = 90.0f;
		}
	}

	if (rotation_f > 80.0f) {
		xpos_f -= 0.08f;
		if (xpos_f < -20.0f) {
			rotation_f = -90.0f;
		}
	}

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	increase += (float)XM_PI * 0.1f;
	if (increase > 0.0f && increase < 90.0f) {
		rotation_g = 0.0f;
	}

	if (increase > 90.0f && increase < 180.0f) {
		rotation_g = 90.0f;
	}

	if (increase > 180.0f && increase < 270.0f) {
		rotation_g = 180.0f;
	}

	if (increase > 270.0f && increase < 360.0f) {
		rotation_g = 270.0f;
		increase -= 360.0f;
	}

	// Set the frames per second.
	result = m_Text->SetFPS(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCPU(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	faceCount = m_Model_bilboard->GetFaceCount() + m_Model_plane->GetFaceCount() + m_Bizon->GetFaceCount() + m_DeerA->GetFaceCount() + m_Fox->GetFaceCount() + m_Wolf->GetFaceCount() + m_Giraf->GetFaceCount() + m_Tree->GetFaceCount();
	
	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, faceCount, m_D3D->GetDeviceContext());

	if (!result)
	{
		return false;
	}

	// 파티클 시스템에 대한 프레임 처리를 실행합니다.
	m_ParticleSystem->Frame(frameTime, m_D3D->GetDeviceContext());
	

	if (Main == TRUE) {
		// Render the graphics scene.
		result = MainRender();
		if (!result)
		{
			return false;
		}
	}
	else {
		result = Render(rotation, rotation_f, rotation_g, xpos_f, ypos_d);
		if (!result)
		{
			return false;
		}
	}
	
	

	// Set the position of the camera.
//	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	
	

	return true;
}

void GraphicsClass::CreateBoundingVolumes(std::vector<XMFLOAT3>&vertPosArray, float& boundingSphere, XMVECTOR& objectCenterOffset)
{
	XMFLOAT3 minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (UINT i = 0; i < vertPosArray.size(); i++)
	{
		// The minVertex and maxVertex will most likely not be actual vertices in the model, but vertices
		// that use the smallest and largest x, y, and z values from the model to be sure ALL vertices are
		// covered by the bounding volume

		//Get the smallest vertex 
		minVertex.x = min(minVertex.x, vertPosArray[i].x);	// Find smallest x value in model
		minVertex.y = min(minVertex.y, vertPosArray[i].y);	// Find smallest y value in model
		minVertex.z = min(minVertex.z, vertPosArray[i].z);	// Find smallest z value in model

		//Get the largest vertex 
		maxVertex.x = max(maxVertex.x, vertPosArray[i].x);	// Find largest x value in model
		maxVertex.y = max(maxVertex.y, vertPosArray[i].y);	// Find largest y value in model
		maxVertex.z = max(maxVertex.z, vertPosArray[i].z);	// Find largest z value in model
	}

	// Compute distance between maxVertex and minVertex
	float distX = (maxVertex.x - minVertex.x) / 2.0f;
	float distY = (maxVertex.y - minVertex.y) / 2.0f;
	float distZ = (maxVertex.z - minVertex.z) / 2.0f;

	// Now store the distance between (0, 0, 0) in model space to the models real center
	objectCenterOffset = XMVectorSet(maxVertex.x - distX, maxVertex.y - distY, maxVertex.z - distZ, 0.0f);

	// Compute bounding sphere (distance between min and max bounding box vertices)
	// boundingSphere = sqrt(distX*distX + distY*distY + distZ*distZ) / 2.0f;
	boundingSphere = XMVectorGetX(XMVector3Length(XMVectorSet(distX, distY, distZ, 0.0f)));
}

bool GraphicsClass::Render(float rotation,float rotation_f,float rotation_g,float xpos_f,float ypos_d)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, rotationMatrix;
	bool result;
	XMFLOAT4 diffuseColor_G[2], diffuseColor_A[2];
	XMFLOAT4 lightPosition;
	XMFLOAT3 cameraPosition, modelPosition;
	double angle;
	float rotationX;

	diffuseColor_G[0] = m_Light->GetDiffuseColor();
	diffuseColor_G[1] = m_Light1->GetDiffuseColor();

	diffuseColor_A[0] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	diffuseColor_A[1] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	lightPosition = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0);

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	result = RenderSkyDome();
	if (!result)
	{
		assert("Render Sky Dome Error");
	}
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	//if (!result)
	//{
	//	return false;
	//}
	//viewMatrix = m_baseViewMatrix;
	//// Render the bitmap with the texture shader.
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	//if (!result)
	//{
	//	return false;
	//}

	m_Camera->GetViewMatrix(viewMatrix);



	// Turn on the alpha blending before rendering the text.

	m_D3D->TurnZBufferOn();


	//m_D3D->GetWorldMatrix(worldMatrix);
	//m_D3D->TurnOnAlphaBlending();
	//// Render the text strings.
	//result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	//if (!result)
	//{
	//	return false;
	//}
	//m_D3D->TurnOffAlphaBlending();
	
	
	
	worldMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model_plane->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model_plane->GetIndexCount(), 1, 
		worldMatrix, viewMatrix, projectionMatrix,
		m_Model_plane->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_G,lightPosition,
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	m_D3D->GetWorldMatrix(worldMatrix);
	
	worldMatrix = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Tree->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Tree->GetIndexCount(), m_Tree->GetInstanceCount(), 
		worldMatrix, viewMatrix, projectionMatrix,
		m_Tree->GetTexture(), 
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_G,lightPosition,
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	
	if(!result)
	{
		return false;
	}


	cameraPosition = m_Camera->GetPosition();

	modelPosition.x = 0.0f;
	modelPosition.y = 1.5f;
	modelPosition.z = 0.0f;

	// atan2 함수를 이용하여 빌보드 모델에 적용될 회전값을 계산합니다. 이렇게 하여 빌보드 모델이 현재 카메라 위치를 바라보게 합니다.
	angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / XM_PI);

	// 회전각도를 라디안으로 변환합니다.
	rotationX = (float)angle * 0.0174532925f;

	worldMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixRotationY(rotationX) * XMMatrixTranslation(modelPosition.x, modelPosition.y, modelPosition.z) * XMMatrixTranslation(0.0f, 50.0f + xpos_f / 2.0f, 0.0f);


	m_Model_bilboard->Render(m_D3D->GetDeviceContext());
	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model_bilboard->GetIndexCount(), 1,
		worldMatrix, viewMatrix, projectionMatrix,
		m_Model_bilboard->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	

	if (!result)
	{
		return false;
	}

	worldMatrix *= XMMatrixTranslation(0.0f, -5.0f, 0.0f);
	m_D3D->TurnOnAlphaBlending();
	// 파티클 시스템 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	m_ParticleSystem->Render(m_D3D->GetDeviceContext());


	// 텍스처 셰이더를  사용하여 모델을 렌더링합니다.
	if (!m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_ParticleSystem->GetTexture()))
	{
		return false;
	}
	m_D3D->TurnOffAlphaBlending();

	worldMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixRotationY(rotation_f * ((float)XM_PI / 180.0f)) * XMMatrixTranslation(xpos_f * 2.0f, 1.5f, -8.0f);
	foxWorld = worldMatrix;
	
	if (foxeaten == 0) {
		m_Fox->Render(m_D3D->GetDeviceContext());

		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Fox->GetIndexCount(), 1,
			worldMatrix, viewMatrix, projectionMatrix,
			m_Fox->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	}
	

	if (!result)
	{
		return false;
	}

	/*worldMatrix = XMMatrixTranslation(xpos_f, 0.5f, -5.0f);*/

	worldMatrix = XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixRotationY(-90.0f * ((float)XM_PI / 180.0f)) * XMMatrixTranslation(0.0f, ypos_d + 0.5f, -25.0f) * XMMatrixRotationY(-rotation);
	deerWorld = worldMatrix;

	if (deereaten == 0) {
		m_DeerA->Render(m_D3D->GetDeviceContext());

		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_DeerA->GetIndexCount(), 1,
			worldMatrix, viewMatrix, projectionMatrix,
			m_DeerA->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result)
		{
			return false;
		}
	}
	

	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetRotationMatrix(rotationMatrix);

	XMVECTOR modelOffset = XMVectorSet(0, 0, 3.0f, 0); // 카메라 바로 아래
	XMVECTOR camposition = XMVectorSet(cameraPosition.x, cameraPosition.y, cameraPosition.z, 0);

	XMVECTOR model = XMVector3TransformCoord(modelOffset, rotationMatrix);
	model = XMVectorAdd(model, camposition);
	
	worldMatrix = XMMatrixTranslationFromVector(model);
	worldMatrix = XMMatrixRotationY(180.0f * ((float)XM_PI / 180.0f)) * XMMatrixMultiply(rotationMatrix, worldMatrix);
	wolfWorld = worldMatrix;
	/*worldMatrix = XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixRotationY((rotation_f - 90.f) * ((float)XM_PI / 180.0f)) * XMMatrixTranslation(-10.0f, ypos_d + 0.5f, -10.0f + xpos_f * 4.0f);*/

	m_Wolf->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Wolf->GetIndexCount(), 1,
		worldMatrix, viewMatrix, projectionMatrix,
		m_Wolf->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	worldMatrix = XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixRotationY(-130.0f * ((float)XM_PI / 180.0f)) * XMMatrixTranslation(7.0f, 2.0f, -5.0f);
	
	m_Bizon->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Bizon->GetIndexCount(), 1,
		worldMatrix, viewMatrix, projectionMatrix,
		m_Bizon->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	worldMatrix = XMMatrixScaling(2.5f, 2.5f, 2.5f) * XMMatrixRotationY((90.0f + rotation_g) * ((float)XM_PI / 180.0f)) * XMMatrixTranslation(-18.0f, 2.0f, -3.0f);

	m_Giraf->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Bizon->GetIndexCount(), 1,
		worldMatrix, viewMatrix, projectionMatrix,
		m_Bizon->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), diffuseColor_A, lightPosition,
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->TurnOnAlphaBlending();
	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	m_D3D->TurnOffAlphaBlending();

	
	// Present the rendered scene to the screen.
	if (BoundingSphereCollision(foxBoundingSphere, foxCenterOffset, foxWorld, wolfBoundingSphere, wolfCenterOffset,wolfWorld))
	{
		foxeaten = 1;
	}

	if (BoundingSphereCollision(deerBoundingSphere, deerCenterOffset, deerWorld, wolfBoundingSphere, wolfCenterOffset, wolfWorld))
	{
		deereaten = 1;
	}

	m_D3D->EndScene();
	return true;
}

bool GraphicsClass::MainRender()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Title->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result)
	{
		return false;
	}
	viewMatrix = m_baseViewMatrix;
	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Title->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Title->GetTexture());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::RenderSkyDome()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMFLOAT3 cameraPosition{};
	bool result = false;

	
	m_D3D->TurnZBufferOff();

	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetViewMatrix(viewMatrix);


	cameraPosition = m_Camera->GetPosition();
	XMMATRIX Scale = XMMatrixScaling(50000.0f, 50000.0f, 50000.0f);
	worldMatrix = Scale * XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	//worldMatrix = XMMatrixScaling(5000.0f, 5000.0f, 5000.0f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	m_SkyDome->Render(m_D3D->GetDeviceContext());
	//m_D3D->GetDeviceContext()->UpdateSubresource(m_SkyDomeShader->GetMatrixBuffer(), 0, NULL, &m_SkyDome, 0, 0);
	result = m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyDome->GetTexture());

	
	m_D3D->TurnZBufferOn();

	return result;
}

bool GraphicsClass::BoundingSphereCollision(float firstObjBoundingSphere, XMVECTOR firstObjCenterOffset, XMMATRIX& firstObjWorldSpace, float secondObjBoundingSphere, XMVECTOR secondObjCenterOffset, XMMATRIX& secondObjWorldSpace)
{
	//Declare local variables
	XMVECTOR world_1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR world_2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float objectsDistance = 0.0f;

	//Transform the objects world space to objects REAL center in world space
	world_1 = XMVector3TransformCoord(firstObjCenterOffset, firstObjWorldSpace);
	world_2 = XMVector3TransformCoord(secondObjCenterOffset, secondObjWorldSpace);

	//Get the distance between the two objects
	objectsDistance = XMVectorGetX(XMVector3Length(world_1 - world_2));

	//If the distance between the two objects is less than the sum of their bounding spheres...
	if (objectsDistance <= (firstObjBoundingSphere + secondObjBoundingSphere))
		//Return true
		return true;

	//If the bounding spheres are not colliding, return false
	return false;
}



