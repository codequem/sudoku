#include "checker.h"

#include "checker_test.h"
#include "generater.h"
#include "startScreen.h"
#include <iostream>

void check_test()
{
	Generater generater(HARD_LEVEL, 9);
    int** board = generater.generater_board();
	int** solution = generater.generater_solution();
	generater.print_board();
	generater.print_solution();

    Checker checker(9, board, solution, solution);
    checker.check();
    bool** result_board = checker.get_result_board();
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (result_board[i][j] == false)
                std::cout << "Row " << i+1 << " Column " << j+1 << " is incorrect." << std::endl;
        }
    }
}