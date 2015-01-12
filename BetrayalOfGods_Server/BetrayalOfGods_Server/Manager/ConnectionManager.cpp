#include "Core\Macro.h"
#include "Manager\ConnectionManager.h"

#include <boost\lexical_cast.hpp>

#include "Game\Connection.h"

ConnectionManager::ConnectionManager()
	: gateway(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), SERVER_PORT))
{
    #ifdef LOGGING
	Logger::Write(4, "\t>ConnectionManager created @ [127.0.0.1:asdasd]!");
    #endif // LOGGING
}

void ConnectionManager::Run()
{	
    Accept();
	io_service.run();
}

/** Accepts new connections */
void ConnectionManager::Accept()
{
    #ifdef LOGGING
    Logger::Write(7, "\t>ConnectionManager accepting..");
    #endif // LOGGING

    char* data = new char[256];
	boost::asio::ip::udp::endpoint connected_endpoint;

	gateway.async_receive_from(boost::asio::buffer(data, 256), connected_endpoint,
		boost::bind(&ConnectionManager::HandleAccept, this, boost::asio::placeholders::error, connected_endpoint, boost::asio::placeholders::bytes_transferred, data));
}

/** Adds and starts a new client, then calls Accepting back */
void ConnectionManager::HandleAccept(const boost::system::error_code& _error, boost::asio::ip::udp::endpoint _endpoint, size_t _received, char* data)
{
     if (!_error && 0 < _received)
     {
		 //Authenticate user!
     }
	 else 
	 {
		#ifdef LOGGING
		 Logger::Write(3, "#Error @ ConnectionManager HandleAccept!");
		 Logger::Write(3, _error.message());
		#endif
	 }
	 
	 delete[] data;

     Accept();
}
