#include "Core\Macro.h"
#include "Manager\GameManager.h"

#include "Game\World.h"

GameManager::GameManager()
	: characters_generator(MAX_CHARACTERS)
{
	worlds.push_back(new World(0));
	//worlds.push_back(new World(1));

    #ifdef LOGGING
    Logger::Write(4, "\t>GameManager created!");
    #endif // LOGGING
}

World* GameManager::GetWorld(const int& _number)
{
	return worlds[_number];
}

