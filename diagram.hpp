// file diagram.hpp

#pragma once

#include "tree.hpp"

class TDiagram
{
private:
	type_data last_type_data;
	type_object last_type_object;
	TScaner* scaner;
	void description();					// ��������
	void data();						// ������
	void dataFuncParams(Tree* func_node);				// ������ ���������� �������
	void listDataParam();
	void elementInParams();
	void variableInParams();
	void assignmentFuncParam(Tree* param, TData* value);
	void function();					// �������
	void ParameterList(Tree* func_node);// �������� ���������� �������
	void Parameter();					// �������� ���������
	void function_body();				// ���� �������
	void type();						// ���
	void list();						// ������
	void element();						// �������
	void elementInParam();
	void variable();					// ����������
	void array();						
	void cycle();
	void array_expression(Tree*);		// ��������� ����������� �������
	void array_ident(TData*);			// ������������� �������
	void assignment();					// ������������
	void expression(TData*);			// ���������
	void composite_operator();			// ��������� ��������
	void operators_and_descriptions();	// ��������� � ��������
	void operator_();					// ��������
	void function_call();				// ����� �������
	void condition();					// �������
	void comparison(TData*);			// ���������
	void BitwiseShift(TData*);			// �����
	void addendum(TData*);				// ���������
	void multiplier(TData*);			// ���������
	void unary_operation(TData*);		// ������� ��������
	void elementary_expression(TData*);	// ������������ ���������
	int look_forward(int pointer);
	int scan(type_lex lex);
	void print_variable(Tree*);
	void print_array(Tree*, int);
public:
	Tree* root;
	TDiagram(TScaner* scaner) {
		this->scaner = scaner;
		this->root = new Tree(scaner);
		this->root->set_current(this->root);
	}
	~TDiagram() {}
	void program();						// ���������
};