// �˵�����ͷ�ļ�

#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>

#include "Image.h"
#include "Utils.h"

using namespace std;

// �˵���
class Menu
{
private:
    Image *image;
    bool exit; // �˳���־
public:
    Menu();              // ���캯��
    ~Menu();             // ��������
    void printMenu();    // ��ӡ�˵���Ϣ
    void handleChoice(); // �����û�ѡ��
    bool getExit();      // �˳�����
};

#endif