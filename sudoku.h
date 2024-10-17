#pragma once
#include <vector>
#include <string>

#include "ichecker.h"
#include "checker.h"
#include "generater.h"
#include "quick_generater.h"

class UserRecord {
	int x;
	int y;
	int value;
	bool is_guess;
public:
	UserRecord(int _x, int _y, int _value, bool _is_guess) : x(_x), y(_y), value(_value), is_guess(_is_guess) {}
 	int get_x() { return x; }
	int get_y() { return y; }
	int get_value() { return value; }
	bool get_is_guess() { return is_guess; }
};

class Sudoku
{
private:
	int level;
	int side_length;
	int step;
	int random_id; //唯一标识每一局对局信息
	std::string total_time;
	int** board;	//初始棋盘
	int** solution;
	int** user_board; //用户自己填写的棋盘
	int** merge_board; //用户填写的棋盘和初始棋盘的合并
	std::vector<std::vector<int>> user_guess_board; // 用户猜测的数据
	std::vector<UserRecord> user_record;
	Ichecker* checker;
	//Generater* generater;
	QuickGenerater* generater;

public:
	Sudoku(int _level, int _side_length);
	~Sudoku();

	void set_side_length(int _side_length);
	int get_side_length();

	int get_step();

	void set_total_time(std::string time);
	std::string get_total_time();

	void set_board(int** board);
	const int** get_board();

	void set_solution(int** solution);
	const int** get_solution();

	void set_user_board(int** user_board);
	const int** get_user_board();

	void set_merge_board(int** merge_board);
	const int** get_merge_board();
	
	void set_board_value(int x, int y, int value);
	int get_board_value(int x, int y);

	void set_solution_value(int x, int y, int value);
	int get_solution_value(int x, int y);

	void set_user_board_value(int x, int y, int value);
	int get_user_board_value(int x, int y);

	void set_merge_board_value(int x, int y, int value);
	int get_merge_board_value(int x, int y);
	
	// 获取当前用户空余的格子数
	int get_empty_num();

	std::vector<UserRecord> const& get_user_record();
	
	std::vector<int> const& get_guess_by_index(int index);
	std::vector<std::vector<int>> const& get_guess_board();
	void add_guess(int index, int value);
	void clear_guess(int index);
	int get_guess_count(int index);
	
	Ichecker* get_checker();
	bool check_user_solution();
	
	// 进行一步操作
	bool one_step(int x, int y, int value, bool is_guess);
	// 回退一步操作
	int** step_back();

	void record_user_actions(int x, int y, int value, bool is_guess);
	void record_all_boards();
};