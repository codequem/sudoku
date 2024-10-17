#include "start.h"
#include "sudoku.h"

#include <chrono>
#include <string>
#include <fstream>
#include <filesystem>
#include <ftxui/component/captured_mouse.hpp>  // for ftxui
#include <ftxui/component/component.hpp>       // for Input, Renderer, Vertical
#include <ftxui/component/component_base.hpp>  // for ComponentBase
#include <ftxui/component/screen_interactive.hpp>  // for Component, ScreenInteractive
#include <ftxui/dom/elements.hpp>  // for operator|, Element, size, border, frame, vscroll_indicator, HEIGHT, LESS_THAN
#include <ftxui/screen/screen.hpp>

using namespace ftxui;

ButtonOption Start::FunctionButtonStyle() {
	auto option = ButtonOption::Animated(Color::RGB(199, 171, 238));
	option.transform = [](const EntryState& s) {
		auto element = text(s.label) | color(Color::White);
		auto background = s.focused ? bgcolor(Color::RGB(5, 84, 26)) : bgcolor(Color::RGB(86, 6, 77));
		if (s.focused)
			element |= bold;
		return element | center | borderEmpty | flex;
		};
	return option;
}

ButtonOption Start::SudokuButtonStyle(Sudoku& sudo, int x, int y) {
	auto option = ButtonOption::Animated();
	option.transform = [&sudo, x, y, this](const EntryState& s) {
		auto element = text(s.label) | flex ;
		bool changeable = (sudo.get_board_value(x, y) == 0); // 按钮显示的是否是棋盘原有的数字
		bool wrong = !sudo.get_checker()->get_result_board()[x][y]; // 用户填入的数字是否是不合规定的
		if (!changeable) 
			element |= color(Color::White); //棋盘上原有的数字使用白色
		else if (wrong)
			element |= color(Color::RGB(255, 0, 0)); //错误的数字使用红色标出
		else
			element |= color(Color::RGB(187, 255, 0)); //目前正确的数字使用绿色标出
			//element |= color(Color::RGB(169, 219, 231));
		if (s.focused)
			element |= bold;

		auto bordercolor = color(Color::White);// 每一宫的边框颜色
		switch ((y / 3) * 3 + (x / 3))
		{
		case 1:
			bordercolor = color(Color::RGB(222, 162, 162));
			break;
		case 2:
			bordercolor = color(Color::RGB(196, 221, 161));
			break;
		case 3:
			bordercolor = color(Color::RGB(161, 221, 220));
			break;
		case 4:
			bordercolor = color(Color::RGB(161, 189, 221));
			break;
		case 5:
			bordercolor = color(Color::RGB(175, 161, 221));
			break;
		case 6:
			bordercolor = color(Color::RGB(207, 161, 221));
			break;
		case 7:
			bordercolor = color(Color::RGB(155, 166, 11));
			break;
		case 8:
			bordercolor = color(Color::RGB(221, 161, 184));
			break;
		case 0:
			bordercolor = color(Color::RGB(221, 210, 161));
			break;
		}

		//将选中的数字有关的格子标注出来
		bool use_bgcolor = false;
		if (btn_col == y || btn_row == x) use_bgcolor = true; //和被选中的格子同行或者同列
		if (((btn_col / 3) * 3 + (btn_row / 3)) == ((y / 3) * 3 + (x / 3))) use_bgcolor = true; //和选中的格子同宫
		if (sudo.get_merge_board_value(btn_row, btn_col) != 0)
			if (sudo.get_merge_board_value(x,y) == sudo.get_merge_board_value(btn_row, btn_col)) use_bgcolor = true; //和选中的格子值相同的
		if (btn_col == y && btn_row == x) use_bgcolor = false;//被选中的格子使用框架默认的动画

		return element | center | border | bordercolor | flex | ((use_bgcolor) ? bgcolor(Color::RGB(62, 75, 74)) : nothing);
		};
	return option;
}

std::string Start::GetElapsedTime(const std::chrono::steady_clock::time_point& start_time, bool pure) {
	auto now = std::chrono::steady_clock::now();
	auto duration = duration_cast<std::chrono::seconds>(now - start_time);
	long long minutes = duration.count() / 60;
	long long seconds = duration.count() % 60;
	if (pure)
		return std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
	return "Time: " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
}

