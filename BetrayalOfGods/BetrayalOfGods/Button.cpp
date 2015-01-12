#include "Button.h"

Button::Button(const Window* _parent, const int& _index, const std::string& _texture, const float& _x, const float& _y,  const int& _width, const int& _height, LPDIRECT3DDEVICE9 _device)
	: Handler(_parent, _index)
{
	//D3DXGetImageInfoFromFile(("data\\interface\\" + _texture + ".png").c_str(), &texture_info);
	D3DXCreateTextureFromFile(_device, ("data\\interface\\" + _texture + ".png").c_str(), &texture);

	rectangle.left = _x;
	rectangle.top = _y;
	rectangle.right = _x + _width;
	rectangle.bottom = _y + _height;

	position = D3DXVECTOR3(_x, _y, 0);
}

void Button::Render(LPD3DXSPRITE _sprite)
{
	_sprite->Draw(texture, NULL, NULL, &position, 0xFFFFFFFF);
}

void Button::Handle_KeyUp(const UINT_PTR& _key)
{

}

bool Button::Handle_MouseUp(const int& _x, const int& _y)
{
	if ((rectangle.left <= _x) && (_x <= rectangle.right))
		if ((rectangle.top <= _y) && (_y <= rectangle.bottom))
			return true;

	return false;
}

void Button::Dispose()
{
	if (disposed) return; disposed = true;

	texture->Release();
	//textureinfo?
}

