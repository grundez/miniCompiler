#pragma once
#include "Defines.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Scaner
{
private:
    std::string text; // �������� �����
    size_t uk;        // ��������� ������� ������� � �������� ������
    std::string lastLexeme;

public:
    void PutUK(size_t uk);                // ���������� ��������� ������� ������� � �������� ������
    size_t GetUK();                       // �������� ��������� ������� ������� � �������� ������
    void PrintError(const std::string& error, const std::string& text); // ����� ������
    int UseScaner(type_lex lex);          // ������ ������ �������
    void GetData(const std::string& filename); // ��������� ���������� �� �����
    Scaner(const std::string& filename);
    std::vector<int> lineBreakPositions;
    std::string GetLastLexeme();
};