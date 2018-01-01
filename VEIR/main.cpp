//--------------------------------------------------------------------------------------------------
// Project:		The Virtual Environment of Industrial Robots
// File:		main.cpp
//--------------------------------------------------------------------------------------------------

#include "resource.h"
#include "RobotEffectFactory.h"
#include "RobotComponent.h"
#include "RobotMatrix.h"

#include "DXUTmisc.h"
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTCamera.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"
#include "CommonStates.h"
#include "Model.h"

#pragma warning( disable : 4100 )

using namespace DirectX;
//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_X_INC_R0			1
#define IDC_X_DEC_R0			2
#define IDC_Y_INC_R0			3
#define IDC_Y_DEC_R0			4

#define IDC_X_INC_R1			5
#define IDC_X_DEC_R1			6
#define IDC_Y_INC_R1			7
#define IDC_Y_DEC_R1			8

#define IDC_SLIDER_R0J1			11
#define IDC_SLIDER_R0J2			12
#define IDC_SLIDER_R0J3			13
#define IDC_SLIDER_R0J4			14
#define IDC_SLIDER_R0J5			15
#define IDC_SLIDER_R0J6			16

#define IDC_SLIDER_R1J1			21
#define IDC_SLIDER_R1J2			22
#define IDC_SLIDER_R1J3			23
#define IDC_SLIDER_R1J4			24
#define IDC_SLIDER_R1J5			25
#define IDC_SLIDER_R1J6			26

#define IDC_IK_X_INC_R0			31
#define IDC_IK_X_DEC_R0			32
#define IDC_IK_Y_INC_R0			33
#define IDC_IK_Y_DEC_R0			34
#define IDC_IK_Z_INC_R0			35
#define IDC_IK_Z_DEC_R0			36
#define IDC_IK_SLIDER_X_R0		37
#define IDC_IK_SLIDER_Y_R0		38
#define IDC_IK_SLIDER_Z_R0		39

#define IDC_IK_X_INC_R1			41
#define IDC_IK_X_DEC_R1			42
#define IDC_IK_Y_INC_R1			43
#define IDC_IK_Y_DEC_R1			44
#define IDC_IK_Z_INC_R1			45
#define IDC_IK_Z_DEC_R1			46
#define IDC_IK_SLIDER_X_R1		47
#define IDC_IK_SLIDER_Y_R1		48
#define IDC_IK_SLIDER_Z_R1		49


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
const int JOINTS_1_COUNT = 7;
const int JOINTS_2_COUNT = 7;

RobotLeaf Leaf_0[7];
RobotComposite Joint_0[JOINTS_1_COUNT];

RobotLeaf Leaf_1[14];
RobotComposite Joint_1[JOINTS_2_COUNT];


CModelViewerCamera										g_Camera;

CDXUTDialogResourceManager								g_DialogResourceManager;
CDXUTDialog												g_Dialog;
CDXUTDialog												g_Dialog2;
CDXUTDialog												g_DialogIK0;
CDXUTDialog												g_DialogIK1;
CDXUTTextHelper*										g_pTxtHelper = nullptr;

ID3D11InputLayout*										g_pBatchInputLayout = nullptr;

std::unique_ptr<BasicEffect>                            g_BatchEffect;
std::unique_ptr<PrimitiveBatch<VertexPositionColor>>    g_Batch;
std::unique_ptr<RobotEffectFactory>						g_FXFactory;
std::unique_ptr<CommonStates>							g_States;
std::unique_ptr<Model>                                  g_Model;

std::wstring											g_WstringToPrint;
wchar_t													g_WstringBuffer[512];
RobotMatrix												g_MatrixToDisplay;



//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
void RenderText();
void InitRobotTrees();
void InitApp();
void SetOrigins();
void DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color);

