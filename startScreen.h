#pragma once
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "sudoku.h"

#define UNKNOW_LEVEL 0
#define EASY_LEVEL 1
#define MEDIUM_LEVEL 2
#define HARD_LEVEL 3

class startScreen
{
private:
	int btn_row;
	int btn_col;
	bool use_maybe;
	bool use_guess;
public:
	startScreen();
	void drawScreen();
	void startGame(int level);
	void chooseLevel();
	void ruleExplan();
	void operExplan();
	void exitGame();
	void check(Sudoku& sudo);

	ftxui::ButtonOption SudokuButtonStyle(Sudoku& sudo, int x, int y);
};