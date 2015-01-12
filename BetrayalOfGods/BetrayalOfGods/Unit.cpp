#include "Unit.h"

#include <math.h>
#include <boost\lexical_cast.hpp>

Unit::Unit()
	: disposed(false),
	location(0, 0, 0),
	move_vector(0, 0, 0),
	speed(0)
{

}

void Unit::Update(const float& _elapsedtime)
{
	if (speed != 0)
	{
		location.x += move_vector.x * speed * _elapsedtime;
		location.y += move_vector.y * speed * _elapsedtime;
	}
}

void Unit::SetPosition(std::vector<std::string>& _args)
{
	location.x = boost::lexical_cast<float>(_args[1]);
	location.y = boost::lexical_cast<float>(_args[2]);

	move_vector.x = cos(boost::lexical_cast<float>(_args[3]));
	move_vector.y = sin(boost::lexical_cast<float>(_args[3]));

	speed = boost::lexical_cast<float>(_args[4]);
}





