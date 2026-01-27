#include "menu.h"

// 构造函数
Menu::Menu() {
  image = NULL; // 初始化图像对象指针为空
  exit = false; // 初始化退出标志为false
}

// 析构函数
Menu::~Menu() {
  if (image != NULL) {
    // 如果图像对象指针不为空，释放内存空间
    delete image;
    image = NULL;
  }
}

// 打印菜单信息
void Menu::printMenu() {
  cout << "############ Remote Sensing Image Process Tools ############" << endl;
  cout << "# X – Exit\t\t退出程序" << endl;
  cout << "# O – Open\t\t打开影像文件" << endl;
  cout << "# C – Close\t\t关闭当前图像" << endl;
  cout << "# I – Information\t输出当前图像的路径、行列值、波段数、数据类型、排列方式等信息" << endl;
  cout << "# S – Statistics\t输出图像数据统计量，若文件未打开，输出提示" << endl;
  cout << "# B - SaveData\t保存图像的统计信息到文件中" << endl;
  cout << "# H – Histogram \t输出图像的直方图" << endl;
  cout << "# ? – Help\t\t输出本信息" << endl;
  cout << "# D - DisplayImage\t显示图像" << endl;
  cout << "# R – Rotate\t\t图像旋转，输入角度逆时针" << endl;
  cout << "# Z – Zoom\t\t图像缩放，输入比例尺" << endl;
  cout << "# F - Filter\t\t输入滤波核，输出滤波后图像" << endl;
  cout << "# A – Save as\t\t输入保存的文件路径，输出图像为二进制文件" << endl;
  cout << "###########################################################" << endl;
}

// 处理用户选择
void Menu::handleChoice() {
  char choice;
  cout << "\n####################################请输入您的选择：";
  cin >> choice;
  switch (choice) {
  case 'X':
  case 'x':
    exit = true;
    getExit(); // 退出程序
    break;
  case 'O':
  case 'o':
    image = Utils::openImage(); // 打开图像
    break;
  case 'C':
  case 'c':
    Utils::closeImage(image); // 关闭图像
    break;
  case 'I':
  case 'i':
    Utils::showInfo(image); // 显示图像信息
    break;
  case 'S':
  case 's':
    Utils::showStats(image); // 显示图像统计信息
    break;
  case 'B':
  case 'b':
    Utils::saveData(image); // 保存图像信息到文件中
    break;
  case 'H':
  case 'h':
    Utils::showHistogram(image); // 显示图像直方图
    break;
  case '?':
    printMenu(); // 打印菜单信息
    break;
  case 'D':
  case 'd':
    Utils::displayImage(image); // 显示图像
    break;
  case 'R':
  case 'r':
    Utils::rotateImage(image); // 旋转图像
    break;
  case 'Z':
  case 'z':
    Utils::zoomImage(image); // 缩放图像
    break;
  case 'F':
  case 'f': {
    Utils::filterImage(image); // 滤波图像
    break;
  }
  case 'A':
  case 'a':
    Utils::saveImage(image); // 保存图像为文件
    break;
  default:
    cout << "无效的选择，请再试一次。" << endl;
  }
  return; // 返回，避免控制流到达函数的末尾
}

// 退出程序
bool Menu::getExit() {
  return exit;
}