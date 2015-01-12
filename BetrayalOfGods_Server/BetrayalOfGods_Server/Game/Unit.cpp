#include "Core\Macro.h"
#include "Game\Unit.h"

#include <math.h>
#include <sstream>

#include "Game\Region.h"

Unit::Unit(const int _id, const std::string& _name)
	: disposed(false),

	  world(0),
	  region(0),
	  state(0),

	  id(_id),
	  name(_name),
	  location(0, 0),
	  move_vector(0, 0),
	  direction(0),
	  moving(false),
	  speed(60),
	  icon_id(0)
{

}


std::string Unit::GetData()
{
	std::stringstream builder; builder << id << ";" << name << ";" << icon_id << ";" << location.x << ";" << location.y << ";" << direction << ";" << (moving ? speed : 0.0);
	return builder.str();
}

void Unit::Rotate(const float& _direction)
{
	move_vector.x = std::cos(_direction);
	move_vector.y = std::sin(_direction);
	direction = _direction;
	moving = true;

	Broadcast_Location();
}

void Unit::Stop()
{
	moving = false;

	Broadcast_Location();
}
