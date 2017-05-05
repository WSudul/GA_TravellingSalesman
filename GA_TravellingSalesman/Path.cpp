#include "Path.h"
#include <iostream>


Path::Path()
{
}

Path::Path(const Path & P) :
	pathPoints(P.pathPoints)
{

}

Path::Path(const std::vector<std::size_t>& v) :
	pathPoints(v)
{
	std::cout << "vector ctor, size=" << v.size() << std::endl;
}


Path::~Path()
{
}
