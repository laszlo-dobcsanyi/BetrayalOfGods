#include "Character.h"

#include "ResourceManager.h"

#include <vector>
#include <boost\lexical_cast.hpp>
#include <boost\algorithm\string.hpp>

extern Game* game;
extern ResourceManager* resources;

Character::Character(std::vector<std::string>& _args)
	: Unit()
{
	id = boost::lexical_cast<int>(_args[0]);
	name = _args[1];
	icon_id = boost::lexical_cast<int>(_args[2]);

	location.x = boost::lexical_cast<float>(_args[3]);
	location.y = boost::lexical_cast<float>(_args[4]);

	move_vector.x = cos(boost::lexical_cast<float>(_args[5]));
	move_vector.y = sin(boost::lexical_cast<float>(_args[5]));

	speed = boost::lexical_cast<float>(_args[6].data());
}

void Character::Render(D3DXVECTOR3& _center, LPD3DXSPRITE _sprite)
{
	D3DXVECTOR3 position(location.x - _center.x + 512 - 32, location.y - _center.y + 384 - 32, 0);
	_sprite->Draw(resources->character_icons[icon_id], NULL, NULL, &position, 0xFFFFFFFF);
}

void Character::Dispose()
{
	if (disposed) return; disposed = true;
}

