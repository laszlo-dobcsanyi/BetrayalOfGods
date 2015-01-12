#ifndef FIELD_LOADED_H
#define FIELD_LOADED_H

#include <vector>

#include "Field.h"

extern LPDIRECT3DDEVICE9 device;

class World;
class Object;

class Field_Loaded
	: public Field
{
public:
	Field_Loaded();
	Field_Loaded(std::ifstream& _stream);

	void Save(std::ofstream& _stream);

	void Render(LPD3DXSPRITE _sprite, D3DXVECTOR3 _offset);

	void Dispose();

private:
	int tile_id;
	bool collision[32 * 32];

	std::vector<Object*> objects;
};

#endif