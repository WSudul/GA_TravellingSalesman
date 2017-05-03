#pragma once
#include "Selection.h"
#include "Individual.h"
#include "Path.h"
#include "Matrix.h"
#include <exception>

class PathFinder :
	public GeneticAlghortim<Path>
{
public:
	//PathFinder();
	~PathFinder();
	PathFinder(Matrix<int> map, std::vector<std::size_t >& places, unsigned int maxGroupSize);


	Path createIndividual();
	float calcFitness(Path &individual);
	Path mutateClone(const Path &individual);
	void mutate(Path &individual);
	Path crossover(const Path &parent1, const Path &parent2);
	void initRandDevEng();


	void setLocsToVisit(std::vector<std::size_t>& locsToVisit);


	const Matrix<int> preCalculatedMap;
private:
	/*!
	initialize distribution range for Random Device Engine.
	assign to vector a sigle distribution that will be used throughout the execution of algorithm.
	Called at the end of ctor and after new locations were delivered via setLocsToVisit();
	*/
	void initDistribution();

	std::vector<std::size_t> locsToVisit; //locations to visit


};