#pragma once
#include "sudoku.h"
#include <chrono>
#include <string>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>       // for Input, Renderer, Vertical
#include <ftxui/component/component_base.hpp>  // for ComponentBase

#define UNKNOW_LEVEL 0
#define EASY_LEVEL 1
#define MEDIUM_LEVEL 2
#define HARD_LEVEL 3

class Start
{
private:
	// 当前按下的按钮的位置
	int btn_row; 
	int btn_col;
	// 复选框的状态
	bool use_maybe;
	bool use_guess;
	// 用户在结算界面选择要去往的界面
	bool choose_welcome_page;
	bool choose_start_new_game;
	// 用户是否是继续进行保存的游戏
	bool continue_game;
public:
	Start();
	void init();
	void welcomePage(); 
	void startGame(int level);
	void chooseLevel();
	void ruleExplan();
	void operExplan();
	void exitGame();
	void checkStatus(Sudoku& sudo);

	/*
	*  数独按钮的样式渲染器
	*  x,y为当前被按下按钮的位置
	*/
	ftxui::ButtonOption SudokuButtonStyle(Sudoku& sudo, int x, int y);

	/*
	* 普通功能按钮的样式渲染器
	*/
	ftxui::ButtonOption FunctionButtonStyle();

	/*
	* 获取用户解题所用的时间
	* pure为true时，返回XXm XXs，false时返回Time：XXm XXs
	*/
	std::string GetElapsedTime(const std::chrono::steady_clock::time_point& start_time, bool pure);

	/*
	* 设置一个seconds秒的定时器，时间结束后执行callback函数
	*/
	void SetTimer(int seconds, std::function<void()> callback);

	/*
	* 返回给定数字的上标数字/下标数字Unicode字符
	*/
	const char* GetUniCodeUnderNum(int value);
	const char* GetUniCodeUpNum(int value);
};