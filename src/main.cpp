#include "menu.h"
#include <iostream>

using namespace std;

int main() {
  Menu menu;        // 创建菜单对象
  menu.printMenu(); // 打印菜单信息
  while (!menu.getExit()) {
    menu.handleChoice(); // 处理用户选择
  }

  cout << "感谢您使用遥感图像处理程序，再见！" << endl;

  system("pause");
  return 0;
}