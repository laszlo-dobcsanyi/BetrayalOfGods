#include "Core\Macro.h"
#include "Core\Server.h"

#include "Manager\GameManager.h"
#include "Manager\DatabaseManager.h"
#include "Manager\ConnectionManager.h"

server::server()
{
    #ifdef LOGGING
    Logger::Write(4, ">Configurating..");
    #endif // LOGGING

        #ifdef LOGGING
        Logger::Write(4, ">Creating GameManager..");
        #endif // LOGGING
        Manager_Game = new GameManager();

		#ifdef LOGGING
		Logger::Write(4, ">Creating DatabaseManager..");
		#endif
		Manager_Database = new DatabaseManager();
		database_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&DatabaseManager::Run, Manager_Database)));

        #ifdef LOGGING
        Logger::Write(4, ">Creating ConnectionManager..");
        #endif // LOGGING
        Manager_Connection = new ConnectionManager();
		connection_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&ConnectionManager::Run, Manager_Connection)));

    #ifdef LOGGING
    Logger::Write(4, ">Configurated!");
    #endif // LOGGING
}
