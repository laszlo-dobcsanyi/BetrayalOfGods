#ifndef GAME_FIELD_GENERATED_H
#define GAME_FIELD_GENERATED_H

#include <vector>

#include "Game\Field.h"

class Field_Generated
	: public Field
{
public:
	Field_Generated();
	~Field_Generated();

	void Process();

	void ProcessAdding(boost::shared_ptr<Character> _character);
	void ProcessRemoving(boost::shared_ptr<Character> _character);

	void Dispose();

private:
	bool disposed;
};

#endif