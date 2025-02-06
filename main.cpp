﻿#define _CRT_SECURE_NO_WARNINGS

#include "diagram.hpp"
#include <iomanip>


int main(int argc, char* argv[])
{
    TScaner* scaner;
    scaner = new TScaner("input.txt");
    TDiagram* diagram = new TDiagram(scaner);

	diagram->program();

    int type; type_lex lex;
    type=scaner->scaner(lex);
    std::cout << "\nOutput result: " << std::endl;
    if (type == TEnd)
        std::cout << "Succsess" << std::endl;
    else
		std::cout << "ERROR" << std::endl;
    //std::cout << "\nSemantic tree build:\n";
    //diagram->root->Print(0);
}

/*
int main(int argc, char* argv[]) 
{
    TScaner* scaner;
    int type;
    type_lex lex;

    if (argc <= 1) 
        scaner = new TScaner("input.txt");
    else 
        scaner = new TScaner(argv[1]);
    do
    {
        type = scaner->scaner(lex);
        std::cout << std::setw(32) << lex << "   type of:    " << type << std::endl;
    } while (type != TEnd);
    delete scaner;
    return 0;
}
*/