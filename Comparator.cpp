#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <vector>

struct Fitness_index_pair {
	int fitness;
	int index;
};

class Comparator {
public:

	bool operator() (const Fitness_index_pair* a , const Fitness_index_pair* b ) const {

		return a->fitness < b->fitness;
	}
};
#endif