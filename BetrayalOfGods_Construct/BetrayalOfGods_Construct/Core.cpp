#include "Macro.h"

#include <boost\asio.hpp>
#include <boost\thread.hpp>
#include <boost\chrono.hpp>
#include <boost\lexical_cast.hpp>

#include "Window_Main.h"
#include "Window_NewWorld.h"
#include "ResourceManager.h"

#include <d3d9.h> 
#include <d3dx9.h> 

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

LPSTR path;

HWND Form;
LPDIRECT3DDEVICE9 device;

World* world;
Window* window;
ResourceManager* resources;

LRESULT CALLBACK HandleForm(HWND han_Wind, UINT uint_Message, WPARAM wParam, LPARAM lParam);
HWND NewForm(LPCTSTR str_Title, int int_Width, int int_Height);
void GameForm(LPCTSTR _title, const int& _width, const int& _height);
LPDIRECT3DDEVICE9 InitializeDevice(HWND han_WindowToBindTo);
void DrawGame(LPDIRECT3DDEVICE9 p_dx_Device);
void Render(LPDIRECT3DDEVICE9 p_dx_Device);
 
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPreviousInstance,LPSTR lpcmdline,int nCmdShow)
{
	path = new char[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);

	Form = NewForm("Betrayal of Gods - Construct", 1024 + 256 + 16, 768 + 36 + 39);
	device = InitializeDevice(Form);
	resources = new ResourceManager();
	window = new Window_Main();

	int return_value;
	MSG message;
	while((return_value = window->Return()) != 1)
	{
		switch (return_value)
		{
			case 2: window->Dispose(); window = new Window_Main(); break;
			case 3: window->Dispose(); window = new Window_NewWorld(); break;
		}

		if (PeekMessage(&message, Form, 0, 0, PM_REMOVE)) DispatchMessage(&message);
		window->Render();

		Sleep(5);
	}

	window->Dispose();
	resources->Dispose();

	device->Release();
	DestroyWindow(Form);
}

 ///

LRESULT CALLBACK HandleForm(HWND han_Wind,UINT uint_Message,WPARAM wParam,LPARAM lParam)
{
    switch(uint_Message)
    {
        case WM_KEYDOWN:
        {
			window->Handle_KeyUp(wParam); break;
        }
		
		case WM_LBUTTONUP:
		{
			window->Handle_MouseUp(LOWORD(lParam), HIWORD(lParam), 0); break;
		}

		case WM_RBUTTONUP:
		{
			window->Handle_MouseUp(LOWORD(lParam), HIWORD(lParam), 1); break;
		}

		//case WM_KEYUP:
		//{
		//	 switch(wParam)
		//	 {
		//		 case 'W': break;
		//		 case VK_RIGHT: break;
		//		 default: break;
		//	 }
		//}

		//case WM_MOUSEMOVE:
		//{

		//	// Retrieve mouse screen position
		//	int x=(short)LOWORD(lParam);
		//	int y=(short)HIWORD(lParam);

		//	// Check to see if the left button is held down:
		//	bool leftButtonDown=wParam & MK_LBUTTON;

		//	// Check if right button down:
		//	bool rightButtonDown=wParam & MK_RBUTTON;

		//}


		//case WM_RBUTTONUP:
		//{
		//	int xPos = GET_X_LPARAM(lParam); 
		//	int yPos = GET_Y_LPARAM(lParam); 
		//}

		case WM_CLOSE:
		{
            window->Close();
			break;
		}
    }

	return DefWindowProc(han_Wind, uint_Message, wParam, lParam);
}

HWND NewForm(LPCTSTR str_Title, int int_Width, int int_Height)
{
    WNDCLASSEX wnd_Structure;

    wnd_Structure.cbSize = sizeof(WNDCLASSEX);
    wnd_Structure.style = CS_HREDRAW | CS_VREDRAW;
    wnd_Structure.lpfnWndProc = HandleForm;
    wnd_Structure.cbClsExtra = 0;
    wnd_Structure.cbWndExtra = 0;
    wnd_Structure.hInstance = GetModuleHandle(NULL);
    wnd_Structure.hIcon = NULL;
    wnd_Structure.hCursor = NULL;
    wnd_Structure.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wnd_Structure.lpszMenuName = NULL;
    wnd_Structure.lpszClassName = "WindowClassName";
    wnd_Structure.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	//wnd_Structure.hIcon = (HICON) LoadImage(NULL, "BetrayalOfGods.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);

    RegisterClassEx(&wnd_Structure);

    return CreateWindowEx(WS_EX_CONTROLPARENT, "WindowClassName", str_Title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 200, 20, int_Width, int_Height, NULL, NULL, GetModuleHandle(NULL), NULL);
}

void GameForm(LPCTSTR _title, const int& _width, const int& _height)
{
	SetWindowText(Form, _title);
	SetWindowPos(Form, NULL, 0, 0, _width, _height, NULL); //SWP_NOMOVE | SWP_NOSIZE

	D3DPRESENT_PARAMETERS dx_PresParams;
		ZeroMemory( &dx_PresParams, sizeof(dx_PresParams) );
		dx_PresParams.Windowed = TRUE;
		dx_PresParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		dx_PresParams.BackBufferFormat = D3DFMT_UNKNOWN;
	device->Reset(&dx_PresParams);
}


LPDIRECT3DDEVICE9 InitializeDevice(HWND han_WindowToBindTo)
{
    LPDIRECT3D9 p_dx_Object;
    LPDIRECT3DDEVICE9 p_dx_Device;

    p_dx_Object = Direct3DCreate9(D3D_SDK_VERSION);
    if (p_dx_Object == NULL)
    {
        MessageBox(han_WindowToBindTo,"DirectX Runtime library not installed!","InitializeDevice()",MB_OK);
    }

    D3DPRESENT_PARAMETERS dx_PresParams;
		ZeroMemory( &dx_PresParams, sizeof(dx_PresParams) );
		dx_PresParams.Windowed = TRUE;
		dx_PresParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		dx_PresParams.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(p_dx_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, han_WindowToBindTo, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_PresParams, &p_dx_Device)))
		if (FAILED(p_dx_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, han_WindowToBindTo, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dx_PresParams, &p_dx_Device)))
			MessageBox(han_WindowToBindTo,"Failed to create even the reference device!","InitializeDevice()",MB_OK);

	p_dx_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	p_dx_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	p_dx_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    return p_dx_Device;
}