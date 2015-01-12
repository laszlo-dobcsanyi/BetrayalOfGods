#include "Macro.h"
#include "Field_Generated.h"

#include "World.h"
#include "ResourceManager.h"

extern ResourceManager* resources;

Field_Generated::Field_Generated(std::ifstream& _stream)
	: Field()
{
	int tiles_number, objects_number, tmp;

	_stream >> tiles_number;
	while (tiles_number--) { _stream >> tmp; tile_ids.push_back(tmp); }

	_stream >> objects_number;
	while (objects_number--) { _stream >> tmp; object_ids.push_back(tmp); }
}

void Field_Generated::Save(std::ofstream& _stream)
{
	_stream << "\n1\t\t" << tile_ids.size();
	for (int current = 0; current < tile_ids.size(); ++current) _stream << "\t" << tile_ids[current];
	_stream << "\t\t" << object_ids.size();
	for (int current = 0; current < object_ids.size(); ++current) _stream << "\t" << object_ids[current];
}

void Field_Generated::Render(LPD3DXSPRITE _sprite, D3DXVECTOR3 _offset)
{
	D3DXVECTOR3 location = D3DXVECTOR3(_offset.x * 512 + 256, _offset.y * 512 + 256 - 128 + 36, 0);
	_sprite->Draw(resources->Get_TileTexture(tile_ids[0]), NULL, NULL, &location, 0xFFFFFFFF);
}

void Field_Generated::Dispose()
{
	if (disposed) return; disposed = true;
}