void Start::SetTimer(int seconds, std::function<void()> callback) {
	std::thread([seconds, callback]() {
		std::this_thread::sleep_for(std::chrono::seconds(seconds));
		callback();  // 时间到后调用回调函数
		}).detach();  // 使用 detach 分离线程，避免阻塞主线程
}

const char* Start::GetUniCodeUnderNum(int value) {
	switch (value) {
	case 0: return reinterpret_cast<const char*>(u8"\u2080");
	case 1: return reinterpret_cast<const char*>(u8"\u2081");
	case 2: return reinterpret_cast<const char*>(u8"\u2082");
	case 3: return reinterpret_cast<const char*>(u8"\u2083");
	case 4: return reinterpret_cast<const char*>(u8"\u2084");
	case 5: return reinterpret_cast<const char*>(u8"\u2085");
	case 6: return reinterpret_cast<const char*>(u8"\u2086");
	case 7: return reinterpret_cast<const char*>(u8"\u2087");
	case 8: return reinterpret_cast<const char*>(u8"\u2088");
	case 9: return reinterpret_cast<const char*>(u8"\u2089");
	default: return "";  // 如果数字超出范围，返回空字符串
	}
}

const char* Start::GetUniCodeUpNum(int value) {
	switch (value) {
	case 0: return reinterpret_cast<const char*>(u8"\u2070");
	case 1: return reinterpret_cast<const char*>(u8"\u00B9");
	case 2: return reinterpret_cast<const char*>(u8"\u00B2");
	case 3: return reinterpret_cast<const char*>(u8"\u00B3");
	case 4: return reinterpret_cast<const char*>(u8"\u2074");
	case 5: return reinterpret_cast<const char*>(u8"\u2075");
	case 6: return reinterpret_cast<const char*>(u8"\u2076");
	case 7: return reinterpret_cast<const char*>(u8"\u2077");
	case 8: return reinterpret_cast<const char*>(u8"\u2078");
	case 9: return reinterpret_cast<const char*>(u8"\u2079");
	default: return "";  // 如果数字超出范围，返回空字符串
	}
}

class ScreenManager {
public:
	// 单例screen
	static ScreenInteractive& getScreen(int depth) {
		//welcomePage() 处于的深度
		static auto depth_1 = ScreenInteractive::FitComponent();
		//ruleExplan(), operExplan() 处于的深度
		static auto depth_2 = ScreenInteractive::FitComponent();
		//chooseLevel() 处于的深度
		static auto depth_3 = ScreenInteractive::FitComponent();
		//startGame() 处于的深度
		static auto depth_4 = ScreenInteractive::FitComponent();
		//checkStatus() 处于的深度
		static auto depth_5 = ScreenInteractive::FitComponent();

		switch (depth)
		{
		case 1:
			return depth_1;
		case 2:
			return depth_2;
		case 3:
			return depth_3;
		case 4:
			return depth_4;
		case 5:
			return depth_5;
		}
	}
};

Start::Start()
{
	btn_col = 0;
	btn_row = 0;
	use_maybe = false;
	use_guess = false;
	choose_welcome_page = false;
	choose_start_new_game = false;
	init();
}

void Start::init()
{
	welcomePage();
}

void Start::welcomePage()
{
	ScreenManager screenManager;
	
	auto btn_start_game = Button("进入数独游戏", [&] {startGame(UNKNOW_LEVEL);  }, FunctionButtonStyle());
	auto btn_rule_explan = Button("游戏规则说明", [&] {ruleExplan();  }, FunctionButtonStyle());
	auto btn_oper_explan = Button("游戏操作说明", [&] {operExplan();  }, FunctionButtonStyle());
	auto btn_exit_game = Button("退出游戏", [&] { exitGame(); }, FunctionButtonStyle());
	auto buttons = Container::Vertical({
		Container::Horizontal({btn_start_game}) | flex,
		Container::Horizontal({btn_rule_explan}) | flex,
		Container::Horizontal({btn_oper_explan}) | flex,
		Container::Horizontal({btn_exit_game}) | flex,
		});
	auto main_component = Renderer(buttons, [&] {
		return vbox({
			text("欢迎进入数独游戏") | bold | hcenter | color(Color::Yellow),
			text("                ") | hcenter,
			text("                ") | hcenter,
			text("                ") | hcenter,
			text("                ") | hcenter,
			buttons->Render() | frame | size(WIDTH, GREATER_THAN, 30) | vscroll_indicator | size(HEIGHT, LESS_THAN, 50),
			separator(),
			text("请选择一个选项开始游戏") | hcenter | color(Color::RGB(184, 233, 224)),  // 增加提示文本
			}) | flex | center | size(WIDTH, GREATER_THAN, 250) | size(HEIGHT, GREATER_THAN, 200) | border;
		});
	screenManager.getScreen(1).Loop(main_component);
	return;
}