//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
	DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
	return true;
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{

	return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	HRESULT hr;

	auto pd3dImmediateContext = DXUTGetD3D11DeviceContext();
	V_RETURN(g_DialogResourceManager.OnD3D11CreateDevice(pd3dDevice, pd3dImmediateContext));

	g_pTxtHelper = new CDXUTTextHelper(pd3dDevice,pd3dImmediateContext,&g_DialogResourceManager,15);

	// Create other render resources here
	g_States.reset(new CommonStates(pd3dDevice));
	g_FXFactory.reset(new RobotEffectFactory(pd3dDevice));
	g_Batch.reset(new PrimitiveBatch<VertexPositionColor>(pd3dImmediateContext));

	g_BatchEffect.reset(new BasicEffect(pd3dDevice));
	g_BatchEffect->SetVertexColorEnabled(true);

	{
		void const* shaderByteCode;
		size_t byteCodeLength;

		g_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		hr = pd3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			&g_pBatchInputLayout);
		if (FAILED(hr))
			return hr;
	}

	

	WCHAR str[MAX_PATH];
	g_FXFactory->SetPath(L"Models3D\\");
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\fs000.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_0[0].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\fs001.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_0[1].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\fs002.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_0[2].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\fs003.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_0[3].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\fs004.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_0[4].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\fs005.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_0[5].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\fs006.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_0[6].SetModel(move(g_Model));

	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv001.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[0].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv002.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[1].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv003.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[2].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv004.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[3].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv005.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[4].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv006.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[5].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv007.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[6].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv008.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[7].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv009.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[8].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv010.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[9].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv011.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[10].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv012.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[11].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv013.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[12].SetModel(move(g_Model));
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Models3D\\rv014.sdkmesh"));
	g_Model = Model::CreateFromSDKMESH(pd3dDevice, str, *g_FXFactory, true);
	Leaf_1[13].SetModel(move(g_Model));

	InitRobotTrees();
	SetOrigins();

	// Setup the camera's view parameters
	static const XMVECTORF32 s_vecEye = { 0.0f, 3.0f, -6.0f, 0.f };
	g_Camera.SetViewParams(s_vecEye, g_XMZero);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	HRESULT hr;
	V_RETURN(g_DialogResourceManager.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));

	// Setup the camera's projection parameters
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_Camera.SetProjParams(XM_PI / 4, fAspectRatio, 0.1f, 1000.0f);
	g_Camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	g_Camera.SetButtonMasks(0x00, MOUSE_WHEEL, MOUSE_LEFT_BUTTON);

	g_Dialog.SetLocation(15, 15);
	g_Dialog.SetSize(150, 300);

	g_Dialog2.SetLocation(pBackBufferSurfaceDesc->Width-165, 15);
	g_Dialog2.SetSize(150, 300);

	g_DialogIK0.SetLocation(15, 315);
	g_DialogIK0.SetSize(150, 150);

	g_DialogIK1.SetLocation(pBackBufferSurfaceDesc->Width - 165, 315);
	g_DialogIK1.SetSize(150, 150);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	// Update the camera's position based on user input 
	g_Camera.FrameMove(fElapsedTime);
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
	double fTime, float fElapsedTime, void* pUserContext)
{
	// Clear render target and the depth stencil 
	auto pRTV = DXUTGetD3D11RenderTargetView();
	pd3dImmediateContext->ClearRenderTargetView(pRTV, Colors::MidnightBlue);

	auto pDSV = DXUTGetD3D11DepthStencilView();
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

	// Get the projection & view matrix from the camera class
	XMMATRIX mWorld = g_Camera.GetWorldMatrix();
	XMMATRIX mView = g_Camera.GetViewMatrix();
	XMMATRIX mProj = g_Camera.GetProjMatrix();

	g_BatchEffect->SetView(mView);
	g_BatchEffect->SetProjection(mProj);

	// Draw procedurally generated dynamic grid
	const XMVECTORF32 xaxis = { 20.f, 0.f, 0.f };
	const XMVECTORF32 yaxis = { 0.f, 20.f, 0.f };
	DrawGrid(xaxis, yaxis, g_XMZero, 20, 20, Colors::Gray);

	// Draw 3D object
	XMMATRIX scale = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	XMMATRIX world_0 = XMMatrixMultiply(mWorld, scale);
	XMMATRIX world_1 = XMMatrixMultiply(mWorld, scale);
	Joint_0[0].SetParentOrigin(world_0);
	Joint_1[0].SetParentOrigin(world_1);

	Joint_0[0].Transform();
	Joint_1[0].Transform();

	Joint_0[0].Render(pd3dImmediateContext, *g_States, mView, mProj);
	Joint_1[0].Render(pd3dImmediateContext, *g_States, mView, mProj);
	
	// Render GUI
	DXUT_BeginPerfEvent(DXUT_PERFEVENTCOLOR, L"HUD / Stats");
	g_Dialog.OnRender(fElapsedTime);
	g_Dialog2.OnRender(fElapsedTime);
	g_DialogIK0.OnRender(fElapsedTime);
	g_DialogIK1.OnRender(fElapsedTime);
	RenderText();
	DXUT_EndPerfEvent();
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext)
{
	g_DialogResourceManager.OnD3D11ReleasingSwapChain();
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice(void* pUserContext)
{
	g_DialogResourceManager.OnD3D11DestroyDevice();

	DXUTGetGlobalResourceCache().OnDestroyDevice();

	SAFE_DELETE(g_pTxtHelper);

	g_States.reset();
	g_BatchEffect.reset();
	g_FXFactory.reset();
	g_Batch.reset();

	Leaf_0[0].ReleaseModel();
	Leaf_0[1].ReleaseModel();
	Leaf_0[2].ReleaseModel();
	Leaf_0[3].ReleaseModel();
	Leaf_0[4].ReleaseModel();
	Leaf_0[5].ReleaseModel();
	Leaf_0[6].ReleaseModel();

	Leaf_1[0].ReleaseModel();
	Leaf_1[1].ReleaseModel();
	Leaf_1[2].ReleaseModel();
	Leaf_1[3].ReleaseModel();
	Leaf_1[4].ReleaseModel();
	Leaf_1[5].ReleaseModel();
	Leaf_1[6].ReleaseModel();
	Leaf_1[7].ReleaseModel();
	Leaf_1[8].ReleaseModel();
	Leaf_1[9].ReleaseModel();
	Leaf_1[10].ReleaseModel();
	Leaf_1[11].ReleaseModel();
	Leaf_1[12].ReleaseModel();
	Leaf_1[13].ReleaseModel();


	SAFE_RELEASE(g_pBatchInputLayout);
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	bool* pbNoFurtherProcessing, void* pUserContext)
{
	// Pass messages to dialog resource manager calls so GUI state is updated correctly
	*pbNoFurtherProcessing = g_DialogResourceManager.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_Dialog.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	*pbNoFurtherProcessing = g_Dialog2.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	*pbNoFurtherProcessing = g_DialogIK0.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	*pbNoFurtherProcessing = g_DialogIK1.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	// Pass all remaining windows messages to camera so it can respond to user input
	g_Camera.HandleMessages(hWnd, uMsg, wParam, lParam);
	return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
}


//--------------------------------------------------------------------------------------
// Handle mouse button presses
//--------------------------------------------------------------------------------------
void CALLBACK OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
	bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
	int xPos, int yPos, void* pUserContext)
{
}


