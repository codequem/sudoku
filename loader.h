#pragma once
#include <filesystem>
#include <optional>
#include <string>

class Loader
{
private:
	int side_length;
	int** unfinished_board;
	int** unfinished_user_board;
	int** solution;
	bool finished;
public:
	Loader(int side_length);
	~Loader();
	std::optional<std::filesystem::directory_entry> findLatestFileWithPrefix(const std::string& folder, const std::string& prefix);
	bool has_save_file(const std::string& folder, const std::string& prefix);
	void load_boards(const std::string& folder, const std::string& prefix, int side_length);
	const int** get_unfinished_board();
	const int** get_unfinished_user_board();
	const int** get_solution();
	bool has_unfinished_board();
};

