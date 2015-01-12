#ifndef MANAGER_CONNECTIONMANAGER_H
#define MANAGER_CONNECTIONMANAGER_H

#define SERVER_PORT 1425

#include <boost\asio.hpp>
#include <boost\thread.hpp>
#include <boost\shared_ptr.hpp>

class server;
class Connection;

class ConnectionManager
{
public:
    ConnectionManager();

	void Run();

private:
    boost::asio::io_service io_service;
	boost::asio::ip::udp::socket gateway;

	void Accept();
    void HandleAccept(const boost::system::error_code& _error, boost::asio::ip::udp::endpoint _endpoint, size_t _received, char* data);
};

#endif
