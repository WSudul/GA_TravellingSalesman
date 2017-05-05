// GA_TravellingSalesman.cpp : Defines the entry point for the console application.
//
#include "Individual.h"
#include "PathFinder.h"
#include <iostream>
#include <random>
#include "stdafx.h"


int main()
{
	int test[100][100];
	std::uniform_int_distribution<int> distMap(1, 45);
	std::mt19937 generator;

	for (std::size_t i = 0; i < 100; ++i)
	{
		for (std::size_t j = 0; j < 100; ++j)
		{
			test[j][i] = distMap(generator);
			if (j == i)
				test[j][i] = 0;
		}
	};

	int test2[10][10]= { 
	{ 47,32,70,14,95,79,46,4,13,29 },
	{ 32,54,57,13,100,28,80,25,25,87 },
	{ 37,67,64,51,28,13,77,21,63,31 },
	{ 1,62,87,23,41,79,93,58,85,82 },
	{ 68,95,68,90,34,49,16,3,70,92 },
	{ 21,71,57,4,39,83,97,60,45,97 },
	{ 64,17,68,4,5,67,46,81,89,82 },
	{ 69,42,32,99,50,90,17,13,22,68 },
	{ 19,77,67,73,37,99,66,16,20,50 },
	{ 40,14,57,31,8,77,56,30,37,19 } };
	Individual map(100, 100);
	
	for (int i = 0; i < 100; i++)
		map.fillRow(test[i], 100, i);
	

	std::cout << "map is ready " << map.getSize1() << "x" << map.getSize2() << std::endl;
	std::vector<std::size_t> v;
	v.resize(80);
	std::size_t i = 0;
	for (auto it = v.begin(); it != v.end(); ++it)
		*it = i++;
	std::cout << "locations to visit\n";
	for (auto it = v.begin(); it != v.end(); ++it)
		std::cout << *it << " ";

	std::cout << "creating solution obj\n";
	PathFinder solution(map,v ,100);
	

	std::cout << "finding solution" << std::endl;
	Path bestPath=solution.findSolution(10, 100, 10);
	
	std::cout << "Best path\n";
	for (auto it = bestPath.pathPoints.begin(); it != bestPath.pathPoints.end(); ++it)
		std::cout << *it << " ";



    return 0;
}

