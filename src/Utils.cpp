#include "../include/Utils.h"
#include <fstream>
#include <iomanip>

// ��ͼ���ļ�
Image *Utils::openImage()
{
    string path; // ͼ���ļ�·��
    cout << "������ͼ���ļ�·����";
    cin >> path;
    Image *image = new Image(path); // ����ͼ����󣬸���·����ȡͼ���ļ�
    if (image->isEmpty())
    {
        delete image;
        image = NULL;
        return NULL;
    }
    return image; // ����ͼ�����ָ��
}

// �ر�ͼ���ļ�
void Utils::closeImage(Image *image)
{
    if (image == NULL)
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }
    delete image; // �ͷ�ͼ�����ָ��
    image = NULL; // ����ͼ�����ָ��Ϊ��
    cout << "ͼ��ɹ��رգ�" << endl;
}

// ��ʾͼ����Ϣ
void Utils::showInfo(Image *image)
{
    if (image == NULL)
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }
    image->showInfo();
}

// ��ʾͼ��ͳ����Ϣ
void Utils::showStats(Image *image)
{
    if (image == NULL)
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }
    image->showStats();
}

// ��ʾͼ��ֱ��ͼ
void Utils::showHistogram(Image *image)
{
    if (image == NULL)
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }
    image->showHistogram();
}

// ��ʾͼ��
void Utils::displayImage(Image *image)
{
    if (image == NULL)
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }
    image->displayImage();
}

// ��תͼ��
void Utils::rotateImage(Image *image)
{
    if (image == NULL)
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }

    double angle;
    cout << "��������ת�Ƕȣ��Զ�Ϊ��λ����";
    cin >> angle;
    image->rotateImage(angle);
}

// ����ͼ��
void Utils::zoomImage(Image *image)
{
    if (image == NULL)
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }

    double ratio;
    cout << "���������ű�����";
    cin >> ratio;
    image->zoomImage(ratio);
}

// �˲�ͼ��
void Utils::filterImage(Image *image)
{
    if (image == NULL)
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }

    int kernelSize;
    cout << "�������˲��˴�С����������";
    cin >> kernelSize;

    if (kernelSize % 2 != 1)
    {
        cout << "�˲��˴�С����ȷ��" << endl;
        return;
    }

    Mat kernel(kernelSize, kernelSize, CV_32F);
    cout << "�������˲��˾���" << kernelSize << " * " << kernelSize << "����" << endl;
    for (int i = 0; i < kernelSize; i++)
    {
        for (int j = 0; j < kernelSize; j++)
        {
            cin >> kernel.at<float>(i, j);
        }
    }

    image->filterImage(kernel);
}

// ����ͼ��Ϊ�ļ�
void Utils::saveImage(Image *image)
{
    if (image == NULL)
    {
        cout << "û��ͼ�񱻴򿪣�" << endl;
        return;
    }
    string path;
    cout << "������ͼ���ļ�·���� ";
    cin >> path;
    image->saveImage(path);
}