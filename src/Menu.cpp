#include "../include/Menu.h"

// ���캯��
Menu::Menu()
{
    image = NULL; // ��ʼ��ͼ�����ָ��Ϊ��
    exit = false; // ��ʼ���˳���־Ϊfalse
}

// ��������
Menu::~Menu()
{
    if (image != NULL)
    {
        // ���ͼ�����ָ�벻Ϊ�գ��ͷ��ڴ�ռ�
        delete image;
        image = NULL;
    }
}

// ��ӡ�˵���Ϣ
void Menu::printMenu()
{
    cout << "############ Remote Sensing Image Process Tools ############" << endl;
    cout << "# X �C Exit\t\t�˳�����" << endl;
    cout << "# O �C Open\t\t��Ӱ���ļ�" << endl;
    cout << "# C �C Close\t\t�رյ�ǰͼ��" << endl;
    cout << "# I �C Information\t�����ǰͼ���·��������ֵ�����������������͡����з�ʽ����Ϣ" << endl;
    cout << "# S �C Statistics\t���ͼ������ͳ���������ļ�δ�򿪣������ʾ" << endl;
    cout << "# B - SaveData\t����ͼ���ͳ����Ϣ���ļ���" << endl;
    cout << "# H �C Histogram \t���ͼ���ֱ��ͼ" << endl;
    cout << "# ? �C Help\t\t�������Ϣ" << endl;
    cout << "# D - DisplayImage\t��ʾͼ��" << endl;
    cout << "# R �C Rotate\t\tͼ����ת������Ƕ���ʱ��" << endl;
    cout << "# Z �C Zoom\t\tͼ�����ţ����������" << endl;
    cout << "# F - Filter\t\t�����˲��ˣ�����˲���ͼ��" << endl;
    cout << "# A �C Save as\t\t���뱣����ļ�·�������ͼ��Ϊ�������ļ�" << endl;
    cout << "###########################################################" << endl;
}

// �����û�ѡ��
void Menu::handleChoice()
{
    char choice;
    cout << "\n####################################����������ѡ��";
    cin >> choice;
    switch (choice)
    {
    case 'X':
    case 'x':
        exit = true;
        getExit(); // �˳�����
        break;
    case 'O':
    case 'o':
        image = Utils::openImage(); // ��ͼ��
        break;
    case 'C':
    case 'c':
        Utils::closeImage(image); // �ر�ͼ��
        break;
    case 'I':
    case 'i':
        Utils::showInfo(image); // ��ʾͼ����Ϣ
        break;
    case 'S':
    case 's':
        Utils::showStats(image); // ��ʾͼ��ͳ����Ϣ
        break;
    case 'B':
    case 'b':
        Utils::saveData(image); // ����ͼ����Ϣ���ļ���
        break;
    case 'H':
    case 'h':
        Utils::showHistogram(image); // ��ʾͼ��ֱ��ͼ
        break;
    case '?':
        printMenu(); // ��ӡ�˵���Ϣ
        break;
    case 'D':
    case 'd':
        Utils::displayImage(image); // ��ʾͼ��
        break;
    case 'R':
    case 'r':
        Utils::rotateImage(image); // ��תͼ��
        break;
    case 'Z':
    case 'z':
        Utils::zoomImage(image); // ����ͼ��
        break;
    case 'F':
    case 'f':
    {
        Utils::filterImage(image); // �˲�ͼ��
        break;
    }
    case 'A':
    case 'a':
        Utils::saveImage(image); // ����ͼ��Ϊ�ļ�
        break;
    default:
        cout << "��Ч��ѡ��������һ�Ρ�" << endl;
    }
    return; // ���أ�������������ﺯ����ĩβ
}

// �˳�����
bool Menu::getExit()
{
    return exit;
}