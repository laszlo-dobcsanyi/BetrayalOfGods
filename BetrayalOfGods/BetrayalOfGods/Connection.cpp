#include "Macro.h"
#include "Connection.h"

#include <boost\bind.hpp>
#include <boost\shared_ptr.hpp>

#include "Message.hpp"
#include "Window.h"

Connection::Connection()
	: io_service(),
	  socket(io_service),
	  disposed(false)
{
}

void Connection::Connect()
{
	std::string server_address = "127.0.0.1";
	std::string server_port = "1425";

	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query((server_address.c_str()), (server_port.c_str()));
	boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

	Connect(iterator);

	thread = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));
}


void Connection::Connect(boost::asio::ip::tcp::resolver::iterator& _iterator)
{
	boost::asio::async_connect(socket, _iterator,
		boost::bind(&Connection::HandleConnection, shared_from_this(), boost::asio::placeholders::error, _iterator));
}


void Connection::HandleConnection(const boost::system::error_code& _error, boost::asio::ip::tcp::resolver::iterator& _iterator)
{
	if (!_error) //  && IsConnected()
	{
		disposed = false;

		boost::shared_ptr<Message> msg(new Message());
        boost::asio::async_read(socket,
            boost::asio::buffer(msg->SetData(), Message::header_length),
            boost::bind(&Connection::ReadHeader, shared_from_this(), msg, boost::asio::placeholders::error));
	}
	else
	{
		Connect(_iterator);
	}
}

/** */
void Connection::ReadHeader(boost::shared_ptr<Message> _msg, const boost::system::error_code& _error)
{
	if (!_error && _msg->DecodeHeader() && IsConnected())
    {
        boost::asio::async_read(socket,
            boost::asio::buffer(_msg->SetBody(), _msg->GetBodyLength()),
            boost::bind(&Connection::ReadBody, shared_from_this(), _msg, boost::asio::placeholders::error));
    }
    else
    {
		std::cout << "# Error @ Connection ReadHeader!\n";
        Dispose();
    }
}

/** */
void Connection::ReadBody(boost::shared_ptr<Message> _msg, const boost::system::error_code& _error)
{
    if (!_error && IsConnected())
    {
        window->Handle_Message(_msg);

        boost::shared_ptr<Message> msg(new Message());
        boost::asio::async_read(socket,
            boost::asio::buffer(msg->SetData(), Message::header_length),
            boost::bind(&Connection::ReadHeader, shared_from_this(), msg, boost::asio::placeholders::error));
    }
    else
    {
		std::cout << "# Error @ Connection ReadBody!\n" << _error.message() << std::endl;
		Dispose();
    }
}

/** */
void Connection::Send(boost::shared_ptr<Message> _msg)
{
    boost::asio::async_write(socket,
        boost::asio::buffer(_msg->GetData(), _msg->GetLength()),
    boost::bind(&Connection::HandleSend, shared_from_this(), boost::asio::placeholders::error));

    _msg.reset();
}

/** */
void Connection::Send(const int& _cmd, const std::string& _data)
{
    boost::shared_ptr<Message> msg(new Message(_cmd, _data));

    boost::asio::async_write(socket,
        boost::asio::buffer(msg->GetData(), msg->GetLength()),
    boost::bind(&Connection::HandleSend, shared_from_this(), boost::asio::placeholders::error));

    msg.reset();

}

/** */
void Connection::HandleSend(const boost::system::error_code& _error)
{
	if (_error)
    {
		std::cout << "# Error @ Connection HandleSend!\n" << _error.message() << std::endl;
		Dispose();
    }
}


void Connection::Dispose()
{		
	if (disposed) return; disposed = true;

	socket.close();
	thread.detach();
}

