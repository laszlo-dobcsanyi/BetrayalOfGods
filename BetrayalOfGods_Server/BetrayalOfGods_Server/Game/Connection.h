#ifndef GAME_CONNECTION_H
#define GAME_CONNECTION_H

#include <string>
#include <boost\bind.hpp>
#include <boost\asio.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>

class server;
class Message;
class Character;

extern server* Server;

class Connection
	: public boost::enable_shared_from_this<Connection>
{
public:
	enum ServerCommand
	{
		LOGIN_OK = 0,
		REGISTRATE_OK = 1,

		HERO_DATA = 10,
		HERO_WORLD = 11,

		CHARACTER_ENTER = 20,
		CHARACTER_MOVE = 21,

		OBJECT_ADD = 30,
		OBJECT_REMOVE = 39,

		ERROR_LOGIN = 1000,
		ERROR_REGISTRATE = 1001
	};

	enum ClientCommand
	{
		LOGIN = 0,
		REGISTRATE = 1,

		HERO_MOVE = 10,
		HERO_STOP = 11
	};

public:
	std::string name_in_database;
    boost::asio::ip::udp::socket& GetSocket() { return socket; };

    Connection(boost::asio::io_service& _io_service);
	~Connection();

	void Start();	

	void Send(boost::shared_ptr<Message> _msg);
    void Send(const int& _cmd, const std::string& _data);

	void Dispose();

private:
    bool disposed;

	boost::shared_ptr<Character> character;
    boost::asio::ip::udp::socket socket;

    void ReadHeader(boost::shared_ptr<Message> _msg, const boost::system::error_code& _error);
    void ReadBody(boost::shared_ptr<Message> _msg, const boost::system::error_code& _error);
    void HandleMessage(boost::shared_ptr<Message> _msg);

    void HandleSend(const boost::system::error_code& _error);
};

#endif
