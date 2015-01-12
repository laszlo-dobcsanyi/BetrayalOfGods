#ifndef WINDOW_LAUNCHER_H
#define WINDOW_LAUNCHER_H

#include "Window.h"
#include "Button.h"
#include "TextBox.h"

class LauncherWindow
	: public Window
{
public:
	TextBox* TextBox_Username;
	TextBox* TextBox_Password;

	Button* Button_Login;
	Button* Button_Registrate;

	LauncherWindow(LPDIRECT3DDEVICE9 _device);

	void NextActive();
	void SetActive(const int& _index);

	void Render(LPDIRECT3DDEVICE9 _device);
	void Handle_KeyUp(const UINT_PTR& _key);
	void Handle_MouseUp(const int& _x, const int& _y);

	void Dispose();

private:
	LPDIRECT3DTEXTURE9 background;

	LPD3DXFONT font_input;
	LPD3DXFONT font_title;

	void OnLogin();
	void OnRegistration();
};

#endif