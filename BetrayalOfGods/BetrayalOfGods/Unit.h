#ifndef UNIT_H
#define UNIT_H

#include <string>

#include <d3d9.h> 
#include <d3dx9.h> 

#include "Game.h"

class Unit
{
public:
	D3DXVECTOR3 location;
	D3DXVECTOR3 move_vector;
	float speed;

	int id;
	int icon_id;
	std::string name;

	void Update(const float& _elapsedtime);

	void SetPosition(std::vector<std::string>& _args);

	virtual void Render(D3DXVECTOR3& _center, LPD3DXSPRITE _sprite) = 0;
	virtual void Dispose() = 0;

protected:
	bool disposed;

	Unit();
};

#endif