//--------------------------------------------------------------------------------------
// Call if device was removed.  Return true to find a new device, false to quit
//--------------------------------------------------------------------------------------
bool CALLBACK OnDeviceRemoved(void* pUserContext)
{
	return true;
}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	switch (nControlID)
	{
	case IDC_X_INC_R0:
		Joint_0[0].SetTranslationX(Joint_0[0].GetTranslationX()+250.f);
		break;
	case IDC_X_DEC_R0:
		Joint_0[0].SetTranslationX(Joint_0[0].GetTranslationX() - 250.f);
		break;
	case IDC_Y_INC_R0:
		Joint_0[0].SetTranslationY(Joint_0[0].GetTranslationY() + 250.f);
		break;
	case IDC_Y_DEC_R0:
		Joint_0[0].SetTranslationY(Joint_0[0].GetTranslationY() - 250.f);
		break;

	case IDC_X_INC_R1:
		Joint_1[0].SetTranslationX(Joint_1[0].GetTranslationX() + 250.f);
		break;
	case IDC_X_DEC_R1:
		Joint_1[0].SetTranslationX(Joint_1[0].GetTranslationX() - 250.f);
		break;
	case IDC_Y_INC_R1:
		Joint_1[0].SetTranslationY(Joint_1[0].GetTranslationY() + 250.f);
		break;
	case IDC_Y_DEC_R1:
		Joint_1[0].SetTranslationY(Joint_1[0].GetTranslationY() - 250.f);
		break;


	case IDC_SLIDER_R0J1:
		Joint_0[1].SetRotationZ(float(g_Dialog.GetSlider(IDC_SLIDER_R0J1)->GetValue()));
		break;
	case IDC_SLIDER_R0J2:
		Joint_0[2].SetRotationZ(float(g_Dialog.GetSlider(IDC_SLIDER_R0J2)->GetValue()));
		break;
	case IDC_SLIDER_R0J3:
		Joint_0[3].SetRotationZ(float(g_Dialog.GetSlider(IDC_SLIDER_R0J3)->GetValue()));
		break;
	case IDC_SLIDER_R0J4:
		Joint_0[4].SetRotationZ(float(g_Dialog.GetSlider(IDC_SLIDER_R0J4)->GetValue()));
		break;
	case IDC_SLIDER_R0J5:
		Joint_0[5].SetRotationZ(float(g_Dialog.GetSlider(IDC_SLIDER_R0J5)->GetValue()));
		break;
	case IDC_SLIDER_R0J6:
		Joint_0[6].SetRotationZ(float(g_Dialog.GetSlider(IDC_SLIDER_R0J6)->GetValue()));
		break;


	case IDC_SLIDER_R1J1:
		Joint_1[1].SetRotationZ(float(g_Dialog2.GetSlider(IDC_SLIDER_R1J1)->GetValue()));
		break;
	case IDC_SLIDER_R1J2:
		Joint_1[2].SetRotationZ(float(g_Dialog2.GetSlider(IDC_SLIDER_R1J2)->GetValue()));
		break;
	case IDC_SLIDER_R1J3:
		Joint_1[3].SetRotationZ(float(g_Dialog2.GetSlider(IDC_SLIDER_R1J3)->GetValue()));
		break;
	case IDC_SLIDER_R1J4:
		Joint_1[4].SetRotationZ(float(g_Dialog2.GetSlider(IDC_SLIDER_R1J4)->GetValue()));
		break;
	case IDC_SLIDER_R1J5:
		Joint_1[5].SetRotationZ(float(g_Dialog2.GetSlider(IDC_SLIDER_R1J5)->GetValue()));
		break;
	case IDC_SLIDER_R1J6:
		Joint_1[6].SetRotationZ(float(g_Dialog2.GetSlider(IDC_SLIDER_R1J6)->GetValue()));
		break;


	}
}

