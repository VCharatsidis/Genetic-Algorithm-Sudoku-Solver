#ifndef MATESTRATEGY_H
#define MATESTRATEGY_H

#include <vector>
#include "CrossOver.cpp"

using std::vector;

class MateStrategy
{
public:
	virtual ~MateStrategy() = default;
	virtual void breed(vector<int> fittest_individuals_indexes) = 0;

};

#endif