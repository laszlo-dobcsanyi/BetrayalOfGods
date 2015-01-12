#include "Game.h"

#include <boost\thread.hpp>
#include <boost\foreach.hpp>

#include <sstream>

#include "Character.h"

extern LPDIRECT3DDEVICE9 device;

Game::Game()
{

}

void Game::Update(const float& _elapsedtime)
{
	characters.Process_Removing();

	hero->Update(_elapsedtime);
	boost::unique_lock<boost::shared_mutex> characters_lock(characters.data.mutex);
	BOOST_FOREACH(boost::shared_ptr<Character> character, characters.data.list)
	{
		character->Update(_elapsedtime);
	}
	characters_lock.unlock();

	characters.Process_Adding();
}

boost::shared_ptr<Character> Game::GetCharacter(const int& _id)
{
	if (hero->id == _id) return hero;

	boost::shared_lock<boost::shared_mutex> characters_lock(characters.data.mutex);
	BOOST_FOREACH(boost::shared_ptr<Character> character, characters.data.list)
	{
		if (character->id == _id)
			return character;
	}

	return 0;
}


void Game::Dispose()
{
	if(disposed) return; disposed = true;

	delete this;
}

