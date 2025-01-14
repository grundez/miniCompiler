#include "Scaner.h"

/**
* ������ ���������� �������� ���� � ���������
*/
type_lex keyword[maxKeyword] =
{
    "int", "short", "long", "char", "while", "main", "void"
};

/**
* ������ ���������������, ������� ������������� ���������� �������� ������ � ���������
*/
int indexKeyword[maxKeyword] =
{
    typeInt, typeShort, typeLong, typeChar, typeWhile, typeMain, typeVoid
};

//������ ���� ���� ������
void Scaner::PutUK(size_t uk) 
{
    if (uk < text.size()) 
    {
        this->uk = uk;
    }
    else if(text.size() != 0)
    {
        std::cerr << "��������� ����� �� ������� ������." << std::endl;
        exit(1);
    }
}

size_t Scaner::GetUK()
{
    return uk;
}

void Scaner::GetData(const std::string& filename)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        text = buffer.str(); // ��������� ����� �� �����
        text += '\n';

        // ���������� ������� �������� ������
        for (size_t i = 0; i < text.size(); ++i) {
            if (text[i] == '\n') {
                lineBreakPositions.push_back(i);
            }
        }

        std::cout << "������� ����� ���������:" << std::endl << text << std::endl;
        std::cout << std::endl << "��������� �������:" << std::endl;
    }
    else
    {
        PrintError("������ �������� ��� �������� �����.", filename);
    }
}

void Scaner::PrintError(const std::string& error, const std::string& lexeme)
{
    int line = 1;
    int pos = uk;

    for (size_t i = 0; i < lineBreakPositions.size(); ++i) {
        if (lineBreakPositions[i] < uk) {
            line++;
        }
        else {
            pos = uk - (i == 0 ? 0 : lineBreakPositions[i - 1] + 1);
            break;
        }
    }

    std::cout << "������: " << error << "������ " << line << ", ������� " << pos;
    if (!lexeme.empty())
    {
        std::cout << ", �������: " << lexeme;
    }
    std::cout << std::endl;
    exit(1);
}

