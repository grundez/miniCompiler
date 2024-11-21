#include "defines.h"

typedef char IM[MaxText]; // текст ИМ
typedef char LEX[MaxLex]; // лексема

class TScanner {
private:
    IM t; // исходный модуль
    int uk = 0; // указатель текущей позиции в ИМ
    FILE* in; // файл с входными данными

public:
    TScanner() : uk(0), in(nullptr) {
        // Обнуляем массив t
        std::memset(t, 0, sizeof(t));
    }
    void setUK(int i); // сеттер указателя 
    int getUK(void); // геттер указателя
    void printError(char i); // 
    int scanner(LEX lex);
    void getData();
};
