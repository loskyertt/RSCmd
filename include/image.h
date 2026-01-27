// 图像处理功能的头文件

#ifndef IMAGE_H
#define IMAGE_H

#include <fstream>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

// 图像类
class Image {
private:
  string path; // 图像文件路径
  int rows;    // 图像行数
  int cols;    // 图像列数
  int bands;   // 图像波段数
  int type;    // 图像数据类型
public:
  Mat data;                       // 图像数据矩阵
  Image();                        // 构造函数
  Image(string path);             // 带参数的构造函数，根据路径读取图像文件
  ~Image();                       // 析构函数
  bool isEmpty();                 // 判断图像对象是否为空
  void showInfo();                // 显示图像信息
  void showStats();               // 显示图像统计信息
  void saveData(string filename); // 保存图像统计信息
  void showHistogram();           // 显示图像直方图
  void displayImage();            // 显示图像
  void rotateImage(double angle); // 旋转图像
  void zoomImage(double ratio);   // 缩放图像
  void filterImage(Mat kernel);   // 滤波图像
  void saveImage(string path);    // 保存图像为文件

  string getPath(); // 获取图像文件路名的方法
  int getRows();    // 获取图像行数的方法
  int getCols();    // 获取图像列数的方法
  int getBands();   // 获取图像波段数的方法
  int getType();    // 获取图像数据类型的方法
};

#endif