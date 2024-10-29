# 基于Ftxui命令行图形库开发的命令行数独游戏
- Ftxui库:https://github.com/ArthurSonzogni/FTXUI
- 该代码需要Ftxui静态库编译运行，静态库下载链接:https://github.com/ArthurSonzogni/FTXUI/releases/download/v5.0.0/ftxui-5.0.0-win64.zip
# 运行界面
- 开始界面

  ![01.png](/images/01.png)
- 可以进行游戏难度的选择

  - 简单，随机挖去20个空格

  - 中等，随机挖去40个空格

  - 困难，随机挖去大约60个空格，游戏的生成需要大约5~10秒

  - 保证生成的数独随机且具有唯一解。

    ![02.png](/images/02.png)

- 游戏界面如下

  ![03.png](/images/03.png)

  - 可以启用Maybe功能，会自动计算该格在规则内可以填入的值。

    ![04.png](/images/04.png)
  - 当用户输入不符合规则的值时，游戏将会给出反馈
  
    ![05.png](/images/05.png)
  
  - 当用户填满所有格子后，将会自动对结果进行判断
  
  - 用户可以随时退出游戏，但游戏进程将会被保存。
  
- 具有游戏规则说明和操作说明

  ![06.png](/images/06.png)

  ![07.png](/images/07.png)
