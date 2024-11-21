#include "defines.h"

typedef char IM[MaxText]; // ����� ��
typedef char LEX[MaxLex]; // �������

class TScanner {
private:
    IM t; // �������� ������
    int uk = 0; // ��������� ������� ������� � ��
    FILE* in; // ���� � �������� �������

public:
    TScanner() : uk(0), in(nullptr) {
        // �������� ������ t
        std::memset(t, 0, sizeof(t));
    }
    void setUK(int i); // ������ ��������� 
    int getUK(void); // ������ ���������
    void printError(char i); // 
    int scanner(LEX lex);
    void getData();
};
