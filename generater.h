#pragma once

#define EASY_EMPTY 20
#define MEDIUM_EMPTY 40
#define HARD_EMPTY 60

class Generater
{
private:
	int level;
	int side_length;
	int** board;
	int** solution;

	bool is_valid(int row, int col, int num);
	bool fill_sudoku();
	void remove_elements();


public:
	Generater(int _level, int _side_length);
	~Generater();

	int** generater_board();
	int** generater_solution();
	
	void print_board();
	void print_solution();
};