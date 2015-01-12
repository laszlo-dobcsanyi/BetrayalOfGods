#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include "Window.h"

class Window_Main
	: public Window
{
public:
	Window_Main();

	void NextActive();
	void SetActive(const int& _index);

	void Render();
	void Handle_KeyUp(const UINT_PTR& _key);
	void Handle_MouseUp(const int& _x, const int& _y, const int& button);

	void Dispose();

private:
	Button* Button_NewWorld;
	Button* Button_LoadWorld;

	D3DXVECTOR3 camera;
	float scale;

	void LoadWorld();
};

#endif