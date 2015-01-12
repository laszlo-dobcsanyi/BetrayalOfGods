#include "Macro.h"
#include "ResourceManager.h"

#include <fstream>

#include <boost\lexical_cast.hpp>

#include <d3d9.h> 
#include <d3dx9.h> 

#include "Tile.h"

extern LPDIRECT3DDEVICE9 device;

ResourceManager::ResourceManager()
	: disposed(false),
	  world_data(0)
{
	for (int faction = 0; faction < FACTIONS_NUMBER; faction++)
		for (int current = 0; current < CHARACTER_ICONS_NUMBER; current++)
		{
			std::stringstream name; name << "data\\icons\\hero"  << faction << "_" << current << ".png";
			D3DXCreateTextureFromFileEx(device, name.str().c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
                                        0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
                                        0, NULL, NULL, &character_icons[faction * CHARACTER_ICONS_NUMBER + current]);
		}

   /* for (int Current = 0; Current < 2 * 6 * 6 + 1; Current++)
        Spell.Icons[Current] = TextureLoader.FromFile(Device, @"data\icons\" + Spell.Icon_Names[Current] + ".png");

    for (int Current = 0; Current < 2 * 6 * 6 + 1; Current++)
        Mark.Icons[Current] = TextureLoader.FromFile(Device, @"data\icons\" + Mark.Icon_Names[Current] + ".png");*/
}

void ResourceManager::LoadWorld(const int& _id)
{
	std::ifstream world_file("data\\worlds\\" + boost::lexical_cast<std::string>(_id) + ".world");
	if (world_file)
	{
		world_file >> world_name;
		world_file >> world_width >> world_height;

		int tmp;
		int* tile_data = new int[world_width * world_height];
		for(int current = 0; current < world_width * world_height; ++current)
			world_file >> tmp >> tmp >> tile_data[current] >> tmp;
		world_data = tile_data;
	}
	world_file.close();
}

const Tile* const ResourceManager::GetTile(const int& _id)
{
	for(int current = 0; current < tiles.size(); ++current)
		if (tiles[current]->id == _id) return tiles[current];

	//Really?!
	Tile* tile = new Tile(_id);
	tiles.push_back(tile);
	return tile;
}


void ResourceManager::UpdateTileset(D3DXVECTOR3 _location)
{
	//TODO
}

void ResourceManager::Dispose()
{
	if (disposed) return; disposed = true;

	for(int current = 0; current < tiles.size(); ++current)
		tiles[current]->Dispose();
	
	for (int Faction = 0; Faction < FACTIONS_NUMBER; Faction++)
		for (int Current = 0; Current < CHARACTER_ICONS_NUMBER; Current++)
			character_icons[Faction * CHARACTER_ICONS_NUMBER + Current]->Release();

	delete[] world_data;

	delete this;
}
