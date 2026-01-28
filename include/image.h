// 图像处理功能的头文件

#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <string>

/* 图像类 */
class Image {
 private:
  std::string m_path;  // 图像文件路径
  int m_rows;          // 图像行数
  int m_cols;          // 图像列数
  int m_bands;         // 图像波段数
  int m_type;          // 图像数据类型

 public:
  cv::Mat m_data;  // 图像数据矩阵

 public:
  Image();                  // 默认构造函数
  Image(std::string path);  // 构造函数：读取图像文件信息
  ~Image();                 // 析构函数

 public:
  std::string getPath();  // 获取图像文件路名
  int getRows();          // 获取图像行数
  int getCols();          // 获取图像列数
  int getBands();         // 获取图像波段数
  int getType();          // 获取图像数据类型

 public:
  bool isEmpty();                       // 判断图像对象是否为空
  void showInfo();                      // 显示图像信息
  void showStats();                     // 显示图像统计信息
  void saveData(std::string filename);  // 保存图像统计信息
  void showHistogram();                 // 显示图像直方图
  void displayImage();                  // 显示图像
  void rotateImage(double angle);       // 旋转图像
  void zoomImage(double ratio);         // 缩放图像
  void filterImage(cv::Mat kernel);     // 滤波图像
  void saveImage(std::string path);     // 保存图像为文件
};

#endif