#pragma once

#include "Scaner.h"
#include "Tree.h"

class Diagram
{
private:
	Scaner* scaner;
	Tree* tree;
	void Description();						// Описание
	void Parameter();
	void ArgumentList();
	void Data();							// Данные
	void Function();						// Функция
	void Type();							// Тип
	void List();							// Список
	void Variable(int typeData1);			// Переменная
	void Assignment(int flagChar);			// Присваивание
	void Expression();						// Выражение
	void CompositeOperator();				// Составной оператор
	void OperatorsAndDescriptions();		// Операторы и описания
	void ParameterList();					// Описание параметров функции
	void Operator();						// Оператор
	void FunctionCall();					// Вызов функции
	void Comparison();						// Сравнение
	void BitwiseShift();					// Побитовый сдвиг
	void Summand();							// Слагаемое
	void Multiplier();						// Множитель
	void UnaryOperation();					// Унарная операция
	void ElementaryExpression();			// Элементарное выражение
	void Cycle();							// Цикл
	void ReturnStatement();					// Оператор возврата (return)
	int LookForward(int pointer);			
	int Scan(type_lex lex);
	
public:
	Diagram(Scaner* scaner, Tree* tree);
	void Program();							// Программа

};