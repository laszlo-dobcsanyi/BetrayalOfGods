#include "TextBox.h"

TextBox::TextBox(const Window* _parent, const int& _index, const std::string& _value, const int& _x, const int& _y, const int& _width, const int& _height, D3DCOLOR _color, bool _authenticate)
	: Handler(_parent, _index),
	  color(_color),
	  authenticate(_authenticate),
	  text(_value)
{
	position.left = _x;
	position.top = _y;
	position.right = _x + _width;
	position.bottom = _y + _height;
}

void TextBox::Render(LPD3DXFONT _font)
{
	if (!authenticate) _font->DrawText(NULL, text.c_str(), -1, &position, 0, color);
	else
	{
		std::string mask = "";
		for(int i = 0; i < text.length(); ++i)
			mask.push_back('*');

		_font->DrawText(NULL, mask.c_str(), -1, &position, 0, color);
	}
}

void TextBox::Handle_KeyUp(const UINT_PTR& _key)
{
	switch(_key)
	{
		case 'A' : text += 'a'; break;
		case 'B' : text += 'b'; break;
		case 'C' : text += 'c'; break;
		case 'D' : text += 'd'; break;
		case 'E' : text += 'e'; break;
		case 'F' : text += 'f'; break;
		case 'G' : text += 'g'; break;
		case 'H' : text += 'h'; break;
		case 'I' : text += 'i'; break;
		case 'J' : text += 'j'; break;
		case 'K' : text += 'k'; break;
		case 'L' : text += 'l'; break;
		case 'M' : text += 'm'; break;
		case 'N' : text += 'n'; break;
		case 'O' : text += 'o'; break;
		case 'P' : text += 'p'; break;
		case 'Q' : text += 'q'; break;
		case 'R' : text += 'r'; break;
		case 'S' : text += 's'; break;
		case 'T' : text += 't'; break;
		case 'U' : text += 'u'; break;
		case 'V' : text += 'v'; break;
		case 'W' : text += 'w'; break;
		case 'X' : text += 'x'; break;
		case 'Y' : text += 'y'; break;
		case 'Z' : text += 'z'; break;

		case '0' : text += '0'; break;
		case '1' : text += '1'; break;
		case '2' : text += '2'; break;
		case '3' : text += '3'; break;
		case '4' : text += '4'; break;
		case '5' : text += '5'; break;
		case '6' : text += '6'; break;
		case '7' : text += '7'; break;
		case '8' : text += '8'; break;
		case '9' : text += '9'; break;

		case VK_BACK: /*text.erase(text.begin() + text.size() - 1);*/ if (text.length() != 0) text.pop_back(); break;
	}
}

bool TextBox::Handle_MouseUp(const int& _x, const int& _y)
{
	if ((position.left <= _x) && (_x <= position.right))
		if ((position.top <= _y) && (_y <= position.bottom))
			return true;

	return false;
}

void TextBox::Dispose()
{
	if (disposed) return; disposed = true;
}

