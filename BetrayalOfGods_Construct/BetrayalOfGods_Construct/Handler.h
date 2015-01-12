#ifndef HANDLER_H
#define HANDLER_H

#include <string>

#include <d3d9.h> 
#include <d3dx9.h> 

class Window;

class Handler
{
public:
	virtual void Handle_KeyUp(const UINT_PTR& _key) = 0;
	virtual bool Handle_MouseUp(const int& _x, const int& _y) = 0;

	virtual void Dispose() = 0;

protected:
	int index;
	bool disposed;

	const Window* parent;

	Handler(const Window* _parent, const int& _index);
};

#endif