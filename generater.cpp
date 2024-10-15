#include "generater.h"
#include "startScreen.h"
#include <random>
#include <vector>
#include <fstream>
#include <iostream>

bool Generater::is_valid(int row, int col, int num)
{
	for (int i = 0; i < side_length; i++)
		if (board[row][i] == num || board[i][col] == num)
			return false;
	int start_row = row - row % 3;
	int start_col = col - col % 3;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (board[i + start_row][j + start_col] == num)
				return false;
	return true;
}

bool Generater::fill_sudoku()
{
	// 找到一个未填充的单元格
	for (int row = 0; row < side_length; row++) {
		for (int col = 0; col < side_length; col++) {
			if (board[row][col] == 0) {
				// 随机打乱数字1-9的顺序
				std::vector<int> nums{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
				std::random_device rd;
				std::mt19937 g(rd());
				shuffle(nums.begin(), nums.end(), g);

				for (int num : nums) {
					// 尝试填充数字
					if (is_valid(row, col, num)) {
						board[row][col] = num;
						// 递归调用，继续填充下一个格子
						if (fill_sudoku()) {
							return true;
						}
						// 回溯
						board[row][col] = 0;
					}
				}
				return false;
			}
		}
	}
	for(int i = 0; i<side_length;i++)
		for (int j = 0; j < side_length; j++)
			solution[i][j] = board[i][j];
	return true;
}

void Generater::remove_elements()
{
	int numToRemove = 0;
	switch (level)
	{
	case EASY_LEVEL:
		numToRemove = EASY_EMPTY;
		break;
	case MEDIUM_LEVEL:
		numToRemove = MEDIUM_EMPTY;
		break;
	case HARD_LEVEL:
		numToRemove = HARD_EMPTY;
		break;
	}
	std::random_device rd;
	std::mt19937 g(rd());

	while (numToRemove > 0) {
		int row = g() % side_length;
		int col = g() % side_length;

		if (board[row][col] != 0) {
			int backup = board[row][col];
			board[row][col] = 0;
			numToRemove--;
		}
	}
}

Generater::Generater(int _level, int _side_length)
{
	level = _level;
	side_length = _side_length;
	board = new int* [side_length];
	solution = new int* [side_length];
	for (int i = 0; i < side_length; i++)
	{
		board[i] = new int[side_length];
		solution[i] = new int[side_length];
	}
	for(int i = 0; i< side_length; i++)
		for (int j = 0; j < side_length; j++)
		{
			board[i][j] = 0;
			solution[i][j] = 0;
		}
}

Generater::~Generater()
{
	for (int i = 0; i < side_length; i++)
	{
		delete[] board[i];
		delete[] solution[i];
	}
	delete[] board;
	delete[] solution;
}

int** Generater::generater_board()
{
	fill_sudoku();
	remove_elements();
	return board;

}

int** Generater::generater_solution()
{
	return solution;
}

void Generater::print_board()
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			std::cout << board[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Generater::print_solution()
{
	std::ofstream out("solution.txt");
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			out << solution[i][j] << " ";
		}
		out << std::endl;
	}
}
