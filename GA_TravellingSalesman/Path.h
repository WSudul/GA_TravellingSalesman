#pragma once
#include <vector>

class Path
{
public:
	Path();
	Path(const Path &P);
	Path(const std::vector<std::size_t>&v);
	virtual ~Path();
	std::vector<std::size_t> pathPoints; //vector storing points 
};