void Start::startGame(int level)
{
	if (level == UNKNOW_LEVEL)
		chooseLevel();
	else
	{
		std::string strings[81]; //按钮中的数字数组
		bool input_mode = false; //记录当前是输入模式还是普通的选中模式

		ScreenManager screenManager;
		Sudoku sudoku(level, 9); 

		// 开始时间
		auto start_time = std::chrono::steady_clock::now();
		// 计时器组件
		auto timer = Renderer([&] {
			return text(GetElapsedTime(start_time, false)) | center;
			}) | size(WIDTH, GREATER_THAN, 20);
		// 提示组件
		std::string prompt_text;
		auto prompt = Renderer([&] {
			return text(prompt_text) | center;
			});
		std::string empty_num;
		auto empty = Renderer([&] {
			return text(empty_num) | center;
			});
		// 开关组件
		auto checkbox_maybe = Checkbox("启用Maybe", &use_maybe);
		auto checkbox_guess = Checkbox(" 填入猜测", &use_guess);
		auto checkbox_layout = Container::Vertical({
			checkbox_maybe | center,
			checkbox_guess | center,
			}) |flex ;

		// 数独按钮组容器(网格排列)，管理交互
		auto sudoku_buttons = Container::Vertical({});
		for (int row = 0; row < sudoku.get_side_length(); ++row) {
			auto row_container = Container::Horizontal({});
			for (int col = 0; col < sudoku.get_side_length(); ++col) {
				sudoku.get_board()[row][col] == 0 ? strings[row * 9 + col] = " " : strings[row * 9 + col] = std::to_string(sudoku.get_board()[row][col]); // 如果棋盘中存储的为0，则显示空，否则显示存储的数字
				auto button = Button(&strings[row * 9 + col], [&, col, row] {
					// 按钮被点击后的处理函数
					input_mode = true;
					btn_row = row;
					btn_col = col;

					// 如果用户点击的是一个可修改的格子
					if (sudoku.get_board_value(row, col) == 0)
					{
						dynamic_cast<Checker*>(sudoku.get_checker())->check(); // 对全局棋盘进行扫描，以便算出正确的maybe结果
						auto maybe_values = dynamic_cast<Checker*>(sudoku.get_checker())->maybe(row, col);

						// 如果用户不启用Maybe功能，则不做任何提示
						if (use_maybe == false)
							prompt_text = "";
						else{
							prompt_text = "Maybe: ";
							for (int i = 0; i < maybe_values.size(); i++)
							{
								prompt_text += std::to_string(maybe_values[i]);
								if (i != maybe_values.size() - 1)
									prompt_text += ",";
							}
						}

						// 提示用户当前剩余的空格数
						empty_num = "Empty: ";
						empty_num += std::to_string(sudoku.get_empty_num());
					}
					else
						prompt_text = "";
				}, SudokuButtonStyle(sudoku, row, col));
				row_container->Add(button | size(WIDTH, EQUAL, 5) | size(HEIGHT, EQUAL, 5) | flex );
			}
			sudoku_buttons->Add(row_container);
		}

		// 数独按钮渲染器，管理布局
		auto sudoku_grid = Renderer(sudoku_buttons, [&] {
			Elements rows;
			for (int row = 0; row < 9; ++row) {
				Elements cols;
				for (int col = 0; col < 9; ++col) {
					cols.push_back(sudoku_buttons->ChildAt(row)->ChildAt(col)->Render() | flex);
				}
				rows.push_back(hbox(std::move(cols)) | flex);
			}
			return vbox(std::move(rows)) | border;
		}) | size(WIDTH, GREATER_THAN, 40) | size(HEIGHT, GREATER_THAN, 40);

		sudoku_grid = CatchEvent(sudoku_grid, [&](Event event) {
			if (!input_mode) return false;
			if (event.is_character()) {
				std::string input_char = event.input();
				// 检查输入是否为有效数字
				if (sudoku.get_board_value(btn_row, btn_col) != 0)
					input_mode = false;
				else
				{
					int index = btn_row * 9 + btn_col;
					// 用户填入的是确切的数字
					if (!use_guess)
					{
						// 将之前用户猜测的数字擦除
						sudoku.clear_guess(index);
						int value = 0;
						if (input_char >= "1" && input_char <= "9") {
							strings[index] = input_char;  // 更新字符串为用户输入的值
							value = std::stoi(input_char);
						}
						else if (input_char == "0" || input_char == " ") {
							strings[index] = " ";  // 允许用户清空输入
						}
						// sudoku将一步操作记录到数据中
						sudoku.one_step(btn_row, btn_col, value, false);
					}
					// 用户填入的是一个猜测
					else
					{
						// 将之前用户写入的数据清除
						int value = 0;
						//sudoku.one_step(btn_row, btn_col, value, false);
						if (input_char >= "1" && input_char <= "9") {
							value = std::stoi(input_char);
						if (sudoku.get_guess_count(index) >= 3)
							sudoku.clear_guess(index);
						sudoku.add_guess(index, value);
						std::string temp_string = "";
						auto guess_board = sudoku.get_guess_by_index(index);
						for (int i = 0; i < sudoku.get_guess_count(index); i++)
						{
							//上下标数字交替使用，避免拥挤
							if (i & 1)
								temp_string += GetUniCodeUnderNum(guess_board[i]);
							else
								temp_string += GetUniCodeUpNum(guess_board[i]);
						}
						strings[index] = temp_string;
						sudoku.one_step(btn_row, btn_col, value, true);
					}
					else if (input_char == "0" || input_char == " ") {
						sudoku.clear_guess(index);
						strings[index] = "";
					}
				}
				// 输入完成，切换输入模式为false
				input_mode = false;
			}
				// 如果用户填入的数字个数等于81个，且数独解法正确，则游戏胜利
				if (sudoku.get_empty_num() == 0)
				{
					// 结束计时
					sudoku.set_total_time(GetElapsedTime(start_time, true));
					checkStatus(sudoku);
				}
				return true;
			}
			return false;
		});

		// 右侧菜单栏的按钮组
		auto undo_button = Button("撤销", [&] {
			auto record = sudoku.get_user_record();
			if (record.size() > 0 && sudoku.get_step() > 1) {
				auto last_record = record.at(sudoku.get_step() - 1); // 获取最后一条记录
				auto second_last_record = record.at(sudoku.get_step() - 2); //获取倒数第二条记录
				btn_row = last_record.get_x();
				btn_col = last_record.get_y();
				int index = btn_row * 9 + btn_col;
				//如果撤销的是一个猜测
				if (last_record.get_is_guess())
				{
					std::string temp_string = "";
					auto guess_board = sudoku.get_guess_by_index(index);
					// 少给用户打印一个,诶嘿
					for (int i = 0; i < sudoku.get_guess_count(index) - 1; i++)
					{
						//上下标数字交替使用，避免拥挤
						if (i & 1)
							temp_string += GetUniCodeUnderNum(guess_board[i]);
						else
							temp_string += GetUniCodeUpNum(guess_board[i]);
					}
					strings[index] = temp_string;
				}
				else
				{
					//如果撤销的是同一个格子
					if (last_record.get_x() == second_last_record.get_x() && last_record.get_y() == second_last_record.get_y())
						strings[index] = std::to_string(second_last_record.get_value());
					else
						strings[index] = " ";
				}
			}
			else if (record.size() > 0) {
				auto first_record = record.at(0);
				strings[first_record.get_x() * 9 + first_record.get_y()] = " ";
			}
			//统一到这里销毁一步操作
			sudoku.step_back();
			//更新checker状态
			dynamic_cast<Checker*>(sudoku.get_checker())->check();
			}) | color(Color::RGB(169, 219, 231));
		auto check_button = Button("检查结果", [&] {
			sudoku.set_total_time(GetElapsedTime(start_time, true));
			checkStatus(sudoku);
		}) | color(Color::RGB(199, 171, 238));
		auto show_answer_button = Button("查看答案", [&] {
			auto solution = sudoku.get_solution();
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					// 将答案展示给用户
					strings[i * 9 + j] = std::to_string(solution[i][j]);
				}
			}
			//两秒后恢复原状
			SetTimer(2, [&] {

				for (int i = 0; i < 9; i++)
				{
					for (int j = 0; j < 9; j++)
					{
						int index = i * 9 + j;
						if (sudoku.get_merge_board()[i][j] == 0)
							strings[index] = " ";
						else
							strings[index] = std::to_string(sudoku.get_merge_board()[i][j]);
						if (sudoku.get_guess_count(index) > 0)
						{
							std::string temp_string = "";
							auto guess_board = sudoku.get_guess_by_index(index);
							// 少给用户打印一个,诶嘿
							for (int i = 0; i < sudoku.get_guess_count(index); i++)
							{
								//上下标数字交替使用，避免拥挤
								if (i & 1)
									temp_string += GetUniCodeUnderNum(guess_board[i]);
								else
									temp_string += GetUniCodeUpNum(guess_board[i]);
							}
							strings[index] = temp_string;
						}
					}
				}
				});
		}) | color(Color::RGB(222, 162, 162));
		auto new_game_button = Button("再来一局", [&] { 
			screenManager.getScreen(4).ExitLoopClosure()();
		}) | color(Color::RGB(196, 221, 161));
		auto back_button = Button("返回", [&] {
			// 返回上层循环
			screenManager.getScreen(4).ExitLoopClosure()();
		}) | color(Color::RGB(155, 166, 11));

		auto menu = Container::Vertical({
			checkbox_layout,
			undo_button,
			check_button,
			show_answer_button,
			new_game_button,
			back_button
		});

		// 右侧菜单栏的布局
		auto right_menu = Renderer(menu, [&] {
			return vbox({
				timer->Render() | border,
				prompt->Render() | border,
				empty->Render() | border,
				checkbox_layout->Render(),
 				filler(),
				undo_button->Render(),
				check_button->Render(),
				show_answer_button->Render(),
				new_game_button->Render(),
				back_button->Render(),
				}) | border | flex;
		});

		// 主布局：左边是数独表格，右边是菜单栏
		auto layout = Container::Horizontal({
			sudoku_grid | size(HEIGHT, EQUAL, 29) ,     // 数独表格，占据大约2倍空间
			right_menu | flex_shrink,    // 菜单栏，占据1倍空间
		}) | center | size(WIDTH, GREATER_THAN, 300) | size(HEIGHT, GREATER_THAN, 200);

		auto game_component = Renderer(layout, [&] {
			// 如果用户在结算界面选择不回到这里，就结束本层循环
			if (choose_start_new_game || choose_welcome_page)
			{
				screenManager.getScreen(4).ExitLoopClosure()();
				choose_start_new_game = false;
			}
			return layout->Render()| flex;
		});
		screenManager.getScreen(4).Loop(game_component);
	}
}

