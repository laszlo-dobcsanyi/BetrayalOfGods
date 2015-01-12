#include "Core\Macro.h"
#include "Game\Character.h"

#include <fstream>

#include "Core\Server.h"
#include "Core\Message.hpp"
#include "Manager\GameManager.h"
#include "Game\Connection.h"
#include "Game\World.h"
#include "Game\Field.h"
#include "Game\Region.h"
#include "Group.h"

Character::Character(std::string& _name, boost::shared_ptr<Connection> _connection)
	: Unit(Server->Manager_Game->characters_generator.Next(), _name),
	  connection(_connection)
{
	#ifdef LOGGING
	Logger::Write(7, "\t>Loading Character (" + name + ")..");
	#endif

	icon_id = 3;
	
	location.x = 8 * 512 ; location.y = 7 * 512;

	int spawn_world = 0;

	world = Server->Manager_Game->GetWorld(spawn_world);

	std::stringstream builder; builder << id << ";" << name << ";" << icon_id << ";" << location.x << ";" << location.y << ";" << direction << ";" << (moving ? speed : 0.0);
	connection->Send(Connection::HERO_DATA, builder.str());

	builder.str(""); builder.clear(); builder << world->id;
	connection->Send(Connection::HERO_WORLD, builder.str());
}

uint8_t Character::Update(const float& _elapsedtime)
{
	state = 0x00000000;

	if (moving)
	{
		location.x += (move_vector.x * speed * _elapsedtime);
		location.y += (move_vector.y * speed * _elapsedtime);

		if (region->x < (int)(location.x / 512)) { //std::cout << "Character moving " << region->x << ":" << region->y << " -> " << region->neighbours[0]->x  << ":" << region->neighbours[0]->y << std::endl;
			region = region->neighbours[0]; state |= Unit_State::REGION_MOVING; }
		if ((int)(location.x / 512) < region->x) { //std::cout << "Character moving " << region->x << ":" << region->y << " -> " << region->neighbours[4]->x  << ":" << region->neighbours[4]->y << std::endl; 
			region = region->neighbours[4];	state |= Unit_State::REGION_MOVING;}
		if (region->y < (int)(location.y / 512)) { //std::cout << "Character moving " << region->x << ":" << region->y << " -> " << region->neighbours[6]->x  << ":" << region->neighbours[6]->y << std::endl;
			region = region->neighbours[6]; state |= Unit_State::REGION_MOVING; }
		if ((int)(location.y / 512) < region->y) { //std::cout << "Character moving " << region->x << ":" << region->y << " -> " << region->neighbours[2]->x  << ":" << region->neighbours[2]->y << std::endl;
			region = region->neighbours[2]; state |= Unit_State::REGION_MOVING;}
	}

	return state;
}

void Character::Dispose()
{
	if (disposed) return; disposed = true;

	#ifdef LOGGING
	Logger::Write(5, "\t>Disposing Character..");
	#endif

	if (group != 0) group->characters.Remove(shared_from_this());

	connection.reset();

	//world.reset();
}

Character::~Character()
{
	#ifdef LOGGING
	Logger::Write(4, "\t>Character destructor..");
	#endif
}

///

void Character::Broadcast_Enter()
{
	std::cout << "Broadcasting Enter..\n";
	boost::shared_ptr<Message> msg = boost::shared_ptr<Message>(new Message(Connection::CHARACTER_ENTER, GetData()));

	region->BroadcastMessage(msg);
}

void Character::Broadcast_Enter(Field* _field)
{
	std::cout << "Broadcasting Enter (Field)..\n";

	boost::shared_ptr<Message> msg = boost::shared_ptr<Message>(new Message(Connection::CHARACTER_ENTER, GetData()));

	_field->BroadcastMessage(msg);
}

void Character::Broadcast_Location()
{
	std::cout << "Broadcasting Location..\n";

	std::stringstream builder; builder << id << ";" << location.x << ";" << location.y << ";" << direction << ";" << (moving ? speed : 0.0);
	boost::shared_ptr<Message> msg = boost::shared_ptr<Message>(new Message(Connection::CHARACTER_MOVE, builder.str()));

	region->BroadcastMessage(msg);
}