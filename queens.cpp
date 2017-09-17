#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<algorithm>
using namespace std;
#define N 15

struct solution {
	int errors_number;
	int* array;
};


struct population {
	solution* solutions;
	int size;
};

int hits(int* board, int n);

void delete_population(population* population1)
{
	int i;
	for (i = 0; i < population1->size; i++)
		free(population1->solutions[i].array);
	free(population1->solutions);
	free(population1);
}

solution* mutation(solution* solution1)
{
	int i, j;
	i = rand() % N;
	j = rand() % N;
	swap(solution1->array[i], solution1->array[j]);
	i = rand() % N;
	j = rand() % N;
	swap(solution1->array[i], solution1->array[j]);
	return solution1;
}

population* sort_solutions(population* population1)
{
	int i, j, swaps_counter;
	solution tmp;
	for (i = 0; i < population1->size; i++)
		population1->solutions[i].errors_number = hits(population1->solutions[i].array, N);

	while (1)
	{
		swaps_counter = 0;
		for (j = 0; j < population1->size - 1; j++)
			if (population1->solutions[j].errors_number > population1->solutions[j + 1].errors_number)
			{
				tmp = population1->solutions[j];
				population1->solutions[j] = population1->solutions[j + 1];
				population1->solutions[j + 1] = tmp;
				swaps_counter++;
			}
		if (swaps_counter == 0)
			break;
	}

	return population1;
}

solution generate_solution(int n)
{
	solution new_solution;
	new_solution.array = (int*)malloc(n * sizeof(int));
	int i;
	for (i = 0; i < n; i++)
		new_solution.array[i] = i;

	for (i = 0; i < n; i++)
		swap(new_solution.array[i], new_solution.array[rand() % n]);

	new_solution.errors_number = hits(new_solution.array, N);
	return new_solution;
}

population* create_population(int size)
{
	population* x = (population*)malloc(sizeof(population));
	x->size = size;
	int i;
	x->solutions = (solution*)malloc(size * sizeof(solution));
	for (i = 0; i < size; i++)
		x->solutions[i] = generate_solution(N);
	x = sort_solutions(x);

	return x;
}

int hits(int* board, int n)
{
	int x, y, i, j, hit = 0;

	for (x = 0; x < n; x++)
	{
		y = board[x];

		//lower right

		i = x + 1;
		j = y + 1;
		while ((i < n && j < n) && (i >= 0 && j >= 0))
		{
			if (board[i] == j)
			{
				hit++;
				break;
			}
			else {
				i++;
				j++;
			}
		}

		//lower left

		i = x + 1;
		j = y - 1;
		while ((i < n && j < n) && (i >= 0 && j >= 0))
		{
			if (board[i] == j)
			{
				hit++;
				break;
			}
			else {
				i++;
				j--;
			}
		}

		//top right

		i = x - 1;
		j = y + 1;
		while ((i < n && j < n) && (i >= 0 && j >= 0))
		{
			if (board[i] == j)
			{
				hit++;
				break;
			}
			else {
				i--;
				j++;
			}
		}

		//top left

		i = x - 1;
		j = y - 1;
		while ((i < n && j < n) && (i >= 0 && j >= 0))
		{
			if (board[i] == j)
			{
				hit++;
				break;
			}
			else {
				i--;
				j--;
			}
		}
	}


	return hit;
}

solution crossing(solution parent_1, solution parent_2)
{
	solution new_solution = generate_solution(N);
	int k;
	int i = 0, j = 0, index, val;

	while (i < N && j < N)
	{
		if (parent_1.array[i] == parent_2.array[j])
		{
			index = i;
			val = parent_1.array[i];

			for (k = 0; k < N; k++)
			{
				if (new_solution.array[k] == val)
					swap(new_solution.array[k], new_solution.array[index]);
			}
		}
		i++;
		j++;
	}

	return new_solution;
}

population* new_generation(population* x)
{
	int index = x->size - x->size / 3;
	int i, j;
	int n = 0;
	for (i = index; i < x->size; i++)
		free(x->solutions[i].array);
	for (i = index; i < x->size; i++)
	{
		x->solutions[i] = crossing(x->solutions[rand() % index], x->solutions[rand() % index]);
	}

	if ((x->solutions[0].errors_number == 2) && (x->solutions[(int)(x->size*0.5)].errors_number == 2) && (rand() % 10 == 0))
	{
		for (j = 0; j < N; j++)
			if (x->solutions[0].array[j] == x->solutions[(int)(x->size*0.5)].array[j])
				n++;
		if (n == N)
		{
			index = x->size / 10;
			for (i = index; i < x->size; i++)
			{
				free(x->solutions[i].array);
				x->solutions[i] = generate_solution(N);
			}
		}
	}

	for (i = x->size / 10; i < x->size; i++)
		if (rand() % 10 == 0)
			mutation(&(x->solutions[i]));

	x = sort_solutions(x);

	return x;
}

void print_solutions(population* A, int n)
{
	printf("\n");
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < N; j++)
			printf("%d ", A->solutions[i].array[j]);
		printf(" - %d", hits(A->solutions[i].array, N));
		printf("\n");
	}
	printf("\n");

}

void print_solution(solution A)
{
	int i;
	printf("\n");
	for (i = 0; i < N; i++)
		printf("%d ", A.array[i]);
	printf("\n");
}

void migration(population* population_1, population* population_2)
{
	int i, j, k;
	solution tmp;
	for (i = 0; i < population_1->size / 2; i++)
	{
		k = rand() % population_1->size;
		j = rand() % population_1->size;
		tmp = population_1->solutions[k];
		population_1->solutions[k] = population_2->solutions[j];
		population_2->solutions[j] = tmp;
	}
}

int main()
{
	srand(time(0));
	int solutions_number;
	scanf("%d", &solutions_number);
	population* A;
	population* B;
	int n = 0, k = 0;
	int unsolved = 1;
	int unsolved1 = 1;
		A = create_population(solutions_number);
		B = create_population(solutions_number);
		while (unsolved && unsolved1)
		{
			A = new_generation(A);
			B = new_generation(B);
			k++;
			if (k == 500)
			{
				migration(A, B);
				k = 0;
			}
			n++;
			unsolved = A->solutions[0].errors_number;
			unsolved1 = B->solutions[0].errors_number;
			printf("%d:  %d %d %d \t %d:  %d %d %d\n", n, unsolved, A->solutions[A->size / 2].errors_number,
				A->solutions[A->size - 1].errors_number, n, unsolved1, B->solutions[A->size / 2].errors_number,
				B->solutions[B->size - 1].errors_number);

		}


	if (unsolved == 0)
		print_solution(A->solutions[0]);
	else
		print_solution(B->solutions[0]);

	delete_population(A);
	delete_population(B);


	return 0;
}