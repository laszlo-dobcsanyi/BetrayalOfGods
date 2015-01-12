#include "Macro.h"
#include "Field_Loaded.h"

#include <boost\lexical_cast.hpp>

#include "World.h"
#include "Object.h"
#include "ResourceManager.h"

extern LPSTR path;

extern ResourceManager* resources;

Field_Loaded::Field_Loaded()
	: Field(),
	  tile_id(0)
{
	for (int row = 0; row < 32; ++row)
		for (int column = 0; column < 32; ++column)
			collision[row * 32 + column] = 0;
}

Field_Loaded::Field_Loaded(std::ifstream& _stream)
	: Field()
{
	int objects_number;

	_stream >> tile_id >> objects_number;

	while (objects_number--) objects.push_back(new Object(_stream));
}

void Field_Loaded::Save(std::ofstream& _stream)
{
	_stream << "\n0\t" << tile_id << "\t" << objects.size();
	for (int current = 0; current < objects.size(); ++current) objects[current]->Save(_stream);
}

void Field_Loaded::Render(LPD3DXSPRITE _sprite, D3DXVECTOR3 _offset)
{
	D3DXVECTOR3 location = D3DXVECTOR3(_offset.x * 512 + 256, _offset.y * 512 + 256 - 128 + 36, 0);
	_sprite->Draw(resources->Get_TileTexture(tile_id), NULL, NULL, &location, 0xFFFFFFFF);

	for (int row = 0; row < 32; ++row)
		for (int column = 0; column < 32; ++column)
			_sprite->Draw((collision[row * 32 + column] ? resources->on : resources->off), NULL, NULL, &D3DXVECTOR3(location.x + column * 16, location.y + row * 16, 0), 0xFFFFFFFF);

	_sprite->Draw(resources->loaded_frame, NULL, NULL, &location, 0xFFFFFFFF);
}

void Field_Loaded::Dispose()
{
	if (disposed) return; disposed = true;
}
