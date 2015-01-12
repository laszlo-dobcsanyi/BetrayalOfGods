#include "Macro.h"
#include "World.h"

#include <fstream>

#include <boost\lexical_cast.hpp>

#include "ResourceManager.h"

extern ResourceManager* resources;

World::World(LPSTR _file_name)
	: file_name(_file_name),
	  disposed(false)
{
	resources->LoadWorld(_file_name);

	std::ifstream world_file(file_name);

	if (world_file)
	{
		std::getline(world_file, name);
		world_file >> width >> height;

		fields = new Field_Descriptor[width * height];

		int tmp;
		for (int current = 0; current < width * height; ++current)
		{
			world_file >> tmp;
			switch (tmp)
			{
				case 0: fields[current] = Field_Descriptor(new Field_Loaded(world_file)); break;
				case 1: fields[current] = Field_Descriptor(new Field_Generated(world_file));break;
			}
		}

		world_file.close();
	}
	else MessageBox(NULL, "Error while opening file!", "Error" , MB_ICONERROR | MB_OK);
}

void World::Save()
{
	std::ofstream world_file((boost::lexical_cast<std::string>(path) + boost::lexical_cast<std::string>(file_name)).c_str());

	world_file << name << "\n" << width << "\t" << height;

	for (int current = 0; current < width * height; ++current) fields[current].Save(world_file);
}

void World::Render(LPD3DXSPRITE _sprite, D3DXVECTOR3 _camera, const float& _scale)
{
	int size = 1 / _scale;

	for(int row = -size; row <= size; row++)
		for (int column = -size; column <= size; column++)
			if ((0 <= _camera.x + column && _camera.x + column < width) && (0 <= _camera.y + row && _camera.y + row < height))
			{
				fields[((int)_camera.x + column) + ((int)_camera.y + row) * width].field->Render(_sprite, D3DXVECTOR3(column, row, 0));
				if (row == 0 && column == 0)
				{
					D3DXVECTOR3 location = D3DXVECTOR3(256, 256 - 128 + 36, 0);
					_sprite->Draw(resources->tile_frame, NULL, NULL, &location, 0xFFFFFFFF);
				}
			}
}

void World::SwitchField(const int& _x, const int& _y)
{
	if (fields[_y * width + _x].field_kind == -1) return;

	if (fields[_y * width + _x].field_kind == 0)
	{
		fields[_y * width + _x].Dispose();
		fields[_y * width + _x] = new Field_Loaded();
		return;
	}

	if (fields[_y * width + _x].field_kind == 1)
	{
		fields[_y * width + _x].Dispose();
		fields[_y * width + _x] = new Field_Generated();
		return;
	}
}

void World::Dispose()
{
	if (disposed) return; disposed = true;

	delete[] file_name;

	for(int current = 0; current < width * height; ++current)
		fields[current].Dispose();
}
