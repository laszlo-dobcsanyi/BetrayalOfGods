#ifndef GAME_FIELD_LOADED_H
#define GAME_FIELD_LOADED_H

#include <string>

#include "Game\Field.h"

class Field_Loaded
	: public Field
{
public:
	Field_Loaded(const std::string& _filename);

	void Process();

	void ProcessAdding(boost::shared_ptr<Character> _character);
	void ProcessRemoving(boost::shared_ptr<Character> _character);

private:
};
#endif