//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	// Enable run-time memory check for debug builds.
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// DXUT will create and use the best device
	// that is available on the system depending on which D3D callbacks are set below

	// Set general DXUT callbacks
	DXUTSetCallbackFrameMove(OnFrameMove);
	DXUTSetCallbackKeyboard(OnKeyboard);
	DXUTSetCallbackMouse(OnMouse);
	DXUTSetCallbackMsgProc(MsgProc);
	DXUTSetCallbackDeviceChanging(ModifyDeviceSettings);
	DXUTSetCallbackDeviceRemoved(OnDeviceRemoved);

	// Set the D3D11 DXUT callbacks. Remove these sets if the app doesn't need to support D3D11
	DXUTSetCallbackD3D11DeviceAcceptable(IsD3D11DeviceAcceptable);
	DXUTSetCallbackD3D11DeviceCreated(OnD3D11CreateDevice);
	DXUTSetCallbackD3D11SwapChainResized(OnD3D11ResizedSwapChain);
	DXUTSetCallbackD3D11FrameRender(OnD3D11FrameRender);
	DXUTSetCallbackD3D11SwapChainReleasing(OnD3D11ReleasingSwapChain);
	DXUTSetCallbackD3D11DeviceDestroyed(OnD3D11DestroyDevice);

	// Perform any application-level initialization here

	InitApp();
	DXUTInit(true, true, nullptr); // Parse the command line, show msgboxes on error, no extra command line params
	DXUTSetCursorSettings(true, true); // Show the cursor and clip it when in full screen
	DXUTCreateWindow(L"The Virtual Environment of Industrial Robots");

	// Only require 10-level hardware or later
	DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, 800, 600);
	DXUTMainLoop(); // Enter into the DXUT ren  der loop

					// Perform any application-level cleanup here

	return DXUTGetExitCode();
}


