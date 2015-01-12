#include "Macro.h"
#include "Window_Game.h"

#include <boost\lexical_cast.hpp>

#include "Connection.h"

void GameWindow::Handle_KeyUp(const UINT_PTR& _key)
{

}

void GameWindow::Handle_MouseUp(const int& _x, const int& _y)
{
	if ((512 - 32 <= _x) && (_x <= 512 + 32))
		if ((384 - 32 <= _y) && (_y <= 384 + 32))
			{ connection->Send(Connection::HERO_STOP, ""); return; }

	float direction = std::atan2(_y - 384, _x - 512);
	connection->Send(Connection::HERO_MOVE, boost::lexical_cast<std::string>(direction));
}