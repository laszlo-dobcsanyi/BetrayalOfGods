#ifndef GAME_H
#define GAME_H

#include <boost\shared_ptr.hpp>

#include <d3d9.h> 
#include <d3dx9.h> 

#include "ListProcessable.hpp"

class Character;

class Game
{
public:
	boost::shared_ptr<Character> hero;
	ListProcessable<boost::shared_ptr<Character> > characters;

	Game();

	void Update(const float& _elapsedtime);

	boost::shared_ptr<Character> GetCharacter(const int& _id);

	void Dispose();

private:
	bool disposed;
};

#endif