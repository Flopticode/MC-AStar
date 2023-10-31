#include "Path.h"

Path::Path(std::list<BlockPos> data)
	:data(data)
{

}

std::list<BlockPos> Path::get()
{
	return data;
}