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
			int val = b.boxes[i][j].value;

			if (val == 0)
			{
				std::cout << " _ ";
			}
			else
			{
				if (b.boxes[i][j].fixed) 
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
	for (int i = 0; i < b.sudoku_size; i++)
	{
		for (int j = 0; j < b.sudoku_size; j++)
		{
			int val = b.population[individual][i][j].value;

			if (val == 0)
			{
				std::cout << " _ ";
			}
			else
			{
				if (b.population[individual][i][j].fixed)
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

	/*Board board = Board();
	Population pop = Population(true);*/
	//GeneticSolver solver = GeneticSolver();

	/*solver.print_board(pop, 0);
	FitnessCounter fc = FitnessCounter();
	
	int kati = fc.count_fitness(0,pop);
	cout << "fitness " + std::to_string(fc.count_column_errors(0, pop)) << endl;*/

	
	/*solver.make_new_population();
	solver.print(solver.next_gen, 0);
	solver.mutate(0);
	solver.print(solver.next_gen, 0);*/
	vector<double> avg_fitnesses;
	int runs = 1;
	while (true)
	{
		cout << "-------------------------------------------------run : "+std::to_string(runs)+" -----------------------------------------------------" << endl;

		GeneticSolver* solver = new GeneticSolver();
		solver->solve();
		
		if (solver->solved)
		{
			break;
		}
		runs++;
		avg_fitnesses.push_back(solver->best_avg_fitness);

		double avg_fit_per_run = 0;
		for (int i = 0; i < avg_fitnesses.size(); i++)
		{
			avg_fit_per_run += avg_fitnesses[i];
			cout << " run " + std::to_string(i) +" : "+ std::to_string(avg_fitnesses[i]) +" , ";
		}

		cout << " " << endl;
		cout << "average fitness in all runs "+std::to_string(avg_fit_per_run / avg_fitnesses.size()) << endl;

		delete solver;
	}
	
	cout << "solved " << endl;
	getchar();
	return 0;
}