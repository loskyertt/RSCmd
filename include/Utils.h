#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#include "Image.h"

using namespace std;
using namespace cv; // ʹ��OpenCV�����ռ�

// ����������
class Utils
{
public:
    static Image *openImage();                                     // ��ͼ���ļ�������ͼ�����ָ��
    static void closeImage(Image *image);                          // �ر�ͼ���ļ����ͷ�ͼ�����ָ��
    static void showInfo(Image *image);                            // ��ʾͼ����Ϣ
    static void showStats(Image *image);                           // ��ʾͼ��ͳ����Ϣ
    static void saveData(Image *image);                            // ����ͼ��ͳ����Ϣ
    static void showHistogram(Image *image);                       // ��ʾͼ��ֱ��ͼ
    static void displayImage(Image *image);                        // ��ʾͼ��
    static void rotateImage(Image *image);                         // ��תͼ��
    static void zoomImage(Image *image);                           // ����ͼ��
    static void filterImage(Image *image);                         // �˲�ͼ��
    static void saveImage(Image *image);                           // ����ͼ��Ϊ�ļ�
};

#endif