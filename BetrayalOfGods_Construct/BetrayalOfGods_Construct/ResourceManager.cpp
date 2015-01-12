#include "Macro.h"
#include "ResourceManager.h"

#include <vector>
#include <fstream>
#include <sstream>

#include <boost\lexical_cast.hpp>
#include <boost\algorithm\string.hpp>


ResourceManager::ResourceManager()
	: disposed(false),
	  tiles_number(-1)
{
	D3DXCreateFont(device, 32, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial Black"), &font);

	D3DXCreateTextureFromFile(device, (boost::lexical_cast<std::string>(path) + "\\data\\interface\\panel.png").c_str(), &panel); 
	D3DXCreateTextureFromFile(device, (boost::lexical_cast<std::string>(path) + "\\data\\interface\\tile_frame.png").c_str(), &tile_frame); 
	D3DXCreateTextureFromFile(device, (boost::lexical_cast<std::string>(path) + "\\data\\interface\\loaded_frame.png").c_str(), &loaded_frame); 

	D3DXCreateTextureFromFile(device, (boost::lexical_cast<std::string>(path) + "\\data\\interface\\on.png").c_str(), &on); 
	D3DXCreateTextureFromFile(device, (boost::lexical_cast<std::string>(path) + "\\data\\interface\\off.png").c_str(), &off); 
}

void ResourceManager::LoadWorld(LPSTR _file_name)
{
	//Dispose loaded tiles
	for (int current = 0; current < tiles_number; ++current) tiles[current].Dispose();

	//Split the file name
	std::vector<std::string> args;
	boost::split(args, _file_name, boost::is_any_of("."));

	//Pop file format, push new format
	args.pop_back();
	args.push_back(".resource");

	//Create the string with the arguments and the file format
	std::stringstream file_name_stream;
	for(unsigned int current = 0; current < args.size(); ++current)
		file_name_stream << args[current];

	//Open file
	std::ifstream resource_file(file_name_stream.str());

	if (resource_file)
	{
		//Read the tiles number and allocate memory
		resource_file >> tiles_number; tiles = new Tile[tiles_number];

		//Create the tiles
		for (int current = 0; current < tiles_number; ++current) { int tile; resource_file >> tile; tiles[current] = Tile(tile); }
	} else { MessageBox(NULL, "Error while opening resource file!", "Error" , MB_ICONERROR | MB_OK); return; }

	//Close file
	resource_file.close();
}

LPDIRECT3DTEXTURE9 ResourceManager::Get_TileTexture(int _id)
{
	for(int current = 0; current < tiles_number; ++current) if (tiles[current].id == _id) return tiles[current].texture; return 0;
}

void ResourceManager::Dispose()
{
	if (disposed) return; disposed = true;

	font->Release();

	panel->Release();
	tile_frame->Release();
	
	for (int current = 0; current < tiles_number; ++current) tiles[current].Dispose();
}

