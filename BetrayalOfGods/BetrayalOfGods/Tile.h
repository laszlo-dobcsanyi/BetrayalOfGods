#ifndef TILE_H
#define TILE_H

#include <vector>

#include <d3d9.h> 
#include <d3dx9.h> 

#define COLLISION_CLASSES 8

struct ObjectTexture
{
	int id;
	LPDIRECT3DTEXTURE9 texture;

	ObjectTexture(const int& _id, const std::string& _texture);
};

class Tile
{
public:
	int id;

	Tile(const int& _id);

	inline const LPDIRECT3DTEXTURE9 GetTexture() const { return tile_texture; };
	const LPDIRECT3DTEXTURE9 GetObjectTexture(const int& _id) const;
	 
	void Dispose();

private:
	bool disposed;

	LPDIRECT3DTEXTURE9 tile_texture;
	std::vector<ObjectTexture*> object_textures[COLLISION_CLASSES];
};

#endif