#pragma once
#include "ichecker.h"
#include <vector>

class Checker : public Ichecker
{
private:
	int side_length;
	int** board;
	int** user_board;
	int** merge_board;
	bool** result_board;
	bool result;
	std::vector<int> maybe_list;

public:
	Checker();
	Checker(int _side_length, int** _board, int** _user_board, int** _merge_board);
	~Checker();
	
	void set_board(int** _board);
	void set_user_board(int** _user_board);
	void set_side_length(int _side_length);
	void set_merge_board(int** _merge_board);

	bool check_row();
	bool check_column();
	bool check_block();
	void check();

	const std::vector<int>& maybe(int x, int y);

	bool check_user_solution() override;
	bool** get_result_board() override;
};