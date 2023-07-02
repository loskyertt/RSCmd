// ͼ�����ܵ�ͷ�ļ�

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

// ͼ����
class Image
{
private:
    string path; // ͼ���ļ�·��
    int rows;    // ͼ������
    int cols;    // ͼ������
    int bands;   // ͼ�񲨶���
    int type;    // ͼ����������
public:
    Mat data;                       // ͼ�����ݾ���
    Image();                        // ���캯��
    Image(string path);             // �������Ĺ��캯��������·����ȡͼ���ļ�
    ~Image();                       // ��������
    bool isEmpty();                 // �ж�ͼ������Ƿ�Ϊ��
    void showInfo();                // ��ʾͼ����Ϣ
    void showStats();               // ��ʾͼ��ͳ����Ϣ
    void saveData(string filename); // ����ͼ��ͳ����Ϣ
    void showHistogram();           // ��ʾͼ��ֱ��ͼ
    void displayImage();            // ��ʾͼ��
    void rotateImage(double angle); // ��תͼ��
    void zoomImage(double ratio);   // ����ͼ��
    void filterImage(Mat kernel);   // �˲�ͼ��
    void saveImage(string path);    // ����ͼ��Ϊ�ļ�

    string getPath(); // ��ȡͼ���ļ�·���ķ���
    int getRows();    // ��ȡͼ�������ķ���
    int getCols();    // ��ȡͼ�������ķ���
    int getBands();   // ��ȡͼ�񲨶����ķ���
    int getType();    // ��ȡͼ���������͵ķ���
};

#endif