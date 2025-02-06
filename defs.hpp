#pragma once

#include <cstdlib>
#include <string>

#define MAX_TEXT 16384
#define MAX_LEX 64
// + 1 Для '\0' Максимальные числа __int64 (long long)
#define MAX_DEC_LEX 20
#define MAX_HEX_LEX 19

#define MAX_KEYWORD 16

typedef char type_lex[MAX_LEX];
typedef char type_mod[MAX_TEXT];

// Идентификатор
#define TIdent 1
// Константы
#define TConst10 11
#define TConst16 12
#define TConstChar 13
// Ключевые слова
#define TInt 21
#define TShort 22
#define TLong 23
//#define T__Int64 24
#define TChar 25
#define TWhile 31
#define TMain 32
#define TVoid 33
// Специальные знаки
#define TComma 41
#define TSemicolon 42
#define TLeftBracket 43
#define TRightBracket 44
#define TLeftBrace 45
#define TRightBrace 46

// Операции
#define TAdd 51
#define TSub 52
#define TMul 53
#define TDiv 54
#define TMod 55
#define TEval 56
#define TEq 57
#define TNe 58
#define TGt 59
#define TLt 60
#define TGe 61
#define TLe 62
#define TLeftShift 63 
#define TRightShift 64
// Дополнительно
#define TEnd 71
#define TErr 72