#include "checker.h"
#include <iostream>
#include <fstream>

Checker::Checker()
{
	side_length = 0;
	board = nullptr;
	user_board = nullptr;
	merge_board = nullptr;
	result_board = nullptr;
	result = false;
}

Checker::Checker(int _side_length, int** _board, int** _user_board, int ** _merge_board)
{
	side_length = _side_length;
	board = _board;
	user_board = _user_board;
	merge_board = _merge_board;
	result_board = new bool* [side_length];
	for (int i = 0; i < side_length; i++)
	{
		result_board[i] = new bool[side_length];
	}
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			result_board[i][j] = true;
		}
	}
	result = false;
}

Checker::~Checker()
{
	for (int i = 0; i < side_length; i++)
	{
		delete[] result_board[i];
	}
	delete[] result_board;
}

void Checker::set_board(const int** _board)
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->board[i][j] = _board[i][j];
		}
	}
}

void Checker::set_user_board(const int** _user_board)
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->user_board[i][j] = _user_board[i][j];
		}
	}
}

void Checker::set_side_length(int _side_length)
{
	side_length = _side_length;
}

void Checker::set_merge_board(const int** _merge_board)
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->merge_board[i][j] = _merge_board[i][j];
		}
	}
}

bool Checker::check_row()
{
	for (int i = 0; i < side_length; i++)
	{
		bool used[9] = { false };
		for (int j = 0; j < side_length; j++)
		{
			//如果棋盘此处不为空
			if (merge_board[i][j] != 0)
			{
				//并且此处已经使用过1-9中的数字
				if (used[merge_board[i][j] - 1])
				{
					int the_num = merge_board[i][j];
					for (int k = 0; k < side_length; k++){
						//如果此处的数字(用户填写的)和其他地方的数字相同，给它标记为false
						if(user_board[i][k] == the_num)
							result_board[i][k] = false;
					}
					result = false;
				}
				else
				{
					//标记此数字已经使用过
					used[merge_board[i][j] - 1] = true;
				}
			}
		}
	}
	return result;
}

bool Checker::check_column()
{
	for (int j = 0; j < side_length; j++)
	{
		bool used[9] = { false };
		for (int i = 0; i < side_length; i++)
		{
			//如果棋盘此处不为空
			if (merge_board[i][j] != 0)
			{
				//并且此处已经使用过1-9中的数字
				if (used[merge_board[i][j] - 1])
				{
					int the_num = merge_board[i][j];
					for (int k = 0; k < side_length; k++) {
						//如果此处的数字(用户填写的)和其他地方的数字相同，给它标记为false
						if (user_board[k][j] == the_num)
							result_board[k][j] = false;
					}
					result = false;
				}
				else
				{
					//标记此数字已经使用过
					used[merge_board[i][j] - 1] = true;
				}
			}
		}
	}
	return result;
}

bool Checker::check_block()
{
	for (int i = 0; i < side_length; i += 3)
	{
		for (int j = 0; j < side_length; j += 3)
		{
			bool used[9] = { false };
			for (int m = i; m < i + 3; m++)
			{
				for (int n = j; n < j + 3; n++)
				{
					//如果棋盘此处不为空
					if (merge_board[m][n] != 0)
					{
						//并且此处已经使用过1-9中的数字
						if (used[merge_board[m][n] - 1])
						{
							int the_num = merge_board[m][n];
							for (int k = i; k < i + 3; k++) {
								for (int l = j; l < j + 3; l++) {
									//如果此处的数字(用户填写的)和其他地方的数字相同，给它标记为false
									if (user_board[k][l] == the_num)
										result_board[k][l] = false;
								}
							}
							result = false;
						}
						else
						{
							//标记此数字已经使用过
							used[merge_board[m][n] - 1] = true;
						}
					}
				}
			}
		}
	}
	return result;
}

void Checker::check()
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			result_board[i][j] = true;
		}
	}
	result = true;
	check_row();
	check_column();
	check_block();
}

const std::vector<int>& Checker::maybe(int x, int y)
{
	maybe_list.clear();
	//返回一个格子可能的值
	bool used[9] = { false };
	for (int i = 0; i < side_length; i++)
	{
		if (merge_board[x][i] != 0)
		{
			used[merge_board[x][i] - 1] = true;
		}
		if (merge_board[i][y] != 0)
		{
			used[merge_board[i][y] - 1] = true;
		}
	}
	int start_x = x / 3 * 3;
	int start_y = y / 3 * 3;
	for (int i = start_x; i < start_x + 3; i++)
	{
		for (int j = start_y; j < start_y + 3; j++)
		{
			if (merge_board[i][j] != 0)
			{
				used[merge_board[i][j] - 1] = true;
			}
		}
	}
	for (int i = 0; i < side_length; i++)
	{
		//如果有0-9中的数字没有使用过，就加入到可能的值中
		if (!used[i])
		{
			maybe_list.push_back(i + 1);
		}
	}
	return maybe_list;
}

bool Checker::check_user_solution()
{
	return result;
}

bool** Checker::get_result_board()
{
	return result_board;
}
