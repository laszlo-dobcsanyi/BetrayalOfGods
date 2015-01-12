#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>

#include "Game\Unit.h"

class server;
class Group;
class Field;
class Connection;

class Character
    : public Unit,
	  public boost::enable_shared_from_this<Character>
{
    public:
		boost::shared_ptr<Group> group;
		boost::shared_ptr<Connection> connection;

		Character(std::string& _name, boost::shared_ptr<Connection> _connection);
		~Character();

		uint8_t Update(const float& _elapsedtime);
		void Dispose();

		///

		void Broadcast_Enter();
		void Broadcast_Enter(Field* _field);
		void Broadcast_Location();
};

#endif
