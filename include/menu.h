// 菜单功能头文件

#ifndef MENU_H
#define MENU_H

#include "image.h"

// 菜单类
class Menu {
 private:
  Image *image;
  bool exit;  // 退出标志

 public:
  Menu();               // 构造函数
  ~Menu();              // 析构函数
  void printMenu();     // 打印菜单信息
  void handleChoice();  // 处理用户选择
  bool getExit();       // 退出程序
};

#endif