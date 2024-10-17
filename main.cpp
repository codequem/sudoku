#include "start.h"
#include "loader.h"

int main() {
	Start start;
	/*Loader loader(9);
	if (loader.has_unfinished_board("./data", "boards")) {
		loader.load_boards("./data", "boards", 9);
		const int** unfinished_board = loader.get_unfinished_board();
		const int** unfinished_user_board = loader.get_unfinished_user_board();
		const int** solution = loader.get_solution();
		std::cout << "Board:" << std::endl;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				std::cout << unfinished_board[i][j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << "User Board : " << std::endl;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				std::cout << unfinished_user_board[i][j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << "Solution:" << std::endl;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				std::cout << solution[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}*/
    return 0;
}