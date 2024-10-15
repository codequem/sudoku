#include "sudoku.h"

Sudoku::Sudoku(int _level, int _side_length) : user_guess_board(_side_length * _side_length)
{
	level = _level;
	side_length = _side_length;
	step = 0;
	generater = new QuickGenerater(level);
	auto generater_board = generater->generater_board();
	auto generater_solution = generater->get_solution();
	
	board = new int* [side_length];
	solution = new int* [side_length];
	user_board = new int* [side_length];
	merge_board = new int* [side_length];
	for (int i = 0; i < side_length; i++)
	{
		board[i] = new int[side_length];
		solution[i] = new int[side_length];
		user_board[i] = new int[side_length];
		merge_board[i] = new int[side_length];
	}
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			board[i][j] = generater_board[i][j];
			merge_board[i][j] = generater_board[i][j];
			solution[i][j] = generater_solution[i][j];
			user_board[i][j] = 0;
		}
	}
	checker = new Checker(side_length, board, user_board, merge_board);
}

Sudoku::~Sudoku()
{
	for (int i = 0; i < side_length; i++)
	{
		delete[] board[i];
		delete[] solution[i];
		delete[] user_board[i];
		delete[] merge_board[i];
	}
	delete[] board;
	delete[] solution;
	delete[] user_board;
	delete[] merge_board;
	delete generater;
	delete checker;
}

void Sudoku::set_board(int** board)
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->board[i][j] = board[i][j];
		}
	}
}

void Sudoku::set_solution(int** solution)
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->solution[i][j] = solution[i][j];
		}
	}
}

void Sudoku::set_user_board(int** user_board)
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->user_board[i][j] = user_board[i][j];
		}
	}
}

int** Sudoku::get_board()
{
	return copy_board();
}

int** Sudoku::get_solution()
{
	return copy_solution();
}

int** Sudoku::get_user_board()
{
	return copy_user_board();
}

int Sudoku::get_side_length()
{
	return side_length;
}

void Sudoku::set_side_length(int _side_length)
{
	side_length = _side_length;
}

void Sudoku::set_board_value(int x, int y, int value)
{
	board[x][y] = value;
}

int Sudoku::get_board_value(int x, int y)
{
	return board[x][y];
}

void Sudoku::set_solution_value(int x, int y, int value)
{
	solution[x][y] = value;
}

int Sudoku::get_solution_value(int x, int y)
{
	return solution[x][y];
}

void Sudoku::set_user_board_value(int x, int y, int value)
{
	user_board[x][y] = value;
}

int Sudoku::get_user_board_value(int x, int y)
{
	return user_board[x][y];
}

void Sudoku::set_merge_board(int** merge_board)
{
	for(int i = 0; i<side_length;i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->merge_board[i][j] = merge_board[i][j];
		}

	}
}

int** Sudoku::get_merge_board()
{
	return copy_merge_board();
}

void Sudoku::set_merge_board_value(int x, int y, int value)
{
	merge_board[x][y] = value;
}

int Sudoku::get_merge_board_value(int x, int y)
{
	return merge_board[x][y];
}

int Sudoku::get_step()
{
	return step;
}

void Sudoku::set_total_time(std::string time)
{
	total_time = time;
}

std::string Sudoku::get_total_time()
{
	return total_time;
}

int Sudoku::get_empty_num()
{
	int result = 0;
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			if (merge_board[i][j] == 0)
				result++;
		}
	}
	return result;
}

int** Sudoku::copy_board()
{
	int** new_board = new int* [side_length];
	for (int i = 0; i < side_length; i++)
	{
		new_board[i] = new int[side_length];
	}
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			new_board[i][j] = board[i][j];
		}
	}
	return new_board;
}

int** Sudoku::copy_solution()
{
	int** new_solution = new int* [side_length];
	for (int i = 0; i < side_length; i++)
	{
		new_solution[i] = new int[side_length];
	}
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			new_solution[i][j] = solution[i][j];
		}
	}
	return new_solution;
}

int** Sudoku::copy_user_board()
{
	int** new_user_board = new int* [side_length];
	for (int i = 0; i < side_length; i++)
	{
		new_user_board[i] = new int[side_length];
	}
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			new_user_board[i][j] = user_board[i][j];
		}
	}
	return new_user_board;
}

int** Sudoku::copy_merge_board()
{
	int** new_merge_board = new int* [side_length];
	for (int i = 0; i < side_length; i++)
	{
		new_merge_board[i] = new int[side_length];
	}
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			new_merge_board[i][j] = merge_board[i][j];
		}
	}
	return new_merge_board;
}

bool Sudoku::check_user_solution()
{
	dynamic_cast<Checker*>(checker)->check();
	return dynamic_cast<Checker*>(checker)->check_user_solution();
}

void Sudoku::push_record(UserRecord record)
{
	user_record.push_back(record);
}

std::vector<UserRecord> const & Sudoku::get_user_record()
{
	return user_record;
}

bool Sudoku::one_step(int x, int y, int value)
{
	/*如果用户的这步操作是有效的，就返回true,否则返回false*/
	if (get_board_value(x, y) != 0)
		return false;
	set_user_board_value(x, y, value);
	set_merge_board_value(x, y, value);
	dynamic_cast<Checker*>(checker)->set_user_board(user_board);
	dynamic_cast<Checker*>(checker)->set_merge_board(merge_board);
	dynamic_cast<Checker*>(checker)->check();
	push_record(UserRecord(x, y, value));
	step++;
	return true;
}

int** Sudoku::step_back()
{
	if (step > 1)
	{
		auto last_record = user_record[step - 1];
		auto second_last_record = user_record[step - 2];
		if (last_record.get_x() == second_last_record.get_x() && last_record.get_y() == second_last_record.get_y())
		{
			merge_board[last_record.get_x()][last_record.get_y()] = second_last_record.get_value();
			user_board[last_record.get_x()][last_record.get_y()] = merge_board[last_record.get_x()][last_record.get_y()];
		}
		else
		{
			merge_board[last_record.get_x()][last_record.get_y()] = 0;
			user_board[last_record.get_x()][last_record.get_y()] = merge_board[last_record.get_x()][last_record.get_y()];
		}
		user_record.pop_back();
		step--;
	}
	else if(step == 1)
	{
		auto record = user_record[0];
		merge_board[record.get_x()][record.get_y()] = 0;
		user_board[record.get_x()][record.get_y()] = merge_board[record.get_x()][record.get_y()];
		user_record.pop_back();
		step--;
	}
	dynamic_cast<Checker*>(checker)->set_user_board(user_board);
	dynamic_cast<Checker*>(checker)->set_merge_board(merge_board);
	dynamic_cast<Checker*>(checker)->check();
	dynamic_cast<Checker*>(checker)->print_all_board();
	return merge_board;
}

void Sudoku::add_guess(int index, int value)
{
	user_guess_board[index].push_back(value);
}

void Sudoku::clear_guess(int index)
{
	user_guess_board[index].clear();
}

int Sudoku::get_guess_num(int index)
{
	return user_guess_board[index].size();
}

std::vector<int> const& Sudoku::get_guess(int index)
{
	return user_guess_board[index];
}


Ichecker* Sudoku::get_checker()
{
	return checker;
}
