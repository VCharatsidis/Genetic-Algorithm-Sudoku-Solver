#include "Board.cpp"
#include <iostream>
#include <string>
#include "GeneticSolver.cpp"

using std::cout;
using std::endl;

void print(Board b)
{
	for (int i = 0; i < b.size; i++)
	{
		for (int j = 0; j < b.size; j++)
		{
			int val = b.boxes[i][j]->value;

			if (val == 0)
			{
				std::cout << " _ ";
			}
			else
			{
				if (b.boxes[i][j]->fixed) 
				{
					std::cout << " " + std::to_string(val) + "'";
				}
				else 
				{
					std::cout << " " + std::to_string(val) + " ";
				}
				
			}
		}

		std::cout << "" << std::endl;
	}
	std::cout << "" << std::endl;
}

void print(Population b, int individual)
{
	cout << "Individual "+std::to_string(individual) << endl;
	for (int i = 0; i < b.size; i++)
	{
		for (int j = 0; j < b.size; j++)
		{
			int val = b.population[individual][i][j]->value;

			if (val == 0)
			{
				std::cout << " _ ";
			}
			else
			{
				if (b.population[individual][i][j]->fixed)
				{
					std::cout << " " + std::to_string(val) + "'";
				}
				else
				{
					std::cout << " " + std::to_string(val) + " ";
				}
			}
		}

		std::cout << "" << std::endl;
	}
}

int main()
{
	/*Board b = Board();
	print(b);*/

	//for (int i = 0; i < b.size; i++)
	//{
	//	for (int j = 0; j < b.fixed_values[i].size(); j++)
	//	{
	//		cout << std::to_string(b.fixed_values[i][j])+" ";
	//	}
	//	cout << "" << endl;
	//}

	//for (int i = 0; i < b.size; i++)
	//{
	//	for (int j = 0; j < b.available_values[i].size(); j++)
	//	{
	//		cout << std::to_string(b.available_values[i][j]);
	//	}
	//	cout << "" << endl;
	//}

	Population pop = Population(true);

	GeneticSolver solver = GeneticSolver();
	print(solver.pop, 0);
	cout << "fitness " + std::to_string(solver.count_fitness(0, solver.pop)) << endl;
	std::cout << "" << std::endl;
	print(solver.pop, 1);
	cout << "fitness " + std::to_string(solver.count_fitness(1, solver.pop)) << endl;
	std::cout << "" << std::endl;
	//solver.cross_over(0, 2, 99);
	//print(solver.pop, 99);

	solver.make_new_population();

	print(solver.next_gen, 0);
	cout << "new fitness " + std::to_string(solver.count_fitness(0, solver.next_gen)) << endl;
	std::cout << "" << std::endl;

	print(solver.next_gen, 1);
	cout << "new fitness " + std::to_string(solver.count_fitness(1, solver.next_gen)) << endl;
	std::cout << "" << std::endl;

	print(solver.next_gen, 25);
	cout << "children " + std::to_string(solver.count_fitness(25, solver.next_gen)) << endl;
	std::cout << "" << std::endl;

	solver.solve();
	return 0;
}