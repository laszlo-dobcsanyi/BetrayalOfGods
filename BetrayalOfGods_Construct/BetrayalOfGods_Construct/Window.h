#ifndef WINDOW_H
#define WINDOW_H

#include <d3d9.h> 
#include <d3dx9.h>

class World;
class ResourceManager;

class Handler;
class Button;
class TextBox;

extern LPDIRECT3DDEVICE9 device;

extern World* world;
extern ResourceManager* resources;

class Window
{
public:
	inline int Return() { return return_value; };
	inline void Close() { return_value = 1; };

	virtual void NextActive() = 0;
	virtual void SetActive(const int& _index) = 0;

	virtual void Render() = 0;
	virtual void Handle_KeyUp(const UINT_PTR& _key) = 0;
	virtual void Handle_MouseUp(const int& _x, const int& _y, const int& button) = 0;

	virtual void Dispose() = 0;

protected:
	bool disposed;
	int return_value;

	Handler* active;
	int active_index;

	inline Window() : disposed(false), return_value(0), active(0), active_index(0) { };
};

#endif