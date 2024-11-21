#include "defines.h"
#include "Scanner.h"
void TScanner::setUK(int i) {
    uk = i;
}

int TScanner::getUK(void) {
    return uk;
}

void TScanner::printError(char i) {
    printf("Error: invalid symbol: %c at the position %d\n", i, uk);
    exit(EXIT_FAILURE);
}

int TScanner::scanner(LEX l) {
    int i = 0; 
    char currentSymbol;

    while ((currentSymbol = t[uk]) == ' ' || currentSymbol == '\n' || currentSymbol == '\t') {
        uk++; 
    }

    if (currentSymbol == '\0') {
        l[0] = '\0';
        return typeEnd;
    }

    if (currentSymbol == '/' && t[uk + 1] == '/') {
        printf("Detected comment.\n");
        while (currentSymbol != '\n' && currentSymbol != '\0') {
            currentSymbol = t[++uk];
        }
        return scanner(l);
    }

    if ((currentSymbol >= 'a' && currentSymbol <= 'z') || (currentSymbol >= 'A' && currentSymbol <= 'Z') || currentSymbol == '_') {
        do {
            if (i < MaxLex - 1) {  
                l[i++] = currentSymbol;
            }
            else
            {
                printf("Error: line exceeds maximum allowed length of characters.\n");
                exit(EXIT_FAILURE);
            }
            currentSymbol = t[++uk];
        } while ((currentSymbol >= 'a' && currentSymbol <= 'z') || (currentSymbol >= 'A' && currentSymbol <= 'Z')
            || (currentSymbol >= '0' && currentSymbol <= '9') || currentSymbol == '_');
        l[i] = '\0';

        if (strcmp(l, "int") == 0) return typeShort;
        else if (strcmp(l, "short") == 0) return typeInt;
        else if (strcmp(l, "long") == 0) return typeLong;
        else if (strcmp(l, "char") == 0) return typeChar;
        else if (strcmp(l, "while") == 0) return typeWhile;
        else if (strcmp(l, "void") == 0) return typeVoid;
        else if (strcmp(l, "main") == 0) return typeMain;
        else return typeIdf;
    }

    if (currentSymbol >= '0' && currentSymbol <= '9') {
        if (currentSymbol == '0' && (t[uk + 1] == 'x' || t[uk + 1] == 'X')) {
            // ��� ����������������� �����
            l[i++] = currentSymbol; // ���������� '0'
            currentSymbol = t[++uk]; // ��������� � 'x' ��� 'X'
            l[i++] = currentSymbol; // ���������� 'x' ��� 'X'

            currentSymbol = t[++uk]; // ��������� � ������� ����� 'x'
            if (!((currentSymbol >= '0' && currentSymbol <= '9') ||
                (currentSymbol >= 'a' && currentSymbol <= 'f') ||
                (currentSymbol >= 'A' && currentSymbol <= 'F'))) {
                printError(currentSymbol); // ������, ���� ����� 'x' ��� ����������� �������
            }

            do {
                l[i++] = currentSymbol; // ���������� ����������������� �����
                currentSymbol = t[++uk];
            } while ((currentSymbol >= '0' && currentSymbol <= '9') ||
                (currentSymbol >= 'a' && currentSymbol <= 'f') ||
                (currentSymbol >= 'A' && currentSymbol <= 'F'));

            l[i] = '\0'; // ��������� �������
            return typeConst16; // ���������� ��� ������������������ �����
        }
        else {
            // ��� ���������� �����
            do {
                if (i < MaxLex - 1) {
                    l[i++] = currentSymbol;
                }
                else {
                    printf("Error: maximum number of characters exceeded.\n");
                    exit(EXIT_FAILURE);
                }
                currentSymbol = t[++uk];
            } while (currentSymbol >= '0' && currentSymbol <= '9');

            l[i] = '\0'; // ��������� �������
            return typeConst10; // ���������� ��� ����������� �����
        }
    }

    if (currentSymbol == '/' && t[uk + 1] == '*') {
        printf("Unterminated block comment.\n");
        // ���������� ������� '/*'
        uk += 2;
        currentSymbol = t[uk];

        // ���������� ���������� �����������
        while (currentSymbol != '\0') {
            if (currentSymbol == '*' && t[uk + 1] == '/') {
                uk += 2; // ���������� ������� '*/'
                currentSymbol = t[uk];
                break;
            }
            currentSymbol = t[++uk];
        }

        // ���� ��������� ����� ����� �� '*/', ��������� � �������
        if (currentSymbol == '\0') {
            printf("Error: unterminated block comment.\n");
            exit(EXIT_FAILURE);
        }

        return scanner(l); // ������������ � ������������
    }



    switch (currentSymbol) {
    case ',':
        l[i++] = currentSymbol; l[i] = '\0'; uk++;
        return typeComma;
    case ';':
        l[i++] = currentSymbol; l[i] = '\0'; uk++;
        return typeSml;
    case '(':
        l[i++] = currentSymbol; l[i] = '\0'; uk++;
        return typeLeftBr;
    case ')':
        l[i++] = currentSymbol; l[i] = '\0'; uk++;
        return typeRightBr;
    case '{':
        l[i++] = currentSymbol; l[i] = '\0'; uk++;
        return typeLeftFBR;
    case '}':
        l[i++] = currentSymbol; l[i] = '\0'; uk++;
        return typeRightFBR;
    case '=':
        if (t[uk + 1] == '=') {
            l[i++] = currentSymbol; currentSymbol = t[++uk];
            l[i++] = currentSymbol; l[i] = '\0'; uk++;
            return typeEqual;
        }
        else {
            l[i++] = currentSymbol; l[i] = '\0'; uk++;
            return typeSave;
        }
    case '>':
        if (t[uk + 1] == '=') {
            l[i++] = currentSymbol; currentSymbol = t[++uk];
            l[i++] = currentSymbol; l[i] = '\0'; uk++;
            return typeMoreOrEq;
        }
        else if (t[uk + 1] == '>') {
            l[i++] = currentSymbol; currentSymbol = t[++uk];
            l[i++] = currentSymbol; l[i] = '\0'; uk++;
            return typeRightShift;
        }
        else {
            l[i++] = currentSymbol; l[i] = '\0'; uk++;
            return typeMore;
        }
    case '<':
        if (t[uk + 1] == '=') {
            l[i++] = currentSymbol; currentSymbol = t[++uk];
            l[i++] = currentSymbol; l[i] = '\0'; uk++;
            return typeLessOrEq;
        }
        else if(t[uk + 1] == '<'){
            l[i++] = currentSymbol; currentSymbol = t[++uk];
            l[i++] = currentSymbol; l[i] = '\0'; uk++;
            return typeLeftShift;
        }
        else {
            l[i++] = currentSymbol; l[i] = '\0'; uk++;
            return typeLess;
        }
    case '!':
        if (t[uk + 1] == '=') {
            l[i++] = currentSymbol; currentSymbol = t[++uk];
            l[i++] = currentSymbol; l[i] = '\0'; uk++;
            return typeNEqual;
        }
        else {
            printError(currentSymbol);
            return typeError;
        }
    case '+':
        l[i++] = currentSymbol;
        l[i] = '\0';
        uk++;
        return typeAdd;

    case '-':
        l[i++] = currentSymbol;
        l[i] = '\0';
        uk++;
        return typeMinus;
    case '*':
        l[i++] = currentSymbol; l[i] = '\0'; uk++;
        return typeMult;
    case '/':
        l[i++] = currentSymbol; l[i] = '\0'; uk++;
        return typeDiv;
    case '%':
        l[i++] = currentSymbol; l[i] = '\0'; uk++;
        return typeMod;
    default:
        printError(currentSymbol);
        return typeError;
        exit(0);
    }
}

void TScanner::getData() {
    errno_t err = fopen_s(&in, "input.txt", "r");
    if (err != 0) {
        perror("Error: unable to open file.");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (!feof(in) && i < MaxText - 1) {
        char ch;
        if (fscanf_s(in, "%c", &ch, 1) != 1) {
            break;
        }
        t[i++] = ch;
    }
    t[i] = '\0';
    fclose(in);
}

