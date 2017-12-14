//--------------------------------------------------------------------------------------------------
// Project:		The Virtual Environment of Industrial Robots
// File:		main.cpp
//--------------------------------------------------------------------------------------------------

#include "resource.h"
#include "RobotEffectFactory.h"
#include "RobotComponent.h"

#include "DXUT.h"
#include "DXUTCamera.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"
#include "DXUTSettingsDlg.h"
#include "CommonStates.h"
#include "Model.h"

#pragma warning( disable : 4100 )

using namespace DirectX;
//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_JOINT1_INC			1
#define IDC_JOINT1_DEC          2
#define IDC_JOINT2_INC			3
#define IDC_JOINT2_DEC          4


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
RobotLeaf Leaf_0[7];
RobotComposite Joint_0[7];

RobotLeaf Leaf_1[14];
RobotComposite Joint_1[7];



CModelViewerCamera										g_Camera;

CDXUTDialogResourceManager								g_DialogResourceManager;
CD3DSettingsDlg											g_SettingsDlg;
CDXUTDialog												g_Dialog;

ID3D11InputLayout*										g_pBatchInputLayout = nullptr;

std::unique_ptr<BasicEffect>                            g_BatchEffect;
std::unique_ptr<PrimitiveBatch<VertexPositionColor>>    g_Batch;
std::unique_ptr<RobotEffectFactory>						g_FXFactory;
std::unique_ptr<CommonStates>							g_States;
std::unique_ptr<Model>                                  g_Model;
float g_a = 0.0f;
float g_b = 0.0f;



//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
void InitRobotTrees();
void InitApp();
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
	V_RETURN(g_SettingsDlg.OnD3D11CreateDevice(pd3dDevice));

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
	V_RETURN(g_SettingsDlg.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));

	// Setup the camera's projection parameters
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	g_Camera.SetProjParams(XM_PI / 4, fAspectRatio, 0.1f, 1000.0f);
	g_Camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	g_Camera.SetButtonMasks(0x00, MOUSE_WHEEL, MOUSE_LEFT_BUTTON);

	g_Dialog.SetLocation(30, pBackBufferSurfaceDesc->Height - 100);
	g_Dialog.SetSize(80, 80);

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
	XMMATRIX scale1 = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	XMMATRIX translate1 = XMMatrixTranslation(g_a, 0.0f, 0.0f);
	XMMATRIX translate2	= XMMatrixTranslation(0.0f, 0.0f, g_b);
	XMMATRIX transform1 = XMMatrixMultiply(scale1, translate1);
	XMMATRIX transform2 = XMMatrixMultiply(scale1, translate2);
	XMMATRIX transformnone = XMMatrixIdentity();


	XMMATRIX local = XMMatrixMultiply(mWorld, transform1);
	XMMATRIX local1 = XMMatrixMultiply(local, translate2);
	XMMATRIX local2 = XMMatrixMultiply(local1, transformnone);
	XMMATRIX local3 = XMMatrixMultiply(local2, transformnone);
	XMMATRIX local4 = XMMatrixMultiply(local3, transformnone);
	XMMATRIX local5 = XMMatrixMultiply(local4, transformnone);
	XMMATRIX local6 = XMMatrixMultiply(local5, transformnone);

	
	Joint_0[0].Render(pd3dImmediateContext, *g_States, local, mView, mProj);
	Joint_1[0].Render(pd3dImmediateContext, *g_States, local2, mView, mProj);
	
	// Render GUI
	DXUT_BeginPerfEvent(DXUT_PERFEVENTCOLOR, L"BUTTONS");
	g_Dialog.OnRender(fElapsedTime);
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
	g_SettingsDlg.OnD3D11DestroyDevice();

	DXUTGetGlobalResourceCache().OnDestroyDevice();

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

	// Pass messages to settings dialog if its active
	if (g_SettingsDlg.IsActive())
	{
		g_SettingsDlg.MsgProc(hWnd, uMsg, wParam, lParam);
		return 0;
	}

	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_Dialog.MsgProc(hWnd, uMsg, wParam, lParam);
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
	case IDC_JOINT1_INC:
		g_a += 1.0f;
		break;
	case IDC_JOINT1_DEC:
		g_a -= 1.0f;
		break;
	case IDC_JOINT2_INC:
		g_b += 1.0f;
		break;
	case IDC_JOINT2_DEC:
		g_b -= 1.0f;
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
	g_SettingsDlg.Init(&g_DialogResourceManager);
	g_Dialog.Init(&g_DialogResourceManager);
	g_Dialog.SetCallback(OnGUIEvent);

	int ButtonX = 30;
	int ButtonY = 30;
	int iX0 = 0;
	int iY0 = 0;
	int idX = 40;
	int idY = 40;

	g_Dialog.AddButton(IDC_JOINT1_INC, L"J1[+]", iX0, iY0, ButtonX, ButtonY);
	g_Dialog.AddButton(IDC_JOINT2_INC, L"J2[+]", iX0 += idX, iY0, ButtonX, ButtonY);

	iX0 = 0;
	iY0 += idY;

	g_Dialog.AddButton(IDC_JOINT1_DEC, L"J1[-]", iX0, iY0, ButtonX, ButtonY);
	g_Dialog.AddButton(IDC_JOINT2_DEC, L"J2[-]", iX0 += idX, iY0, ButtonX, ButtonY);


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
	Joint_1[2].addLeaf(Leaf_1[2]);
	Joint_1[2].addLeaf(Leaf_1[3]);
	Joint_1[3].addLeaf(Leaf_1[4]);
	Joint_1[3].addLeaf(Leaf_1[5]);
	Joint_1[3].addLeaf(Leaf_1[6]);
	Joint_1[3].addLeaf(Leaf_1[13]);
	Joint_1[4].addLeaf(Leaf_1[7]);
	Joint_1[4].addLeaf(Leaf_1[8]);
	Joint_1[4].addLeaf(Leaf_1[9]);
	Joint_1[5].addLeaf(Leaf_1[10]);
	Joint_1[5].addLeaf(Leaf_1[12]);
	Joint_1[6].addLeaf(Leaf_1[11]);
}