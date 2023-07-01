#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#include "Image.h"

using namespace std;
using namespace cv; // 使用OpenCV命名空间

// 辅助函数类
class Utils
{
public:
    static Image *openImage();                                     // 打开图像文件，返回图像对象指针
    static void closeImage(Image *image);                          // 关闭图像文件，释放图像对象指针
    static void showInfo(Image *image);                            // 显示图像信息
    static void showStats(Image *image);                           // 显示图像统计信息
    static void saveData(Image *image);                            // 保存图像统计信息
    static void showHistogram(Image *image);                       // 显示图像直方图
    static void displayImage(Image *image);                        // 显示图像
    static void rotateImage(Image *image);                         // 旋转图像
    static void zoomImage(Image *image);                           // 缩放图像
    static void filterImage(Image *image);                         // 滤波图像
    static void saveImage(Image *image);                           // 保存图像为文件
};

#endif