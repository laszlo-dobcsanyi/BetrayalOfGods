#include "Macro.h"
#include "Object.h"

#include <boost\lexical_cast.hpp>

extern LPSTR path;

Object::Object(std::ifstream& _stream)
	: disposed(false)
{
	_stream >> id >> x >> y;

	D3DXCreateTextureFromFile(device, (boost::lexical_cast<std::string>(path) + "\\data\\objects\\" + boost::lexical_cast<std::string>(id) + ".png").c_str(), &texture);
}

void Object::Save(std::ofstream& _stream)
{
	_stream << "\t" << id << " " << x << " " << y;
}

void Object::Dispose()
{
	if (disposed) return; disposed = true;

	texture->Release();
}


