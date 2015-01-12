#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <boost\lexical_cast.hpp>

#include <d3d9.h> 
#include <d3dx9.h>

extern LPDIRECT3DDEVICE9 device;
extern LPSTR path;

struct Tile
{
public:
	int id;
	LPDIRECT3DTEXTURE9 texture;

	Tile() : disposed(true), id(-1), texture(0) { };
	Tile(const int& _id) : disposed(true), id(_id) { D3DXCreateTextureFromFile(device, (boost::lexical_cast<std::string>(path) + "\\data\\tiles\\" + boost::lexical_cast<std::string>(_id) + ".png").c_str(), &texture); };

	inline void Dispose() { if (disposed) return; disposed = true; texture->Release(); };

private:
	bool disposed;
};

class ResourceManager
{
public:
	LPD3DXFONT font;

	LPDIRECT3DTEXTURE9 panel;
	LPDIRECT3DTEXTURE9 tile_frame;
	LPDIRECT3DTEXTURE9 loaded_frame;

	LPDIRECT3DTEXTURE9 on;
	LPDIRECT3DTEXTURE9 off;


public:
	ResourceManager();
	
	void LoadWorld(LPSTR _file_name);
	LPDIRECT3DTEXTURE9 Get_TileTexture(int _id);

	void Dispose();

private:
	bool disposed;

	int tiles_number;
	Tile* tiles;
};

#endif