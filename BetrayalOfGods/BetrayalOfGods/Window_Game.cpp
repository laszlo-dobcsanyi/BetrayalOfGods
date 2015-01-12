#include "Macro.h"
#include "Window_Game.h"

#include "Game.h"
#include "Character.h"
#include "ResourceManager.h"

extern Game* game;
extern ResourceManager* resources;

GameWindow::GameWindow(LPDIRECT3DDEVICE9 _device)
	: Window()
{
	D3DXCreateFont(_device, 32, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial Black"), &font_loading);
}

void GameWindow::NextActive()
{

}

void GameWindow::SetActive(const int& _index)
{

}


void GameWindow::Render(LPDIRECT3DDEVICE9 _device)
{
	_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    _device->BeginScene();

	if ((game->hero != 0) && (resources->Ready()))
	{
		DrawFields(_device);
		DrawUnits(_device);
	}
	else
	{
		DrawLoading(_device);
	}

	_device->EndScene();
	_device->Present(NULL, NULL, NULL, NULL);
}


void GameWindow::Dispose()
{
	if (disposed) return; disposed = true;

	delete this;
}
