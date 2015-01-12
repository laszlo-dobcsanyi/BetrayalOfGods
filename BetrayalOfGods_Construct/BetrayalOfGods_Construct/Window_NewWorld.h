#ifndef WINDOW_NEWWORLD_H
#define WINDOW_NEWWORLD_H

#include "Window.h"

class Window_NewWorld
	: public Window
{
public:
	Window_NewWorld();

	void NextActive();
	void SetActive(const int& _index);

	void Render();
	void Handle_KeyUp(const UINT_PTR& _key);
	void Handle_MouseUp(const int& _x, const int& _y, const int& button);

	void Dispose();

private:
	TextBox* TextBox_Name;
	TextBox* TextBox_FileName;

	Button* Button_Create;

	void OnCreate();
};

#endif