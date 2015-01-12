#ifndef GAME_PERSON_H
#define GAME_PERSON_H

#include "Game\Unit.h"

class server;

class Person
    : public Unit
{
    public:
		Person(const std::string& _name);

		void Dispose();

    private:
};

#endif
