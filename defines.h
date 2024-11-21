#pragma once
#include <cstdio>         
#include <cstdlib>       
#include <cstring>  
#include <fstream>
#include <sstream>

//
#define MaxText 10000    
#define MaxLex 100    

// Таблица лексем
#define typeIdf  1

#define typeInt 10
#define typeShort 11
#define typeLong 12
#define typeChar 13
#define typeWhile 14
#define typeVoid 15
#define typeMain 16

#define typeConst10 20
#define typeConst16 21
#define typeConstChar  22

#define typeComma 30
#define typeSml 31
#define typeLeftBr 32
#define typeRightBr 33
#define typeLeftFBR  34
#define typeRightFBR  35

#define typeLess 40
#define typeLessOrEq 41
#define typeMore 42
#define typeMoreOrEq 43
#define typeEqual 44
#define typeNEqual 45
#define typeAdd 46
#define typeMinus 47
#define typeMult 48
#define typeDiv 49
#define typeMod 50
#define typeSave 51
#define typeLeftShift 52 
#define typeRightShift 53

#define typeEnd 100
#define typeError 200

