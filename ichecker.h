#pragma once
class Ichecker
{
public:
	virtual bool check_user_solution() = 0;
	virtual bool** get_result_board() = 0;
	virtual ~Ichecker() {}
};