int Scaner::UseScaner(type_lex lex)
{
    int i = 0;
    lex[0] = '\0';

    while (true)
    {
        // ���������� �������
        while (uk < text.size() && (text[uk] == ' ' || text[uk] == '\t' || text[uk] == '\n'))
        {
            uk++;
        }

        // ����� ��������� 
        if (uk >= text.size())
        {
            lex[i] = '\0';
            return typeEnd;
        }

        // ��������� ������������
        if (text[uk] == '/')
        {
            uk++;
            if (uk < text.size() && text[uk] == '/')
            {
                while (uk < text.size() && text[uk] != '\n')
                {
                    uk++;
                }
                continue;
            }
            else
            {
                // �������
                lex[i++] = '/';
                lex[i++] = '\0';
                return typeDiv;
            }
        }

        // ��������� �������� � ���������� ��������
        if (isdigit(text[uk]) || text[uk] == '\'')
        {
            // ��������� ���������� ��������
            if (text[uk] == '\'')
            {
                uk++;
                if (uk < text.size() && text[uk] != '\'' && text[uk] != '\n')
                {
                    lex[i++] = text[uk++];
                }
                else
                {
                    PrintError("������������ ���������� ���������", lex);
                    return typeError;
                }

                if (uk < text.size() && text[uk] == '\'')
                {
                    uk++;
                    lex[i] = '\0';
                    return constChar;
                }
                else
                {
                    PrintError("��������� ����������� ������� ��� ���������� ���������", lex);
                    return typeError;
                }
            }

            // ��������� �������� ��������
            if (text[uk] == '0' && (uk + 1 < text.size()) && (text[uk + 1] == 'x' || text[uk + 1] == 'X'))
            {
                // ����������������� ���������
                lex[i++] = text[uk++]; // '0'
                lex[i++] = text[uk++]; // 'x' ��� 'X'
                if (uk < text.size() && isxdigit(text[uk]))
                {
                    while (uk < text.size() && isxdigit(text[uk]) && i < maxNumber - 1)
                    {
                        lex[i++] = text[uk++];
                    }
                    lex[i] = '\0';
                    lastLexeme = lex;
                    return typeConst16;
                }
                else
                {
                    PrintError("������������ ����������������� ���������", lex);
                    return typeError;
                }
            }

            // ���������� ���������
            while (uk < text.size() && isdigit(text[uk]) && i < maxNumber - 1)
            {
                lex[i++] = text[uk++];
            }

            lex[i] = '\0';
            lastLexeme = lex;
            return typeConst10;
        }


        // ��������� ���������������
        if (isalpha(text[uk]) || text[uk] == '_')
        {
            while (uk < text.size() && (isalnum(text[uk]) || text[uk] == '_') && i < maxLex - 1)
            {
                lex[i++] = text[uk++];
            }
            lex[i] = '\0';

            if (i == maxLex - 1 && (uk < text.size() && (isalnum(text[uk]) || text[uk] == '_')))
            {
                while (uk < text.size() && (isalnum(text[uk]) || text[uk] == '_'))
                {
                    uk++;
                }
                PrintError("������������� ��������� ������������ ����� �������.", lex);
                return typeError;
            }

            // ���������, �������� �� ������������� �������� ������
            for (int j = 0; j < maxKeyword; j++)
            {
                if (strcmp(lex, keyword[j]) == 0)
                {
                    return indexKeyword[j];
                }
            }
            return typeId;
        }

        // ��������� ����������� ��������
        switch (text[uk])
        {
        case ',':
            uk++; lex[i++] = ','; lex[i] = '\0'; return typeComma;
        case ';':
            uk++; lex[i++] = ';'; lex[i] = '\0'; return typeSemicolon;
        case '(':
            uk++; lex[i++] = '('; lex[i] = '\0'; return typeLeftBracket;
        case ')':
            uk++; lex[i++] = ')'; lex[i] = '\0'; return typeRightBracket;
        case '{':
            uk++; lex[i++] = '{'; lex[i] = '\0'; return typeLeftBrace;
        case '}':
            uk++; lex[i++] = '}'; lex[i] = '\0'; return typeRightBrace;
        case '+':
            uk++; lex[i++] = '+'; lex[i] = '\0'; return typePlus;
        case '-':
            uk++; lex[i++] = '-'; lex[i] = '\0'; return typeMinus;
        case '*':
            uk++; lex[i++] = '*'; lex[i] = '\0'; return typeMul;
        case '%':
            uk++; lex[i++] = '%'; lex[i] = '\0'; return typeMod;
        case '=':
            uk++;
            lex[i++] = '=';
            if (uk < text.size() && text[uk] == '=')
            {
                uk++;
                lex[i++] = '=';
                lex[i] = '\0';
                return typeEq;
            }
            else
            {
                lex[i] = '\0';
                return typeEval;
            }
        case '!':
            uk++;
            lex[i++] = '!';
            if (uk < text.size() && text[uk] == '=')
            {
                uk++;
                lex[i++] = '=';
                lex[i] = '\0';
                return typeUnEq;
            }
            else
            {
                lex[i] = '\0';
                PrintError("�������� ������: ", lex);
                return typeError;
            }
        case '>':
            uk++;
            lex[i++] = '>';
            if (uk < text.size() && text[uk] == '=')
            {
                uk++;
                lex[i++] = '=';
                lex[i] = '\0';
                return typeMoreOrEq;
            }
            else if (uk < text.size() && text[uk] == '>')
            {
                uk++;
                lex[i++] = '>';
                lex[i] = '\0';
                return typeBitwiseRight;
            }
            else
            {
                lex[i] = '\0';
                return typeMore;
            }
        case '<':
            uk++;
            lex[i++] = '<';
            if (uk < text.size() && text[uk] == '=')
            {
                uk++;
                lex[i++] = '=';
                lex[i] = '\0';
                return typeLessOrEq;
            }
            else if (uk < text.size() && text[uk] == '<')
            {
                uk++;
                lex[i++] = '<';
                lex[i] = '\0';
                return typeBitwiseLeft;
            }
            else
            {
                lex[i] = '\0';
                return typeLess;
            }
        default:
            lex[i++] = text[uk++];
            lex[i] = '\0';
            PrintError("����������� ������ � ������ ", lex);
            return typeError;
        }
    }
}

Scaner::Scaner(const std::string& filename)
{
    GetData(filename);
    PutUK(0);
}

std::string Scaner::GetLastLexeme() {
    // ���������� ��������� �������� ��������� �������
    return this->lastLexeme; // �����������, lastLexeme ������ ����� ������� �������
}