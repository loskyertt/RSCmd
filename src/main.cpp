#include <iostream>
#include "../include/Menu.h"

using namespace std;

int main()
{
    Menu menu;        // �����˵�����
    menu.printMenu(); // ��ӡ�˵���Ϣ
    while (!menu.getExit())
    {
        menu.handleChoice(); // �����û�ѡ��
    }

    cout << "��л��ʹ��ң��ͼ��������ټ���" << endl;

    system("pause");
    return 0;
}