#ifndef BUTTON_H
#define BUTTON_H

#include "Handler.h"

extern LPDIRECT3DDEVICE9 device;

class Button
	: public Handler
{
public:
	Button(const Window* _parent, const int& _index, const std::string& _texture, const float& _x, const float& _y, const int& _width, const int& _height);

	void Render(LPD3DXSPRITE _sprite);
	void Handle_KeyUp(const UINT_PTR& _key);
	bool Handle_MouseUp(const int& _x, const int& _y);

	void Dispose();

private:
	RECT rectangle;
	D3DXVECTOR3 position;
	LPDIRECT3DTEXTURE9 texture;
	//D3DXIMAGE_INFO texture_info;

	void (*OnClick)();
};

#endif