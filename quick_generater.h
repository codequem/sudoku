#include <vector>

class DLXSolver
{
public:
    bool solveUnique(std::vector<std::vector<int>>& board);
private:
    int solutions;

    void solve(int row, int col, std::vector<std::vector<int>>& board);

    bool isValid(int row, int col, int num, const std::vector<std::vector<int>>& board);
};

class QuickGenerater
{
private:
    int level;
    std::vector<std::vector<int>> board;
    std::vector<std::vector<int>> solution;
    DLXSolver solver;

    bool fillBoard(int row, int col);
    bool isValid(int row, int col, int num);
    void smartRemoveNumbers();
    void hybridRemoveNumbers();

public:
    QuickGenerater(int level) : level(level), board(9, std::vector<int>(9, 0)) {}

    std::vector<std::vector<int>> generater_solution();

    std::vector<std::vector<int>> generater_board();

    std::vector<std::vector<int>> get_solution();
};