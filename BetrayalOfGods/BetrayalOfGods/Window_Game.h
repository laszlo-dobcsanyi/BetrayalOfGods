#ifndef WINDOW_GAME_H
#define WINDOW_GAME_H

#include "Window.h"

#include "Game.h"

class GameWindow
	: public Window
{
public:
	GameWindow(LPDIRECT3DDEVICE9 _device);

	void NextActive();
	void SetActive(const int& _index);

	void Render(LPDIRECT3DDEVICE9 _device);
	void Handle_KeyUp(const UINT_PTR& _key);
	void Handle_MouseUp(const int& _x, const int& _y);

	void Dispose();

private:
	LPD3DXFONT font_loading;

	void DrawFields(LPDIRECT3DDEVICE9 _device);
	void DrawUnits(LPDIRECT3DDEVICE9 _device);

	void DrawLoading(LPDIRECT3DDEVICE9 _device);
};

#endif