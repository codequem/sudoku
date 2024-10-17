#include "loader.h"
#include <filesystem>
#include <optional>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

Loader::Loader(int _side_length)
{
    unfinished_board = new int*[_side_length];
    unfinished_user_board = new int*[_side_length];
    solution = new int* [_side_length];
    side_length = _side_length;
    for (int i = 0; i < _side_length; i++)
    {
		unfinished_board[i] = new int[_side_length];
		unfinished_user_board[i] = new int[_side_length];
        solution[i] = new int[_side_length];
    }
    finished = false;
}

Loader::~Loader()
{
    for (int i = 0; i < side_length; i++)
    {
		delete[] unfinished_board[i];
        delete[] unfinished_user_board[i];
        delete[] solution[i];
    }
    delete[] unfinished_board;
	delete[] unfinished_user_board;
    delete[] solution;
}

std::optional<std::filesystem::directory_entry> Loader::findLatestFileWithPrefix(const std::string& folder, const std::string& prefix)
{
    std::optional<std::filesystem::directory_entry> latestFile; // 用于存储最新文件的信息
    std::time_t latestTime = 0;  // 初始化为最早时间

    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (entry.is_regular_file() && entry.path().filename().string().rfind(prefix, 0) == 0) {
            std::time_t fileTime = std::filesystem::last_write_time(entry).time_since_epoch().count();
            if (fileTime > latestTime) {
                latestTime = fileTime;
                latestFile = entry;
            }
        }
    }
    return latestFile;
}

bool Loader::has_save_file(const std::string& _folder, const std::string& _prefix)
{
    //std::string folder = "./data";
	//std::string prefix = "boards";

	auto latestFile = findLatestFileWithPrefix(_folder, _prefix);
	if (latestFile.has_value()) 
		return true;
    else
        return false;
}

std::vector<int> parseLineToNumbers(const std::string& line) {
    std::vector<int> numbers;
    std::istringstream stream(line);  // 将行内容转换为流
    int num;

    // 使用流逐个读取整数，遇到空格自动跳过
    while (stream >> num) {
        numbers.push_back(num);
    }

    return numbers;
}

void Loader::load_boards(const std::string& _folder, const std::string& _prefix, int side_length)
{
    //std::string folder = "./data";
    //std::string prefix = "boards";
    if (!has_save_file(_folder, _prefix))
        return;

    auto latestFile = findLatestFileWithPrefix(_folder, _prefix);

    std::ifstream file(latestFile->path());
    if (!file.is_open())
        return;
    bool matchFound_board = false;
    bool matchFound_user_board = false;
    bool matchFound_solution = false;
    int lineCountAfterMatch = 0;
    int empty_count = side_length * side_length;
    std::string keyword_board = "Board:";
    std::string keyword_user_board = "User Board:";
    std::string keyword_solution = "Solution";

	std::string line;
    // 一行一行读取文件内容
    while (std::getline(file, line)) {
		if (lineCountAfterMatch >= side_length)
			break;
        if (matchFound_board) {  // 如果已经找到匹配行，开始计数
			auto nums = parseLineToNumbers(line);
            for (int i = 0; i < side_length && i < nums.size(); i++)
            {
                unfinished_board[lineCountAfterMatch][i] = nums[i];
                if (nums[i] != 0)
                    empty_count--;
            }
            ++lineCountAfterMatch;
        }
        if (!matchFound_board && line.find(keyword_board) != std::string::npos) {
            matchFound_board = true;  // 找到匹配关键词的行
        }
    }
    file.close();
	file.open(latestFile->path());

    lineCountAfterMatch = 0;

    while (std::getline(file, line)) {
        if (lineCountAfterMatch >= side_length)
            break;
        if (matchFound_user_board) {  // 如果已经找到匹配行，开始计数
            auto nums = parseLineToNumbers(line);
            for (int i = 0; i < side_length && i < nums.size(); i++)
            {
                unfinished_user_board[lineCountAfterMatch][i] = nums[i];
                if (nums[i] != 0)
                    empty_count--;
            }
            ++lineCountAfterMatch;
        }
        if (!matchFound_user_board && line.find(keyword_user_board) != std::string::npos) {
            matchFound_user_board = true;  // 找到匹配关键词的行
        }
    }
	file.close();
    file.open(latestFile->path());

    lineCountAfterMatch = 0;
    while (std::getline(file, line)) {
        if (lineCountAfterMatch >= side_length)
            break;
        if (matchFound_solution) {  // 如果已经找到匹配行，开始计数
            auto nums = parseLineToNumbers(line);
            for (int i = 0; i < side_length && i < nums.size(); i++)
                solution[lineCountAfterMatch][i] = nums[i];
            ++lineCountAfterMatch;
        }
        if (!matchFound_solution && line.find(keyword_solution) != std::string::npos) {
            matchFound_solution = true;  // 找到匹配关键词的行
        }
    }
    if(empty_count == 0)
		finished = true;
}

const int** Loader::get_unfinished_board()
{
    return const_cast<const int**>(unfinished_board);
}

const int** Loader::get_unfinished_user_board()
{
	return const_cast<const int**>(unfinished_user_board);
}

const int** Loader::get_solution()
{
    return const_cast<const int**>(solution);
}

bool Loader::has_unfinished_board()
{
    return !finished;
}
