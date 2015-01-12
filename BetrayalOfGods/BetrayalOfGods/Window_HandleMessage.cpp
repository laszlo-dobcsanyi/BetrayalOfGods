#include "Macro.h"
#include "Window_Game.h"

#include <vector>
#include <boost\lexical_cast.hpp>
#include <boost\algorithm\string.hpp>

#include "Character.h"
#include "Message.hpp"
#include "Connection.h"
#include "ResourceManager.h"

extern Game* game;
extern ResourceManager* resources;

void Window::Handle_Message(boost::shared_ptr<Message> _msg)
{
	_msg->DecodeBody();
	std::vector<std::string> args;
	boost::split(args, _msg->GetMsg(), boost::is_any_of(";"));

	switch (_msg->GetCommand())
	{				
		case Connection::LOGIN_OK: returnvalue = 2; break;
		case Connection::REGISTRATE_OK:  break;


		case Connection::ERROR_LOGIN:  break;
		case Connection::ERROR_REGISTRATE:  break;


		case Connection::HERO_DATA: game->hero = boost::shared_ptr<Character>(new Character(args)); break;

		case Connection::HERO_WORLD: resources->LoadWorld(boost::lexical_cast<int>(_msg->GetMsg())); break;


		case Connection::CHARACTER_ENTER:
			game->characters.Add(boost::shared_ptr<Character>(new Character(args))); break;

		case Connection::CHARACTER_MOVE:
			{
				int id = boost::lexical_cast<int>(args[0]);
				boost::shared_ptr<Character> character = game->GetCharacter(id);
				if (character != 0)
				{
					character->SetPosition(args);
				}
				else
				{

				}
			}
			break;

		default:
			//std::cout << "# Unkown Command @ Connection HandleMessage!\n";
			//Dispose();
			break;
	}

	_msg.reset();
}