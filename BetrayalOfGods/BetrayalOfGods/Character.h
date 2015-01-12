#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>

#include "Unit.h"

class Character
	: public Unit
{
public:
	Character(std::vector<std::string>& _args);

	void Render(D3DXVECTOR3& _center, LPD3DXSPRITE _sprite);
	void Dispose();

private:
};

#endif