void Start::chooseLevel()
{
	ScreenManager screenManager;

	auto btn_easy = Button("简单", [&] {
		startGame(EASY_LEVEL); 
	}, FunctionButtonStyle());
	auto btn_medium = Button("中等", [&] {
		startGame(MEDIUM_LEVEL); 
	}, FunctionButtonStyle());
	auto btn_hard = Button("困难", [&] {
		startGame(HARD_LEVEL); 
	}, FunctionButtonStyle());
	auto btn_return = Button("返回", [&] { 
		screenManager.getScreen(3).ExitLoopClosure()(); 
		}, FunctionButtonStyle());
	int row = 0;
	auto buttons = Container::Vertical({
		Container::Horizontal({btn_easy},&row) | flex,
		Container::Horizontal({btn_medium},&row) | flex,
		Container::Horizontal({btn_hard},&row) | flex,
		Container::Horizontal({btn_return},&row) | flex,
		});
	auto level_component = Renderer(buttons, [&] {
		// 如果用户在结算界面选择不回到这里，就结束本层循环
		if (choose_welcome_page)
		{
			screenManager.getScreen(3).ExitLoopClosure()();
			choose_welcome_page = false;
		}
		return vbox({
			text("请选择游戏难度") | bold | hcenter | color(Color::Yellow),
			text("                ") | hcenter,
			text("                ") | hcenter,
			text("                ") | hcenter,
			text("                ") | hcenter,
			buttons->Render() | frame | size(WIDTH, GREATER_THAN, 30) | vscroll_indicator | size(HEIGHT, LESS_THAN, 50),
			separator(),
			text("请选择一个难度开始游戏") | hcenter | color(Color::RGB(184, 233, 224)),  // 增加提示文本
			}) | flex | center | size(WIDTH, GREATER_THAN, 250) | size(HEIGHT, GREATER_THAN, 200) | border;
		});

	screenManager.getScreen(3).Loop(level_component);
}

