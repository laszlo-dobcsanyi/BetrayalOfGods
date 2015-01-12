#ifndef WORLD_H
#define WORLD_H

#include <string>

#include <boost\lexical_cast.hpp>

#include <d3d9.h> 
#include <d3dx9.h>

#include "Field.h"
#include "Field_Loaded.h"
#include "Field_Generated.h"
#include "ResourceManager.h"

extern LPSTR path;

struct Field_Descriptor
{
	int field_kind;
	Field* field;

	inline Field_Descriptor() : field(0), field_kind(-1) { };
	inline Field_Descriptor(Field_Loaded* _field_loaded) : field(_field_loaded), field_kind(1) { };
	inline Field_Descriptor(Field_Generated* _field_generated) : field(_field_generated), field_kind(0) { };

	inline void Save(std::ofstream& _stream) { field->Save(_stream); };
	inline void Dispose() { field->Dispose(); };
};

class World
{
public:
	int width, height;

	World(LPSTR _file_name);

	void Save();

	void Render(LPD3DXSPRITE _sprite, D3DXVECTOR3 _camera, const float& _scale);

	void SwitchField(const int& _x, const int& _y);

	void Dispose();

private:
	bool disposed;

	std::string name;
	const char* file_name;

	Field_Descriptor* fields;
};

#endif