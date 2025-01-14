#pragma once
#include "Scaner.h"

enum type_data 
{
	TYPE_UNKNOWN = 0,
	TYPE_SHORT = 1,
	TYPE_INTEGER = 2,
	TYPE_LONG = 3,
	TYPE_CHAR = 4,
	TYPE_VOID = 5
};

enum type_object 
{
	OBJ_VAR = 1,
	OBJ_CONST = 2,
	OBJ_FUNC = 3,
};

class Tree;

struct Node 
{
	std::string id;
	type_object objectType;
	type_data dataType;
	int flagInit;
	Tree* pointer; // Указатель на узел
	void setInit() 
	{
		flagInit = 1;
	};
};

class Tree
{
private:
	Node* node;
	Tree* up;
	Tree* left;
	Tree* right;
public:
	Tree();
	Tree(Tree*, Tree*, Tree*, Node*);
	~Tree();
	void PrintError(std::string errorMessage, std::string lexeme);
	void SetLeft(Node*);
	void SetRight(Node*);
	Tree* GetLeft();
	Tree* GetRight();
	type_object GetSelfObjectType();
	type_data GetSelfDataType();
	std::string GetSelfId();
	bool IsSelfInit();
	Tree* FindUp(Tree*, std::string);
	Tree* FindUp(std::string);
	Tree* FindUp(Tree*, std::string, type_object);
	Tree* FindUp(std::string, type_object);
	Tree* FindUp(Tree*, type_object);
	Tree* FindUp(type_object);
	Tree* FindFunction(const type_lex& lex);
	Tree* FindUpOneLevel(Tree*, std::string);
	Tree* FindRightLeft(Tree* from, std::string id);
	Tree* FindRightLeft(std::string id);
	Tree* FindLeft(Tree* from, std::string id);
	Tree* FindLeft(std::string id);
	void SetInit();
	type_data GetDataType(int);
	type_data GetTypeResult(type_data, type_data, int);
	void Print(int);
	bool IsDoublicateId(Tree*, std::string);
	bool IsAllowChangeId(std::string);
	bool IsMainExists();
	bool CanBeAdded(type_data type1, type_data type2);
	bool CanBeAdded(int type1, int type2);

};