void Start::ruleExplan()
{
	ScreenManager screenManager;

	auto rule_explan = Renderer([&] {
		return vbox({
			text("数独游戏规则说明") | bold | hcenter | color(Color::Yellow),
			text("                ") | hcenter,
			text("数独游戏在9x9的大方格内进行，其中特殊的：分为3x3的小方格，被称为区。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("数独游戏首先从已经填入数字的格子开始。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("用1至9来填满空格，并且一个格子只能填入一个数字。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("每个数字在每一行只能出现一次。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("每个数字在每一列只能出现一次。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("每个数字在每一区只能出现一次。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("总结这些规则，即每个数字在每一行，每一列和每一区只能出现一次。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("数独游戏的难度分为简单、中等、困难三种，玩家可以根据自己的实际情况选择不同的难度。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("数独游戏的规则简单易懂，但是解题过程中需要一定的逻辑推理能力。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("祝您游戏愉快！") | hcenter | color(Color::RGB(214, 140, 241)),
			separator(),
			text("右键点击返回主菜单...") | hcenter | color(Color::RGB(255, 0, 0)) | bold | blink,
			}) | flex | center | size(WIDTH, GREATER_THAN, 250) | size(HEIGHT, GREATER_THAN, 200) | border | bgcolor(Color::RGB(89, 90, 227));
		});

	rule_explan = CatchEvent(rule_explan, [&](Event event) {
		//捕获鼠标右键点击动作，如果鼠标右键点击则跳转回主界面
		if (event.is_mouse()) {
			if (event.mouse().button == Mouse::Right && event.mouse().motion == Mouse::Pressed) {
				screenManager.getScreen(2).ExitLoopClosure()();//退出当前循环，回到上一层循环，即主界面
				return true;
			}
		}
		return false;
		});
	screenManager.getScreen(2).Loop(rule_explan);
}

void Start::operExplan()
{
	ScreenManager screenManager;

	auto open_explan = Renderer([&] {
		return vbox({
			text("数独游戏操作说明") | bold | hcenter | color(Color::Yellow),
			text("                ") | hcenter,
			text("进入游戏后，点击当前空白的格子，键入你要填写进这个格子的数字即可。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("也可以点击已经填写过的格子，键入你要填写的修改值。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("如果你点击一个格子并键入空格键，可以清除这个格子中你原来填写的值") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("如果你填写的数字不符合数独游戏的规则，数字将会变红。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("你也可以启用猜测功能，当这个数可能出现在多个格子中时，你可以在这些格子中都填入它，来防止自己遗忘。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("但要注意，一个格子中最多可以填入三个猜测的值。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("游戏还为新手提供了Maybe功能，它将会自动展示出将这个格子可能填入的值。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("当然，如果您希望得到纯净的数独体验，也可以将它关闭。") | hcenter | color(Color::RGB(184, 233, 224)),
			text("                ") | hcenter,
			text("祝您游戏愉快！") | hcenter | color(Color::RGB(214, 140, 241)),
			separator(),
			text("右键点击返回主菜单...") | hcenter | color(Color::RGB(255, 0, 0)) | bold | blink,
			}) | flex | center | size(WIDTH, GREATER_THAN, 250) | size(HEIGHT, GREATER_THAN, 200) | border | bgcolor(Color::RGB(89, 90, 227));
		});

	open_explan = CatchEvent(open_explan, [&](Event event) {
		//捕获鼠标右键点击动作，如果鼠标右键点击则跳转回主界面
		if (event.is_mouse()) {
			if (event.mouse().button == Mouse::Right && event.mouse().motion == Mouse::Pressed) {
				screenManager.getScreen(2).ExitLoopClosure()();//退出当前循环，回到上一层循环，即主界面
				return true;
			}
		}
		return false;
		});

	screenManager.getScreen(2).Loop(open_explan);
}

void Start::exitGame()
{
	ScreenManager screenManager;
	//screenManager.getScreen().ExitLoopClosure()返回一个退出循环的函数，这里我们直接执行了这个函数，退出循环，关闭程序
	//退出最上层循环，退出整个程序
	screenManager.getScreen(1).ExitLoopClosure()();
}

void Start::checkStatus(Sudoku& sudo)
{
	ScreenManager screenManager;
	if (sudo.check_user_solution() && sudo.get_empty_num() == 0)
	{
		//弹出对话框，提示用户游戏胜利
		std::string time_text = "总用时:" + sudo.get_total_time();

		auto text_component = Renderer([&] {
			return vbox({
				text("恭喜！") | bold | center | flex | color(Color::RGB(184, 233, 224)),
				text("你答对了！") | bold | center | flex | color(Color::RGB(184, 233, 224)),
				text(time_text) | bold | center | flex | color(Color::RGB(184, 233, 224)),
				});
			}) | center | flex | size(WIDTH, GREATER_THAN, 50) | size(HEIGHT, GREATER_THAN, 10);

		auto button_main_menu = Button("返回主菜单", [&] {
			// 先退出本层循环，再让上两层循环级联退出
			screenManager.getScreen(5).ExitLoopClosure()();
			choose_welcome_page = true;
			}) | color(Color::RGB(196, 221, 161));
		// 先退出本层循环，再让上一层循环级联退出
		auto button_retry = Button("再来一局", [&] {
			screenManager.getScreen(5).ExitLoopClosure()();
			choose_start_new_game = true;
			}) | color(Color::RGB(196, 221, 161));
		auto buttons = Renderer(Container::Horizontal({
			button_main_menu,
			button_retry,
			}), [&] {
				return hbox({
					button_main_menu->Render() | center | flex,
					text("   "),  // 用于分隔两个按钮的空格
					button_retry->Render() | center | flex,
					}) | center | flex;
			}) | center | flex;

		auto layout = Container::Vertical({
			text_component | center | flex,
			buttons | center | flex,
			}) | center | flex;

		auto result_component = Renderer(layout, [&] {
			return vbox({
				text_component->Render() | border | center | flex,
				filler(),
				buttons->Render() | center | flex,
				}) | center | flex | size(WIDTH, GREATER_THAN, 250) | size(HEIGHT, GREATER_THAN, 200) | border | color(Color::RGB(185, 246, 152)) | bgcolor(Color::RGB(89, 90, 227));
			});

		screenManager.getScreen(5).Loop(result_component);
	}
	else
	{
		auto text_component = Renderer([&] {
			return vbox({
				text("很遗憾！") | bold | center | flex | color(Color::RGB(255, 183, 178)),
				text("您的答案不完全正确。") | bold | center | flex | color(Color::RGB(255, 183, 178)),
				text("是否要再看看？") | bold | center | flex | color(Color::RGB(255, 183, 178)),
				});
			}) | center | flex | size(WIDTH, GREATER_THAN, 50) | size(HEIGHT, GREATER_THAN, 10);

		auto button_main_menu = Button("返回主菜单", [&] {
			// 先退出本层循环，再让上两层循环级联退出
			screenManager.getScreen(5).ExitLoopClosure()();
			choose_welcome_page = true;
			}) | color(Color::RGB(196, 221, 161));

		auto button_retry = Button("再看看", [&] {
			// 退出本层循环，允许用户继续进行他的对局
			screenManager.getScreen(5).ExitLoopClosure()();
			}) | color(Color::RGB(196, 221, 161));

		auto buttons = Renderer(Container::Horizontal({
			button_main_menu,
			button_retry,
			}), [&] {
				return hbox({
					button_main_menu->Render() | center | flex,
					text("   "),  // 按钮之间的间隔
					button_retry->Render() | center | flex,
					}) | center | flex;
			}) | center | flex;

		auto layout = Container::Vertical({
			text_component | center | flex,
			buttons | center | flex,
			}) | center | flex;

		auto result_component = Renderer(layout, [&] {
			return vbox({
				text_component->Render() | border | center | flex,
				filler(),  // 在文本和按钮之间填充空白
				buttons->Render() | center | flex,
				}) | center | flex | size(WIDTH, GREATER_THAN, 250) | size(HEIGHT, GREATER_THAN, 200) | border | color(Color::RGB(185, 246, 152)) | bgcolor(Color::RGB(89, 90, 227));
			});

		screenManager.getScreen(5).Loop(result_component);
	}
}