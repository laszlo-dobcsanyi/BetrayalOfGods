#ifndef CORE_SERVER_H
#define CORE_SERVER_H

class GameManager;
class DatabaseManager;
class ConnectionManager;

#include <boost\bind.hpp>
#include <boost\thread.hpp>
#include <boost\shared_ptr.hpp>

class server
{
public:
	server();

	GameManager* Manager_Game;
	DatabaseManager* Manager_Database;
	ConnectionManager* Manager_Connection;

private:
	boost::shared_ptr<boost::thread> database_thread;
	boost::shared_ptr<boost::thread> connection_thread;
};

#endif