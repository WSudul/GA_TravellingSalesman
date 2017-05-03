// GA_TravellingSalesman.cpp : Defines the entry point for the console application.
//
#include "Individual.h"
#include "PathFinder.h"
#include <iostream>
#include "stdafx.h"


int main()
{
	int test[8] = { 0,1,2,3,4,5,6,7 };
	Individual map(8, 8);
	for (int i = 0; i < 8; i++)
	{
		test[i % 8] = 0;
		map.fillRow(test, 4, 0);
		test[i % 8] = 3;
	}
	std::cout << "map is ready " << map.getSize1() << "x" << map.getSize2() << std::endl;
	std::vector<std::size_t> v = { 0, 1, 2, 3,4,5,6 };
	PathFinder solution(map,v ,15);
	solution.initRandDevEng();
	std::cout << "finding solution" << std::endl;
	solution.findSolution(10, 100, 10);




    return 0;
}

