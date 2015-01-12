#ifndef GAME_FIELD_BOUNDARY_H
#define GAME_FIELD_BOUNDARY_H

#include "Game\Field_Loaded.h"

class Field_Boundary
	: public Field_Loaded
{
public:
		void Broadcast_Data(boost::shared_ptr<Character> _character);
private:

};

#endif