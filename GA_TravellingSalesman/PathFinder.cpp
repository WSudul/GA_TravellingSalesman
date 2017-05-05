#include "PathFinder.h"



PathFinder::~PathFinder()
{
}

PathFinder::PathFinder(Matrix<int> map, std::vector<std::size_t >& places, unsigned int maxGroupSize):
	GeneticAlghortim<Path>(),
	preCalculatedMap(map),
	locsToVisit(places)
{
	setMaxGroupSize(maxGroupSize); //#TODO move it to initializer list , default ctor for base class sets it to 10
	initRandDevEng(); //initialize random devices (generators for algorithm) with random seeds

	if (preCalculatedMap.getSize1() != preCalculatedMap.getSize2())
	{
		throw (std::invalid_argument("Map is not complete:" + preCalculatedMap.getSize1() + preCalculatedMap.getSize2() ) );
	}

	//#TODO should i check for unique places to visit?
	//Assume that you need to visit same place multiple times and cost is 0 or near zero. 
	//If not checking for unique then the comparision for places to visit vs places on map is not needed at all.

	if (locsToVisit.size() > preCalculatedMap.getSize1())
	{
		throw (std::invalid_argument("Too many points to visit: " + locsToVisit.size()));
	}


	for (auto it = locsToVisit.begin(); it != locsToVisit.end(); it++)
	{
		if (*it<0 || *it>preCalculatedMap.getSize1())
		{
			throw (std::invalid_argument("Invalid place number: " + *it));
		}
	}

	initDistribution();

}

Path PathFinder::createIndividual()
{
	Path individual(locsToVisit); //create base individual with locsTovisit;

	//randomly reorder elements of specified individual, mtInt is container (there is only 1 generator, therefore adressing 1st elements only)
	std::shuffle(individual.pathPoints.begin(), individual.pathPoints.end(), mtInt.front());

	return individual;
}

float PathFinder::calcFitness(Path & individual)
{
	double distance = 0;

	//loop through all the points and calculate distance
	for (auto it = individual.pathPoints.begin(); it != std::prev(individual.pathPoints.end(), 1); it++)
		distance += preCalculatedMap.at(*it, *std::next(it,1));

	//get from last point to start
	distance += preCalculatedMap.at(individual.pathPoints.back(), individual.pathPoints.front());

	//#TODO check if distance ==0 ? extreme edge case but need to be considered if duplicate points are allowed
	double fitness = 1 / distance;
	//std::cout << "fitness="<<fitness <<"\t for dist="<<distance<<std::endl;
	return (float)fitness;
}

Path PathFinder::mutateClone(const Path & individual)
{ 
	Path clone(individual);
	
	mutate(clone);

	return clone;
}

void PathFinder::mutate(Path & individual)
{
	//mutate should swap 2 places at random. -> consult MutationRate and then generate a value <0:v.size() ) 
	if(mutationDist(mtMutation)>mutationRate)
	{
	
		std::size_t i, j;
		i = distUniformInt.front()(mtInt.front());
		j = distUniformInt.front()(mtInt.front());

		std::swap(individual.pathPoints.at(i), individual.pathPoints.at(j));
	}
	
}

//Order based crossover -Davis 1985 OX1
Path PathFinder::crossover(const Path & parent1, const Path & parent2)
{
	//no less than 3 objects to swap for partial mapped crossover to work
	if (locsToVisit.size() < 3)
		return parent1;
	
	//generate cut places
	std::size_t l, r;
	do {
		l = distUniformInt.front()(mtInt.front());
		r = distUniformInt.front()(mtInt.front());
	} while (l == r);

	if(l > r)
		std::swap(l, r);

	const Path * prnt1Ptr = &parent1;
	const Path * prnt2Ptr = &parent2;
	
	auto cutStart = std::next(prnt1Ptr->pathPoints.begin(), l);
	auto cutEnd = std::next(prnt1Ptr->pathPoints.begin(), r);
	
	auto cutStart2 = std::next(prnt2Ptr->pathPoints.begin(), l);
	auto cutEnd2 = std::next(prnt2Ptr->pathPoints.begin(), r);

	//std::cout << "parents:" << std::endl;
	//for (int i = 0; i < locsToVisit.size(); i++)
	//	std::cout << parent1.pathPoints.at(i)<<" "<<parent2.pathPoints.at(i)<< std::endl;

	//std::cout << "dist(l,r)=" << std::distance(cutStart,cutEnd ) << std::endl;
	//std::cout << "dist(begin,l)" << std::distance(prnt1Ptr->pathPoints.begin(), cutStart) << std::endl;
	
	
	Path child(std::vector<std::size_t>(cutStart, cutEnd)); //create  child and fill child with part of Parent
	child.pathPoints.reserve(locsToVisit.size());

	
	
	//std::cout << "child pre size=" << child.pathPoints.size() << "\tl=" << l << " r=" << r << std::endl;
	
	
	//following snippet will fail to account for multiple locations of the same type! Only unique locations will be valid

	//loop starting from 2nd cut points to the end
	for (auto it = cutEnd2; it != prnt2Ptr->pathPoints.end(); ++it)
	{
		if (std::none_of(child.pathPoints.begin(), child.pathPoints.end(), [&it](const std::size_t & v)->bool {return *it == v; }))
		{
			child.pathPoints.push_back(*it);
		}
	}
	
	//loop from start to the end (adds missing elements)
	for (auto it = prnt2Ptr->pathPoints.begin() ; it != prnt2Ptr->pathPoints.end(); ++it)
	{
		if (std::none_of(child.pathPoints.begin(), child.pathPoints.end(), [&it](const std::size_t & v)->bool {return *it == v; }))
		{
			child.pathPoints.push_back(*it);
		}
	}

	if (child.pathPoints.size() != locsToVisit.size())
	{
		std::cout << "BAD CHILD SIZE! IS:" << child.pathPoints.size() << " SHOULD BE :" << locsToVisit.size() << std::endl;
		throw std::exception("Crossover failed to make appropriate child");
	}
	
	


	return child;
}

void PathFinder::initRandDevEng()
{


	if (Seed.RandSeedInt.size() != 0 )
	{
		mtInt.clear();
		Seed.RandSeedInt.clear();
	}
	
	mtInt.resize(1);
	Seed.RandSeedInt.resize(1);
	setSeeds();
	seedGenerators();
	
	
}

void PathFinder::initDistribution()
{
	
	distUniformInt.push_back(std::uniform_int_distribution<int>(0, locsToVisit.size()-1));
}

inline void PathFinder::setLocsToVisit(std::vector<std::size_t>& locsToVisit)
{


	this->locsToVisit = locsToVisit;
	//#TODO refractor this (see ctor) ?
	if (locsToVisit.size() > preCalculatedMap.getSize1())
	{
		throw (std::invalid_argument("Too many points to visit: " + locsToVisit.size()));
	}


	for (auto it = locsToVisit.begin(); it != locsToVisit.end(); it++)
	{
		if (*it<0 || *it>preCalculatedMap.getSize1())
		{
			throw (std::invalid_argument("Invalid place number: " + *it));
		}
	}


	initDistribution();
}
