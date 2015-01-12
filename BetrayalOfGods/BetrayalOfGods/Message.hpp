#ifndef CORE_MESSAGE_HPP
#define CORE_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class Message
{
public:
  enum { header_length = 4 };
  enum { command_length = 4 };
  enum { max_body_length = 512 };

	Message()
		: bodyLength(0) {}

	Message(const int& _command, const std::string& _message)
		: bodyLength(0) {EncodeBody(_command, _message);}

	const char* GetData() const {return data;}

	char* SetData() {return data;}

	const int& GetCommand() {return command;}

	const std::string& GetMsg() {return message;}

	size_t GetLength() const {return header_length + bodyLength;}

	const char* GetBody() const {return data + header_length;}

	char* SetBody() {return data + header_length;}

	size_t GetBodyLength() const {return bodyLength;}

	void SetBodyLength(size_t new_length)
	{
		bodyLength = new_length;
		if (bodyLength > max_body_length)
			bodyLength = max_body_length;
	}

	void DecodeBody()
	{
		using namespace std; //For strncat and atoi.
		char cmd[command_length + 1] = "";
		strncat_s(cmd, GetBody(), command_length);
		command = atoi(cmd);
			
		message = "";
		for(size_t i = header_length + command_length; i < GetLength(); i++) message += data[i];
	}

	void EncodeBody(const int& _command, const std::string& _msg)
	{
		using namespace std; //For sprintf and memcpy.
		char cmd[command_length + 1] = "";
		sprintf_s(cmd, "%4d", _command);
		memcpy(SetBody(), cmd, command_length);
		const char* tmp = _msg.c_str();
		memcpy(SetBody() + command_length, tmp, strlen(tmp));
		SetBodyLength(strlen(tmp) + command_length);

		EncodeHeader();
	}

	bool DecodeHeader()
	{
		using namespace std; //For strncat and atoi.
		char header[header_length + 1] = "";
		strncat_s(header, data, header_length);
		bodyLength = atoi(header);
		if (bodyLength > max_body_length)
		{
			bodyLength = 0;
			return false;
		}

		return true;
	}

	void EncodeHeader()
	{
		using namespace std; //For sprintf and memcpy.
		char header[header_length + 1] = "";
		sprintf_s(header, "%4d", bodyLength);
		memcpy(data, header, header_length);
	}

private:
	int command;
	std::string message;

	char data[header_length + max_body_length];
	size_t bodyLength;
};

#endif