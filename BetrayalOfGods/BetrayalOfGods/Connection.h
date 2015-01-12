#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost\asio.hpp>
#include <boost\thread.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>

class Window;
class Message;

extern Window* window;

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
	Connection();

	void Connect();
	void Connect(boost::asio::ip::tcp::resolver::iterator& _iterator);

	bool IsConnected() { return socket.is_open(); };

	void Send(boost::shared_ptr<Message> _msg);
	void Send(const int& _cmd, const std::string& _data);

	void Dispose();

private:
	bool disposed;
	boost::thread thread;
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket socket;

	void HandleConnection(const boost::system::error_code& _error, boost::asio::ip::tcp::resolver::iterator& _iterator);

	void ReadHeader(boost::shared_ptr<Message> _msg, const boost::system::error_code& _error);
	void ReadBody(boost::shared_ptr<Message> _msg, const boost::system::error_code& _error);
	void HandleMessage(boost::shared_ptr<Message> _msg);

	void HandleSend(const boost::system::error_code& _error);
};

#endif