void InitApp()
{
	g_Dialog.Init(&g_DialogResourceManager);
	g_Dialog2.Init(&g_DialogResourceManager);
	g_DialogIK0.Init(&g_DialogResourceManager);
	g_DialogIK1.Init(&g_DialogResourceManager);
	g_Dialog.SetCallback(OnGUIEvent);
	g_Dialog2.SetCallback(OnGUIEvent);
	g_DialogIK0.SetCallback(OnGUIEvent);
	g_DialogIK1.SetCallback(OnGUIEvent);

	int SliderX = 150;
	int SliderY = 15;
	int ButtonX = 30;
	int ButtonY = 30;
	int jX0 = 0;
	int jY0 = 0;
	int jdY = 30;


	int idX = 0;
	int idY = 0;
	int iX0 = 30;
	int iY0 = 7*jdY;
	

	g_Dialog.AddSlider(IDC_SLIDER_R0J1, jX0, jY0, SliderX, SliderY, -180, 180, 0);
	g_Dialog.AddSlider(IDC_SLIDER_R0J2, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);
	g_Dialog.AddSlider(IDC_SLIDER_R0J3, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);
	g_Dialog.AddSlider(IDC_SLIDER_R0J4, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);
	g_Dialog.AddSlider(IDC_SLIDER_R0J5, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);
	g_Dialog.AddSlider(IDC_SLIDER_R0J6, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);

	g_Dialog.AddButton(IDC_Y_INC_R0, L"R0 Y[+]", iX0 + ButtonX + idX , iY0, ButtonX, ButtonY); //^
	g_Dialog.AddButton(IDC_X_DEC_R0, L"R0 X[-]", iX0, iY0 + ButtonY + idY,ButtonX, ButtonY);//<
	g_Dialog.AddButton(IDC_X_INC_R0, L"R0 X[+]", iX0 + 2*ButtonX + 2*idX, iY0+ButtonY+idY, ButtonX, ButtonY);//>
	g_Dialog.AddButton(IDC_Y_DEC_R0, L"R0 Y[-]", iX0 + ButtonX + idX, iY0 + 2*ButtonY + 2*idY, ButtonX, ButtonY);//v

	jX0 = 0;
	jY0 = 0;

	g_Dialog2.AddSlider(IDC_SLIDER_R1J1, jX0, jY0, SliderX, SliderY, -180, 180, 0);
	g_Dialog2.AddSlider(IDC_SLIDER_R1J2, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);
	g_Dialog2.AddSlider(IDC_SLIDER_R1J3, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);
	g_Dialog2.AddSlider(IDC_SLIDER_R1J4, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);
	g_Dialog2.AddSlider(IDC_SLIDER_R1J5, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);
	g_Dialog2.AddSlider(IDC_SLIDER_R1J6, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);

	g_Dialog2.AddButton(IDC_Y_INC_R1, L"R1 Y[+]", iX0 + ButtonX + idX, iY0, ButtonX, ButtonY); 
	g_Dialog2.AddButton(IDC_X_DEC_R1, L"R1 X[-]", iX0, iY0 + ButtonY + idY, ButtonX, ButtonY);
	g_Dialog2.AddButton(IDC_X_INC_R1, L"R1 X[+]", iX0 + 2 * ButtonX + 2 * idX, iY0 + ButtonY + idY, ButtonX, ButtonY);
	g_Dialog2.AddButton(IDC_Y_DEC_R1, L"R1 Y[-]", iX0 + ButtonX + idX, iY0 + 2 * ButtonY + 2 * idY, ButtonX, ButtonY);

	//INVERSE KINEMATICS DIALOGS
	iY0 = ButtonY;
	g_DialogIK0.AddButton(IDC_IK_X_INC_R0, L"X[+]", iX0, iY0, ButtonX, ButtonY);
	g_DialogIK0.AddButton(IDC_IK_Y_INC_R0, L"Y[+]", iX0 + 1 * ButtonX + 1 * idX, iY0, ButtonX, ButtonY);
	g_DialogIK0.AddButton(IDC_IK_Z_INC_R0, L"Z[+]", iX0 + 2 * ButtonX + 2 * idX, iY0, ButtonX, ButtonY);
	g_DialogIK0.AddButton(IDC_IK_X_DEC_R0, L"X[-]", iX0, iY0 + ButtonY + idY, ButtonX, ButtonY);
	g_DialogIK0.AddButton(IDC_IK_Y_DEC_R0, L"Y[-]", iX0 + 1 * ButtonX + 1 * idX, iY0 + ButtonY + idY, ButtonX, ButtonY);
	g_DialogIK0.AddButton(IDC_IK_Z_DEC_R0, L"Z[-]", iX0 + 2 * ButtonX + 2 * idX, iY0 + ButtonY + idY, ButtonX, ButtonY);
	jX0 = 0;
	jY0 = 4*ButtonY+4*idY;
	g_DialogIK0.AddSlider(IDC_IK_SLIDER_X_R0, jX0, jY0, SliderX, SliderY, -180, 180, 0);
	g_DialogIK0.AddSlider(IDC_IK_SLIDER_Y_R0, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);
	g_DialogIK0.AddSlider(IDC_IK_SLIDER_Z_R0, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);

	g_DialogIK1.AddButton(IDC_IK_X_INC_R1, L"X[+]", iX0, iY0, ButtonX, ButtonY);
	g_DialogIK1.AddButton(IDC_IK_Y_INC_R1, L"Y[+]", iX0 + 1 * ButtonX + 1 * idX, iY0, ButtonX, ButtonY);
	g_DialogIK1.AddButton(IDC_IK_Z_INC_R1, L"Z[+]", iX0 + 2 * ButtonX + 2 * idX, iY0, ButtonX, ButtonY);
	g_DialogIK1.AddButton(IDC_IK_X_DEC_R1, L"X[-]", iX0, iY0 + ButtonY + idY, ButtonX, ButtonY);
	g_DialogIK1.AddButton(IDC_IK_Y_DEC_R1, L"Y[-]", iX0 + 1 * ButtonX + 1 * idX, iY0 + ButtonY + idY, ButtonX, ButtonY);
	g_DialogIK1.AddButton(IDC_IK_Z_DEC_R1, L"Z[-]", iX0 + 2 * ButtonX + 2 * idX, iY0 + ButtonY + idY, ButtonX, ButtonY);
	jX0 = 0;
	jY0 = 4 * ButtonY + 4 * idY;
	g_DialogIK1.AddSlider(IDC_IK_SLIDER_X_R1, jX0, jY0, SliderX, SliderY, -180, 180, 0);
	g_DialogIK1.AddSlider(IDC_IK_SLIDER_Y_R1, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);
	g_DialogIK1.AddSlider(IDC_IK_SLIDER_Z_R1, jX0, jY0 += jdY, SliderX, SliderY, -180, 180, 0);


	


}

void DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
	auto context = DXUTGetD3D11DeviceContext();
	g_BatchEffect->Apply(context);

	context->IASetInputLayout(g_pBatchInputLayout);

	g_Batch->Begin();

	xdivs = std::max<size_t>(1, xdivs);
	ydivs = std::max<size_t>(1, ydivs);

	for (size_t i = 0; i <= xdivs; ++i)
	{
		float fPercent = float(i) / float(xdivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
		VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
		g_Batch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= ydivs; i++)
	{
		FLOAT fPercent = float(i) / float(ydivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
		VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
		g_Batch->DrawLine(v1, v2);
	}

	g_Batch->End();
}

void InitRobotTrees()
{
	Joint_0[0].addComposite(Joint_0[1]);
	Joint_0[1].addComposite(Joint_0[2]);
	Joint_0[2].addComposite(Joint_0[3]);
	Joint_0[3].addComposite(Joint_0[4]);
	Joint_0[4].addComposite(Joint_0[5]);
	Joint_0[5].addComposite(Joint_0[6]);

	Joint_0[0].addLeaf(Leaf_0[0]);
	Joint_0[1].addLeaf(Leaf_0[1]);
	Joint_0[2].addLeaf(Leaf_0[2]);
	Joint_0[3].addLeaf(Leaf_0[3]);
	Joint_0[4].addLeaf(Leaf_0[4]);
	Joint_0[5].addLeaf(Leaf_0[5]);
	Joint_0[6].addLeaf(Leaf_0[6]);

	Joint_1[0].addComposite(Joint_1[1]);
	Joint_1[1].addComposite(Joint_1[2]);
	Joint_1[2].addComposite(Joint_1[3]);
	Joint_1[3].addComposite(Joint_1[4]);
	Joint_1[4].addComposite(Joint_1[5]);
	Joint_1[5].addComposite(Joint_1[6]);

	Joint_1[0].addLeaf(Leaf_1[0]);
	Joint_1[1].addLeaf(Leaf_1[1]);
	Joint_1[1].addLeaf(Leaf_1[2]);
	Joint_1[2].addLeaf(Leaf_1[3]);
	Joint_1[3].addLeaf(Leaf_1[4]);
	Joint_1[3].addLeaf(Leaf_1[5]);
	Joint_1[3].addLeaf(Leaf_1[6]);
	Joint_1[4].addLeaf(Leaf_1[13]);
	Joint_1[4].addLeaf(Leaf_1[7]);
	Joint_1[4].addLeaf(Leaf_1[8]);
	Joint_1[4].addLeaf(Leaf_1[9]);
	Joint_1[5].addLeaf(Leaf_1[10]);
	Joint_1[5].addLeaf(Leaf_1[12]);
	Joint_1[6].addLeaf(Leaf_1[11]);
}

void SetOrigins()
{
	Joint_0[0].SetRotationX(0);
	Joint_0[0].SetRotationY(0);
	Joint_0[0].SetRotationZ(0);

	Joint_0[1].SetTxTzRxRz(100, 430, 90, 0);
	Joint_0[2].SetTxTzRxRz(750, 0, 0, 0);
	Joint_0[3].SetTxTzRxRz(40, 0, 90, 0);
	Joint_0[4].SetTxTzRxRz(0, 700,-90, 0);
	Joint_0[5].SetTxTzRxRz(0, 0, 90, 0);
	Joint_0[6].SetTxTzRxRz(0, 100, 0, 0);

	Joint_1[1].SetTxTzRxRz(150, 449.5, 90, 0);
	Joint_1[2].SetTxTzRxRz(560.5, 0, 0, 0);
	Joint_1[3].SetTxTzRxRz(80, 0, 90, 0);
	Joint_1[4].SetTxTzRxRz(0, 669.5, -90, 0);
	Joint_1[5].SetTxTzRxRz(0, 0, 90, 0);
	Joint_1[6].SetTxTzRxRz(0, 97.5, 0, 0);
}

void RenderText()
{
	POINT dialog_point;

	g_pTxtHelper->Begin();
	g_pTxtHelper->SetForegroundColor(Colors::Yellow);

	//#1 - ANGLES R1
	g_Dialog.GetLocation(dialog_point);
	g_pTxtHelper->SetInsertionPos(dialog_point.x, dialog_point.y+10);
	for (int i = 1; i < JOINTS_1_COUNT; i++)
	{
		g_WstringToPrint.clear();
		swprintf_s(g_WstringBuffer, L"THETA(%u) = ", i);
		g_WstringToPrint = g_WstringBuffer;
		g_WstringToPrint.append(std::to_wstring(Joint_0[i].GetRotationZ()));
		g_pTxtHelper->DrawTextLine(g_WstringToPrint.c_str());
		g_pTxtHelper->DrawTextLine(L"");
	}

	//#2 - ANGLES R2
	g_Dialog2.GetLocation(dialog_point);
	g_pTxtHelper->SetInsertionPos(dialog_point.x, dialog_point.y+10);
	for (int i = 1; i < JOINTS_2_COUNT; i++)
	{
		g_WstringToPrint.clear();
		swprintf_s(g_WstringBuffer, L"THETA(%u) = ", i);
		g_WstringToPrint = g_WstringBuffer;
		g_WstringToPrint.append(std::to_wstring(Joint_1[i].GetRotationZ()));
		g_pTxtHelper->DrawTextLine(g_WstringToPrint.c_str());
		g_pTxtHelper->DrawTextLine(L"");
	}

	//#3 POSITIONS R1
	g_Dialog.GetLocation(dialog_point);
	g_pTxtHelper->SetInsertionPos(dialog_point.x + 165, dialog_point.y);
	for (int i = 0; i < JOINTS_1_COUNT; i++)
	{
		g_MatrixToDisplay.LoadXMMATRIX(Joint_0[i].GetCurrentOrigin());
		g_WstringToPrint.clear();
		swprintf_s(g_WstringBuffer, L"P0[%u]=(", i);
		g_WstringToPrint = g_WstringBuffer;
		g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.px));
		g_WstringToPrint.append(L", ");
		g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.py));
		g_WstringToPrint.append(L", ");
		g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.pz));
		g_WstringToPrint.append(L")");
		g_pTxtHelper->DrawTextLine(g_WstringToPrint.c_str());
	}
	g_pTxtHelper->DrawTextLine(L"");
	//#4 POSITIONS R2
	for (int i = 0; i < JOINTS_2_COUNT; i++)
	{
		g_MatrixToDisplay.LoadXMMATRIX(Joint_1[i].GetCurrentOrigin());
		g_WstringToPrint.clear();
		swprintf_s(g_WstringBuffer, L"P1[%u]=(", i);
		g_WstringToPrint = g_WstringBuffer;
		g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.px));
		g_WstringToPrint.append(L", ");
		g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.py));
		g_WstringToPrint.append(L", ");
		g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.pz));
		g_WstringToPrint.append(L")");
		g_pTxtHelper->DrawTextLine(g_WstringToPrint.c_str());
	}
	/*
	////////////////////MACIERZ:POCZATEK

	g_Dialog.GetLocation(dialog_point);
	g_pTxtHelper->SetInsertionPos(dialog_point.x+300, dialog_point.y + 15);

	g_MatrixToDisplay.LoadXMMATRIX(Joint_0[1].GetCurrentOrigin());
	g_WstringToPrint.clear();
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r11));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r12));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r13));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.px));
	g_pTxtHelper->DrawTextLine(g_WstringToPrint.c_str());

	g_WstringToPrint.clear();
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r21));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r22));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r23));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.py));
	g_pTxtHelper->DrawTextLine(g_WstringToPrint.c_str());

	g_WstringToPrint.clear();
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r31));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r32));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r33));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.pz));
	g_pTxtHelper->DrawTextLine(g_WstringToPrint.c_str());

	g_WstringToPrint.clear();
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r314));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r324));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.r334));
	g_WstringToPrint.append(L", ");
	g_WstringToPrint.append(std::to_wstring(g_MatrixToDisplay.pz4));
	g_pTxtHelper->DrawTextLine(g_WstringToPrint.c_str());

	////////////////////MACIERZ:KONIEC
	*/

	g_pTxtHelper->End();
}