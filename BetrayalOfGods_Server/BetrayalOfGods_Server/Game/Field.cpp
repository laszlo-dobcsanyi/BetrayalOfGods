#include "Core\Macro.h"
#include "Game\Field.h"

#include <boost\foreach.hpp>

#include "Core\Message.hpp"
#include "Game\Character.h"
#include "Game\Connection.h"

void Field::Process_Messages()
{
	boost::unique_lock<boost::shared_mutex> lock(messages.mutex);

	BOOST_FOREACH(boost::shared_ptr<Character> character, characters.data.list)
	{
		BOOST_FOREACH(boost::shared_ptr<Message> message, messages.list)
		{
			character->connection->Send(message);
		}
	}
	messages.list.clear();
	messages.number = 0;

	lock.unlock();}

void Field::Broadcast_Enter(boost::shared_ptr<Character> _character)
{
	boost::shared_lock<boost::shared_mutex> lock(characters.data.mutex);
	BOOST_FOREACH(boost::shared_ptr<Character> character, characters.data.list)
	{
		_character->connection->Send(Connection::CHARACTER_ENTER, character->GetData());
	}
	lock.unlock();

	BOOST_FOREACH(Object* object, objects)
	{
		_character->connection->Send(Connection::OBJECT_ADD, object->GetData());
	}
}

void Field::Broadcast_Leave(boost::shared_ptr<Character> _character)
{

}



