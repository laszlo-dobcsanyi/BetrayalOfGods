#ifndef OBJECT_H
#define OBJECT_H

#include <fstream>
#include <string>

#include <d3d9.h> 
#include <d3dx9.h>

extern LPDIRECT3DDEVICE9 device;

class Object
{
public:
	Object(std::ifstream& _stream);

	void Save(std::ofstream& _stream);

	void Dispose();

private:
	bool disposed;

	int id;
	float x, y;
	LPDIRECT3DTEXTURE9 texture;
};

#endif