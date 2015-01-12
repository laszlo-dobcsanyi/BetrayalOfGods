#include "Core\Macro.h"
#include "Game\Connection.h"

#include <vector>
#include <boost\lexical_cast.hpp>
#include <boost\algorithm\string.hpp>

#include "Core\Server.h"
#include "Core\Message.hpp"
#include "Manager\DatabaseManager.h"
#include "Manager\GameManager.h"
#include "Game\Group.h"
#include "Game\World.h"
#include "Game\Character.h"

/** */
void Connection::HandleMessage(boost::shared_ptr<Message> _msg)
{
	_msg->DecodeBody();
	std::vector<std::string> args;
	boost::split(args, _msg->GetMsg(), boost::is_any_of(";"));

	switch (_msg->GetCommand())
	{
		case LOGIN:
			if (name_in_database == "\0" && Server->Manager_Database->Authenticate(args[0], args[1], shared_from_this()))
			{
				name_in_database = args[0];

				Send(LOGIN_OK, "");

				boost::shared_ptr<Character> character = boost::shared_ptr<Character>(new Character(name_in_database, shared_from_this()));

				boost::shared_ptr<Group> group = boost::shared_ptr<Group>(new Group(character));
				character->group = group;

				this->character = character;
			}
			else Send(ERROR_LOGIN, "");
			break;

		case REGISTRATE:
			if (name_in_database == "\0" && Server->Manager_Database->Registrate(args[0], args[1])) Send(REGISTRATE_OK, "");
			else Send(ERROR_REGISTRATE, "");
			break;

		if (character != 0)
		{
			case HERO_MOVE: character->Rotate(boost::lexical_cast<float>(args[0])); break;

			case HERO_STOP: character->Stop(); break;
		}

		default:
			#ifdef LOGGING
			Logger::Write(0, "# Unkown Command @ Connection HandleMessage!");
			#endif
			Dispose();
			break;
	}
}