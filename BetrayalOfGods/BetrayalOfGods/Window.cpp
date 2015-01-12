#include "Window.h"

Window::Window()
	: returnvalue(0),
	  disposed(false),
	  active_index(0),
	  active(0)
{

}

void Window::DrawString(const std::string& _text, LPD3DXFONT _font, const int& _x, const int& _y, const int& _width, const int& _height, const int& _a, const int& _r, const int& _g, const int& _b)
{
	RECT rct;
	D3DCOLOR fontColor = D3DCOLOR_ARGB(_a, _r, _g, _b);   
	rct.left = _x; rct.right = _x + _width; rct.top = _y; rct.bottom = _y + _height;
	_font->DrawText(NULL, _text.c_str(), -1, &rct, DT_NOCLIP, fontColor);

}


