#include "Core\Macro.h"
#include "Game\Connection.h"

#include <boost\lexical_cast.hpp>

#include "Core\Server.h"
#include "Core\Message.hpp"
#include "Manager\DatabaseManager.h"
#include "Manager\ConnectionManager.h"
#include "Game\Character.h"
#include "Game\Field.h"

Connection::Connection(boost::asio::io_service& _io_service)
    : socket(_io_service),
	  name_in_database("\0"),
	  disposed(false)
{

}

/** Starts the networking of the client */
void Connection::Start()
{
    boost::shared_ptr<Message> msg(new Message());
    boost::asio::async_read(socket,
        boost::asio::buffer(msg->SetData(), Message::header_length),
        boost::bind(&Connection::ReadHeader, shared_from_this(), msg, boost::asio::placeholders::error));
}

/** */

void Connection::ReadHeader(boost::shared_ptr<Message> _msg, const boost::system::error_code& _error)
{
	if ((!_error) && (_msg->DecodeHeader()) && !disposed)
    {
        boost::asio::async_read(socket,
            boost::asio::buffer(_msg->SetBody(), _msg->GetBodyLength()),
            boost::bind(&Connection::ReadBody, shared_from_this(), _msg, boost::asio::placeholders::error));
    }
    else
    {
		#ifdef LOGGING
		Logger::Write(3, "# Error @ Connection ReadHeader!");
		Logger::Write(3, _error.message());
		#endif
        Dispose();
    }
}

/** */
void Connection::ReadBody(boost::shared_ptr<Message> _msg, const boost::system::error_code& _error)
{
	if (!_error && !disposed)
    {
        HandleMessage(_msg);

        boost::shared_ptr<Message> msg(new Message());
        boost::asio::async_read(socket,
            boost::asio::buffer(msg->SetData(), Message::header_length),
            boost::bind(&Connection::ReadHeader, shared_from_this(), msg, boost::asio::placeholders::error));
    }
    else
    {
		#ifdef LOGGING
		Logger::Write(3, "# Error @ Connection ReadBody!");
		Logger::Write(3, _error.message());
		#endif
		Dispose();
    }
}

/** */
void Connection::Send(boost::shared_ptr<Message> _msg)
{
    boost::asio::async_write(socket,
        boost::asio::buffer(_msg->GetData(), _msg->GetLength()),
    boost::bind(&Connection::HandleSend, this, boost::asio::placeholders::error));
}

/** */
void Connection::Send(const int& _cmd, const std::string& _data)
{
    boost::shared_ptr<Message> msg(new Message(_cmd, _data));

    boost::asio::async_write(socket,
        boost::asio::buffer(msg->GetData(), msg->GetLength()),
    boost::bind(&Connection::HandleSend, this, boost::asio::placeholders::error));
}

/** */
void Connection::HandleSend(const boost::system::error_code& _error)
{
	if (_error && !disposed)
    {
		#ifdef LOGGING
		Logger::Write(0, "# Error @ Connection HandleSend!");
		Logger::Write(0, _error.message());
		#endif
		Dispose();
    }
}


void Connection::Dispose()
{	
    if (disposed) return; disposed = true;
	
	#ifdef LOGGING
	Logger::Write(5, "\t>Disposing Connection..");
	#endif

	if (character != 0)
	{		
		#ifdef LOGGING
		Logger::Write(6, "\t\t>Removing Character from Field..");
		#endif

		character->Dispose();
		character.reset();
	}

	if (name_in_database != "\0")
	{
		#ifdef LOGGING
		Logger::Write(6, "\t\t>Removing Character from Database..");
		#endif

		Server->Manager_Database->Remove(name_in_database);
		name_in_database = "\0";
	}

    socket.close();
}

Connection::~Connection()
{
	#ifdef LOGGING
	Logger::Write(4, "\t>Connection destructor..");
	#endif
}

