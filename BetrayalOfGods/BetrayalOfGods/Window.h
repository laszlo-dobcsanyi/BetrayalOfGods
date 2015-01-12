#ifndef WINDOW_H
#define WINDOW_H

#include <boost\shared_ptr.hpp>

#include <string>

#include <d3d9.h> 
#include <d3dx9.h> 

class Connection;
class Handler;
class Message;

extern boost::shared_ptr<Connection> connection;

class Window
{
public:
	int Return() { return returnvalue; };
	void Close() { returnvalue = 1; };

	void Handle_Message(boost::shared_ptr<Message> _msg);

	virtual void NextActive() = 0;
	virtual void SetActive(const int& _index) = 0;

	virtual void Render(LPDIRECT3DDEVICE9 _device) = 0;
	virtual void Handle_KeyUp(const UINT_PTR& _key) = 0;
	virtual void Handle_MouseUp(const int& _x, const int& _y) = 0;
	virtual void Dispose() = 0;

protected:
	int returnvalue;
	bool disposed;

	Handler* active;
	int active_index;

	Window();
	void DrawString(const std::string& _text, LPD3DXFONT _font, const int& _x, const int& _y, const int& _width, const int& _height, const int& _a, const int& _r, const int& _g, const int& _b);
};

#endif