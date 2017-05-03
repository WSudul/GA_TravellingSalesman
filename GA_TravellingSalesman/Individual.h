#pragma once

#include <cstdlib>
#include "Matrix.h"




class Individual: public Matrix<int>
{
public:
	Individual();
	Individual::Individual(std::size_t size1, std::size_t size2);
	~Individual();

};

