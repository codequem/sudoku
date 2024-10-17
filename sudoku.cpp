#include "sudoku.h"
#include <fstream>
#include <filesystem>
#include <random>

Sudoku::Sudoku(int _level, int _side_length, bool has_board) : user_guess_board(_side_length * _side_length)
{
	level = _level;
	side_length = _side_length;
	step = 0;

	// 生成唯一标识符
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(10000000, 99999999);
	random_id = dist(gen);
	
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
	generater = new QuickGenerater(level);
	if (!has_board)
	{
		auto generater_board = generater->generater_board();
		auto generater_solution = generater->get_solution();
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
	}
	else
	{
		for (int i = 0; i < side_length; i++)
		{
			for (int j = 0; j < side_length; j++)
			{
				board[i][j] = 0;
				merge_board[i][j] = 0;
				solution[i][j] = 0;
				user_board[i][j] = 0;
			}
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

int Sudoku::get_side_length()
{
	return side_length;
}

void Sudoku::set_side_length(int _side_length)
{
	side_length = _side_length;
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

void Sudoku::set_board(const int** board)
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->board[i][j] = board[i][j];
		}
	}
}

const int** Sudoku::get_board()
{
	return const_cast<const int**>(board);
}

const int** Sudoku::get_solution()
{
	return const_cast<const int**>(solution);
}

void Sudoku::set_solution(const int** solution)
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->solution[i][j] = solution[i][j];
		}
	}
}

const int** Sudoku::get_user_board()
{
	return const_cast<const int**>(user_board);
}

void Sudoku::set_user_board(const int** user_board)
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->user_board[i][j] = user_board[i][j];
		}
	}
}

void Sudoku::set_merge_board(const int** merge_board)
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->merge_board[i][j] = merge_board[i][j];
		}
	}
}

const int** Sudoku::get_merge_board()
{
	return const_cast<const int**>(merge_board);
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

void Sudoku::set_merge_board_value(int x, int y, int value)
{
	merge_board[x][y] = value;
}

int Sudoku::get_merge_board_value(int x, int y)
{
	return merge_board[x][y];
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

std::vector<UserRecord> const& Sudoku::get_user_record()
{
	return user_record;
}

std::vector<int> const& Sudoku::get_guess_by_index(int index)
{
	return user_guess_board[index];
}

std::vector<std::vector<int>> const& Sudoku::get_guess_board()
{
	return user_guess_board;
}

void Sudoku::add_guess(int index, int value)
{
	user_guess_board[index].push_back(value);
}

void Sudoku::clear_guess(int index)
{
	user_guess_board[index].clear();
}

int Sudoku::get_guess_count(int index)
{
	return user_guess_board[index].size();
}

Ichecker* Sudoku::get_checker()
{
	return checker;
}

bool Sudoku::check_user_solution()
{
	dynamic_cast<Checker*>(checker)->check();
	return dynamic_cast<Checker*>(checker)->check_user_solution();
}

bool Sudoku::one_step(int x, int y, int value, bool is_guess)
{
	/*如果用户的这步操作是有效的，就返回true,否则返回false*/
	if (get_board_value(x, y) != 0)
		return false;
	//如果用户这一步不是猜测，则写入记录中
	if (!is_guess)
	{
		set_user_board_value(x, y, value);
		set_merge_board_value(x, y, value);
		dynamic_cast<Checker*>(checker)->set_user_board(const_cast<const int**>(user_board));
		dynamic_cast<Checker*>(checker)->set_merge_board(const_cast<const int**>(merge_board));
		dynamic_cast<Checker*>(checker)->check();
	}

	user_record.push_back(UserRecord(x, y, value, is_guess));
	step++;
	
	//记录
	record_user_actions(x, y, value, is_guess);
	record_all_boards();
	return true;
}

int** Sudoku::step_back()
{
	if (step > 1)
	{
		auto last_record = user_record[step - 1];
		auto second_last_record = user_record[step - 2];

		// 如果最后一次操作是一个猜测
		if (last_record.get_is_guess())
		{
			//直接给他干碎就行了
			if (user_guess_board.at(last_record.get_x() * 9 + last_record.get_y()).size() >= 1)
				user_guess_board.at(last_record.get_x() * 9 + last_record.get_y()).pop_back();
		}
		else {
			// 倒数两次记录在同一个格子
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
		}
		user_record.pop_back();
		step--;
	}
	else if (step == 1)
	{
		auto record = user_record[0];
		merge_board[record.get_x()][record.get_y()] = 0;
		user_board[record.get_x()][record.get_y()] = merge_board[record.get_x()][record.get_y()];
		user_record.pop_back();
		step--;
	}
	dynamic_cast<Checker*>(checker)->set_user_board(const_cast<const int**>(user_board));
	dynamic_cast<Checker*>(checker)->set_merge_board(const_cast<const int**>(merge_board));
	dynamic_cast<Checker*>(checker)->check();
	record_all_boards();
	return merge_board;
}

void Sudoku::record_user_actions(int x, int y, int value, bool is_guess)
{
	std::string folder_name = "data";
	// 获取当前路径
	std::filesystem::path path = std::filesystem::current_path() / folder_name;
	// 检查文件夹是否存在,如果不存在就创建一个新的文件夹来存放用户的对局数据
	if (!std::filesystem::exists(path))
		std::filesystem::create_directory(path);

	std::ofstream writer;
	std::string file_path = folder_name;
	file_path += ("//steps_" + std::to_string(random_id) + ".log");
	writer.open(file_path, std::ios::out | std::ios::app);
	writer << "step: " << get_step() << "Row: " << x + 1 << " "
		<< "Col: " << y + 1 << " "
		<< "Value: " << value 
		<< "Guess?: " << std::boolalpha << is_guess << std::endl;
	
}

void Sudoku::record_all_boards()
{
	std::string folder_name = "data";
	// 获取当前路径
	std::filesystem::path path = std::filesystem::current_path() / folder_name;
	// 检查文件夹是否存在,如果不存在就创建一个新的文件夹来存放用户的对局数据
	if (!std::filesystem::exists(path))
		std::filesystem::create_directory(path);

	std::ofstream writer;
	std::string file_path = folder_name;
	file_path += ("//boards_" + std::to_string(random_id) + ".log");
	writer.open(file_path);
	writer << "Board:" << std::endl;
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
			writer << board[i][j] << " ";
		writer << std::endl;
	}
	writer << "User Board:" << std::endl;
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
			writer << user_board[i][j] << " ";
		writer << std::endl;
	}
	writer << "Merge Board:" << std::endl;
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
			writer << merge_board[i][j] << " ";
		writer << std::endl;
	}
	writer << "Solution:" << std::endl;
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
			writer << solution[i][j] << " ";
		writer << std::endl;
	}
}

void Sudoku::load_data(const int** _board, const int** _user_board,const int** _solution)
{
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			this->board[i][j] = _board[i][j];
			this->user_board[i][j] = _user_board[i][j];
			this->solution[i][j] = _solution[i][j];
			this->merge_board[i][j] = _board[i][j];
		}
	}
	for (int i = 0; i < side_length; i++)
	{
		for (int j = 0; j < side_length; j++)
		{
			if (user_board[i][j] != 0)
				this->merge_board[i][j] = user_board[i][j];
		}
	}
	dynamic_cast<Checker*>(checker)->set_board(_board);
	dynamic_cast<Checker*>(checker)->set_user_board(_user_board);
	dynamic_cast<Checker*>(checker)->set_merge_board(const_cast<const int**>(merge_board));
	dynamic_cast<Checker*>(checker)->check();
}
