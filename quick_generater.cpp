#include "quick_generater.h"
#include <algorithm>
#include <random>
#include <cstring> // for memset


bool DLXSolver::solveUnique(std::vector<std::vector<int>>& board)
{
    solutions = 0;
    solve(0, 0, board);
    return solutions == 1;
}

void DLXSolver::solve(int row, int col, std::vector<std::vector<int>>& board)
{
    if (row == 9)
    {
        solutions++;
        return;
    }
    if (col == 9)
    {
        solve(row + 1, 0, board);
        return;
    }
    if (board[row][col] != 0)
    {
        solve(row, col + 1, board);
        return;
    }

    for (int num = 1; num <= 9; ++num)
    {
        if (isValid(row, col, num, board))
        {
            board[row][col] = num;
            solve(row, col + 1, board);
            board[row][col] = 0;
        }
    }
}

bool DLXSolver::isValid(int row, int col, int num, const std::vector<std::vector<int>>& board)
{
    for (int i = 0; i < 9; ++i)
    {
        if (board[row][i] == num || board[i][col] == num ||
            board[row / 3 * 3 + i / 3][col / 3 * 3 + i % 3] == num)
        {
            return false;
        }
    }
    return true;
}

bool QuickGenerater::fillBoard(int row, int col)
{
    if (row == 9)
        return true;
    if (col == 9)
        return fillBoard(row + 1, 0);

    std::vector<int> nums{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::shuffle(nums.begin(), nums.end(), std::mt19937{ std::random_device{}() });

    for (int num : nums)
    {
        if (isValid(row, col, num))
        {
            board[row][col] = num;
            if (fillBoard(row, col + 1))
                return true;
            board[row][col] = 0;
        }
    }
    return false;
}

bool QuickGenerater::isValid(int row, int col, int num)
{
    for (int i = 0; i < 9; ++i)
    {
        if (board[row][i] == num || board[i][col] == num ||
            board[row / 3 * 3 + i / 3][col / 3 * 3 + i % 3] == num)
        {
            return false;
        }
    }
    return true;
}

void QuickGenerater::smartRemoveNumbers()
{
    int blanks = level == 1 ? 20 : level == 2 ? 40
        : 60;
    std::mt19937 rng(std::random_device{}());

    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            positions.emplace_back(i, j);

    std::shuffle(positions.begin(), positions.end(), rng); // 打乱所有位置

    int removed = 0;
    for (const auto& [row, col] : positions)
    {
        if (removed >= blanks)
            break;

        int temp = board[row][col];
        board[row][col] = 0;

        // 只有当仍有唯一解时，才算成功移除
        if (solver.solveUnique(board))
        {
            removed++;
        }
        else
        {
            board[row][col] = temp; // 恢复数字
        }
    }
}

void QuickGenerater::hybridRemoveNumbers()
{
    int blanks = level == 1 ? 20 : level == 2 ? 40
        : 60;
    std::mt19937 rng(std::random_device{}());

    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            positions.emplace_back(i, j);

    std::shuffle(positions.begin(), positions.end(), rng);

    int removed = 0;
    int focusZone = rng() % 9; // 随机选择一个 3x3 区块进行优先删除

    // 优先在某个 3x3 区块打孔
    for (const auto& [row, col] : positions)
    {
        if (removed >= blanks)
            break;
        if (row / 3 * 3 + col / 3 != focusZone)
            continue; // 只在特定区块操作

        int temp = board[row][col];
        board[row][col] = 0;
        if (solver.solveUnique(board))
        {
            removed++;
        }
        else
        {
            board[row][col] = temp;
        }
    }

    // 如果未达到空格数量，再在全局打孔
    for (const auto& [row, col] : positions)
    {
        if (removed >= blanks)
            break;
        if (board[row][col] == 0)
            continue; // 跳过已移除的

        int temp = board[row][col];
        board[row][col] = 0;
        if (solver.solveUnique(board))
        {
            removed++;
        }
        else
        {
            board[row][col] = temp;
        }
    }
}

std::vector<std::vector<int>> QuickGenerater::generater_solution()
{
    std::fill(board.begin(), board.end(), std::vector<int>(9, 0));
    fillBoard(0, 0);
    return board;
}

std::vector<std::vector<int>> QuickGenerater::generater_board()
{
    // 生成唯一标识符
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0,3);
    int random_num = dist(gen);

    solution = generater_solution();

    if(random_num&1)
        smartRemoveNumbers();
    else
        hybridRemoveNumbers();
    return board;
}

std::vector<std::vector<int>> QuickGenerater::get_solution()
{
    return solution;
}