#include "Path.h"

Path::Path(std::list<Node*> data)
	:data(data)
{

}

std::list<Node*> Path::get()
{
	return data;
}

void Path::truncate(uint32 length)
{
	if (length >= data.size())
		return;
	data.resize(length);
}

Node* Path::tail()
{
	return data.back();
}
