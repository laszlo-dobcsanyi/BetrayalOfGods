#include "Handler.h"

Handler::Handler(const Window* _parent, const int& _index)
	: parent(_parent),
	  index(_index),
	  disposed(false)
{

}
