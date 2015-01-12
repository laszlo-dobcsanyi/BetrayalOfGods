#ifndef MANAGER_GAMEMANAGER_H
#define MANAGER_GAMEMANAGER_H

#define MAX_CHARACTERS 512

#include <string>
#include <vector>

#include <boost\shared_array.hpp>

#include "Core\Generator.h"

class World;
class server;
class Character;

class GameManager
{
public:
	Generator characters_generator;

    GameManager();

	World* GetWorld(const int& _number);

private:
	std::vector<World*> worlds;
};

#endif
