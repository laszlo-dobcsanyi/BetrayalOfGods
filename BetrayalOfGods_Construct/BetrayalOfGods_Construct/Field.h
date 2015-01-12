#ifndef FIELD_H
#define FIELD_H

#include <fstream>

#include <d3d9.h> 
#include <d3dx9.h>

class Field
{
public:
	inline Field() : disposed(false) { };

	virtual void Save(std::ofstream& _stream) = 0;

	virtual void Render(LPD3DXSPRITE _sprite, D3DXVECTOR3 _location) = 0;

	virtual void Dispose() = 0;

protected:
	bool disposed;
};

#endif