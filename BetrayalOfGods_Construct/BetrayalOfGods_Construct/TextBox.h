#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Handler.h"

class TextBox
	: public Handler
{
public:
	TextBox(const Window* _parent,const int& _index, const std::string& _value, const int& _x, const int& _y, const int& _width, const int& _height, D3DCOLOR _color, bool _authenticate);

	const std::string& GetText() { return text; }
	void SetText(const std::string& _text) { text = _text; }

	void Render(LPD3DXFONT _font);
	void Handle_KeyUp(const UINT_PTR& _key);
	bool Handle_MouseUp(const int& _x, const int& _y);

	void Dispose();

private:
	bool authenticate;
	std::string text;
	RECT position;
	D3DCOLOR color;
};

#endif