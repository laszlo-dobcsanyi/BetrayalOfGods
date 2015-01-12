#include "Macro.h"
#include "Tile.h"

#include <fstream>

#include <boost\lexical_cast.hpp>

extern LPDIRECT3DDEVICE9 device;

ObjectTexture::ObjectTexture(const int& _id, const std::string& _texture)
	 : id(_id)
{
	D3DXCreateTextureFromFile(device, ("data\\objects\\" + _texture + ".png").c_str(), &texture);
}

Tile::Tile(const int& _id)
	: id(_id)
{
	//Open Tile file and load object textures
	std::ifstream tile_file("data\\tiles\\" + boost::lexical_cast<std::string>(id) + ".tile");
	if (tile_file)
	{
		//Load Tile Texture
		std::string texture;	tile_file >> texture;
		D3DXCreateTextureFromFile(device, ("data\\tiles\\" + texture + ".png").c_str(), &tile_texture);

		//Create ObjectTextures in each collision class
		int number;
		for(int current = 0; current < COLLISION_CLASSES; ++current)
		{
			tile_file >> number;
			for(int index = 0; index < number; ++index)
			{
				int object_id; tile_file >> id;
				std::string object_texture; tile_file >> object_texture;

				object_textures[current].push_back(new ObjectTexture(object_id, object_texture));
			}
		}
	}
	tile_file.close();
}

const LPDIRECT3DTEXTURE9 Tile::GetObjectTexture(const int& _id) const
{
	for(int current = 0; current < COLLISION_CLASSES; ++current)
		for (int index = 0; index < object_textures[current].size(); ++index)
			if (object_textures[current][index]->id == _id)
				return object_textures[current][index]->texture;
}

void Tile::Dispose()
{
	if (disposed) return; disposed = true;
	
	tile_texture->Release();

	for(int current = 0; current < COLLISION_CLASSES; ++current)
		for(int index = 0; index < object_textures[current].size(); ++index)
			{
				object_textures[current][index]->texture->Release();
				delete object_textures[current][index];
			}

	delete this;
}

