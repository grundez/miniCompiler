#include <cstdio>
#include "defines.h"
#include "Scanner.h"

int main() {
    int typeData;
    LEX l;
    TScanner scanner;  // Создаем экземпляр класса TScaner
    scanner.getData(); // считываем файл

    do {
        typeData = scanner.scanner(l); // сканируем данные
        printf("typeData: %d, lexem: %s\n", typeData, l);
    } while (typeData != typeEnd);
    return 0;
}