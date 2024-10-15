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
public:
	UserRecord(int _x, int _y, int _value) : x(_x), y(_y), value(_value) {}
	int get_x() { return x; }
	int get_y() { return y; }
	int get_value() { return value; }
};

class Sudoku
{
private:
	int level;
	int side_length;
	int step;
	std::string total_time;
	int** board;	//初始棋盘
	int** solution;
	int** user_board; //用户自己填写的棋盘
	int** merge_board; //用户填写的棋盘和初始棋盘的合并
	std::vector<std::vector<int>> user_guess_board;
	std::vector<UserRecord> user_record;
	Ichecker* checker;
	//Generater* generater;
	QuickGenerater* generater;

public:
	Sudoku(int _level, int _side_length);
	~Sudoku();
	void set_board(int** board);
	void set_solution(int** solution);
	void set_user_board(int** user_board);
	int** get_board();
	int** get_solution();
	int** get_user_board();
	int get_side_length();
	void set_side_length(int _side_length);
	void set_board_value(int x, int y, int value);
	int get_board_value(int x, int y);
	void set_solution_value(int x, int y, int value);
	int get_solution_value(int x, int y);
	void set_user_board_value(int x, int y, int value);
	int get_user_board_value(int x, int y);
	void set_merge_board(int** merge_board);
	int** get_merge_board();
	void set_merge_board_value(int x, int y, int value);
	int get_merge_board_value(int x, int y);
	int get_step();
	void set_total_time(std::string time);
	std::string get_total_time();
	int get_empty_num();
	int** copy_board();
	int** copy_solution();
	int** copy_user_board();
	int** copy_merge_board();
	Ichecker* get_checker();
	bool check_user_solution();
	void push_record(UserRecord record);
	std::vector<UserRecord> const & get_user_record();
	bool one_step(int x, int y, int value);
	int** step_back();
	void add_guess(int index, int value);
	void clear_guess(int index);
	int get_guess_num(int index);
	std::vector<int> const& get_guess(int index);
};