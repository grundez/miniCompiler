#include "Diagram.h"

Diagram::Diagram(Scaner* scaner, Tree* tree)
{
	this->scaner = scaner;
	this->tree = tree;
}

int Diagram::LookForward(int pos) 
{
	type_lex lex;
	int saved_pointer = scaner->GetUK();
	int next_type;
	for (int i = 0; i < pos; i++)
		next_type = scaner->UseScaner(lex);
	scaner->PutUK(saved_pointer);
	return next_type;
}

int Diagram::Scan(type_lex lex) 
{
	return scaner->UseScaner(lex);
}

void Diagram::Program()
{
	type_lex lex;
	int type;
	type = LookForward(1);
	while (type == typeInt || type == typeShort || type == typeLong || type == typeChar || type == typeVoid)
	{
		Description();
		type = LookForward(1);
	}
	if (type != typeEnd)
	{
		type = Scan(lex);
		scaner->PrintError("ожидался конец, ", lex);
	}
}

void Diagram::Description()
{
	type_lex lex;
	int type = LookForward(1);
	int simbol = LookForward(3);
	if ((type == typeInt || type == typeShort || type == typeLong || type == typeChar) && simbol != typeLeftBracket)
	{
		Data();
		return;
	}
	if ((type == typeInt || type == typeShort || type == typeLong || type == typeChar || type == typeVoid) && simbol == typeLeftBracket)
	{
		Function();
		return;
	}
	type = Scan(lex);
	scaner->PrintError("ожидался тип (int, short, long, void), ", lex);
}

void Diagram::List()
{
	type_lex lex;
	type_lex* lexData;
	int type, pointer;
	int typeData = LookForward(1);
	Scan(lex);
	Variable(typeData);
	type = LookForward(1);
	while (type == typeComma)
	{
		type = Scan(lex);
		Variable(typeData);
		type = LookForward(1);
	}
}

void Diagram::Data()
{
	type_lex lex;
	int type_;
	Type();
	List();
	type_ = Scan(lex);
	if (type_ != typeSemicolon)
	{
		scaner->PrintError("ожидалась ;, ", lex);
	}
}

void Diagram::Function() {

	type_lex lex;
	int type = Scan(lex);
	if (type != typeInt && type != typeShort && type != typeLong && type != typeChar && type != typeVoid) {
		scaner->PrintError("ожидался тип (int, short, long, char), ", lex);
	}

	
	int type1 = type;
	type = Scan(lex);
	if (type != typeId && type != typeMain) {
		scaner->PrintError("ожидался идентификатор, ", lex);
	}

	Node* newNode = new Node();
	newNode->id = lex; // Сохраняем идентификатор
	newNode->objectType = OBJ_FUNC;
	newNode->dataType = tree->GetDataType(type1);

	// Вставляем узел в дерево
	tree->SetLeft(newNode);
	tree = tree->GetLeft(); // Переход к новому узлу
	tree->SetRight(NULL);

	Tree* tmpTree = tree; // Сохраняем текущий указатель дерева
	tree = tree->GetRight(); // Переход к правому поддереву

	type = Scan(lex);
	if (type != typeLeftBracket) {
		scaner->PrintError("ожидалась (, ", lex);
	}

	// Обработка параметров функции
	type = LookForward(1);
	if (type != typeRightBracket) { // Если есть параметры
		ParameterList();
	}

	type = Scan(lex);
	if (type != typeRightBracket) {
		scaner->PrintError("ожидалась ), ", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBrace) {
		scaner->PrintError("ожидалась {, ", lex);
	}

	OperatorsAndDescriptions();

	// Возвращаемся к предыдущему узлу
	tree = tmpTree;

	type = Scan(lex);
	if (type != typeRightBrace) {
		scaner->PrintError("ожидалась }, ", lex);
	}
}

void Diagram::ParameterList() {
	type_lex lex;
	int type;

	// Обработка первого параметра
	Parameter();
	type = LookForward(1);

	// Обработка остальных параметров через запятую
	while (type == typeComma) {
		type = Scan(lex);
		Parameter();
		type = LookForward(1);
	}
}

void Diagram::Parameter() {
	type_lex lex;
	int type = LookForward(1);

	// Проверка типа параметра
	if (type != typeInt && type != typeShort && type != typeLong && type != typeChar) {
		type = Scan(lex);
		scaner->PrintError("ожидался тип параметра (int, short, long, char), ", lex);
	}
	type_data typeData = tree->GetDataType(type);
	type = Scan(lex); // Считываем идентификатор
	type = LookForward(1);
	if (type != typeId) {
		scaner->PrintError("ожидался идентификатор параметра, ", lex);
	}
	type = Scan(lex);

	// Добавляем параметр в дерево
	Node* paramNode = new Node();
	paramNode->id = lex;
	paramNode->objectType = OBJ_VAR; // Параметры обрабатываются как переменные
	paramNode->dataType = typeData;
	tree->SetLeft(paramNode);
	tree = tree->GetLeft();
	tree->SetRight(NULL);
}



void Diagram::Type()
{
	type_lex lex;
	int type;
	type = LookForward(1);
	if (type != typeInt && type != typeShort && type != typeLong && type != typeChar)
	{
		scaner->PrintError("ожидался тип (int, short, long, char), ", lex);
	}
}

void Diagram::Variable(int typeData1)
{
	type_lex lex;
	int flagChar = 0;
	Node* newNode = new Node();
	int type;
	if (typeData1 != typeInt && typeData1 != typeShort && typeData1 != typeLong && typeData1 != typeChar)
	{
		scaner->PrintError("ожидался тип (int, short, long, char), ", lex);
	}
	if (typeData1 == typeInt)
		newNode->dataType = TYPE_INTEGER;
	else if (typeData1 == typeShort)
		newNode->dataType = TYPE_SHORT;
	else if (typeData1 == typeLong)
		newNode->dataType = TYPE_LONG;
	else if (typeData1 == typeChar) {
		newNode->dataType = TYPE_CHAR;
		type = LookForward(2);
		if (type == typeEval)
		{
			flagChar = 1;
		}
	}
	else
		newNode->dataType = TYPE_UNKNOWN;
	type = LookForward(1);
	type_data typeData = tree->GetDataType(type);

	if (type != typeId)
	{
		type = Scan(lex);
		scaner->PrintError("ожидался идентификатор, ", lex);
	}

	int pointer = scaner->GetUK();
	type = Scan(lex);

	if (tree->IsDoublicateId(tree, lex)) {
		tree->PrintError("Переопределение", lex);
	}

	newNode->id = lex;  // Устанавливаем идентификатор
	newNode->objectType = OBJ_VAR;

	type = LookForward(1);
	if (type == typeEval) {
		newNode->flagInit = 1;
	}
	else
	{
		newNode->flagInit = 0;
	}

	// Добавляем узел в левое поддерево
	tree->SetLeft(newNode);

	// Переход к левому дочернему узлу для дальнейших операций
	tree = tree->GetLeft();

	scaner->PutUK(pointer);

	type = LookForward(2);
	if (type == typeEval)
	{
		Assignment(flagChar);
		return;
	}
	type = Scan(lex);
}

void Diagram::Assignment(int flag)
{
	type_lex lex;
	int type = Scan(lex);

	if (type != typeId)
	{
		scaner->PrintError("ожидался идентификатор, ", lex);
	}

	Tree* node = tree->FindUp(lex);

	if (node == NULL) {
		scaner->PrintError("Семантическая ошибка. ID не найден", lex);
	}

	if (node->GetSelfObjectType() != OBJ_VAR) {
		scaner->PrintError("Семантическая ошибка. Попытка использования не переменной в присваивании", lex);
	}

	node->SetInit();

	// Получаем тип переменной узла
	type_data varType = node->GetSelfDataType();
	type = Scan(lex);
	if (type != typeEval)
	{
		scaner->PrintError("ожидалось =, ", lex);
	}

	type = LookForward(1); // Следующее значение для присваивания
	//std::cout << type; std::cout << flag;
	if (flag && (type == typeConst10 || type == typeConst16)) {
		std::string value = scaner->GetLastLexeme().c_str();
		if (value < "0" || value > "255") {
			scaner->PrintError("Семантическая ошибка. Значение выходит за пределы диапазона char (0-255), ", lex);
		}
	}

	Expression();
}


void Diagram::Expression() 
{
	type_lex lex;
	int type;

	Comparison();
	type = LookForward(1);
	while (type == typeEq || type == typeUnEq) 
	{
		type = Scan(lex);
		Comparison();
		type = LookForward(1);
	}
}

void Diagram::CompositeOperator() 
{
	type_lex lex;
	int type = Scan(lex);
	Tree* varNode = NULL;
	type_data typeData = tree->GetDataType(type);
	if (type != typeLeftBrace)
	{
		scaner->PrintError("ожидалась {, ", lex);
	}

	Node* newNode = new Node();
	newNode->id = lex;
	newNode->objectType = OBJ_FUNC;
	newNode->dataType = typeData;
	if (varNode != NULL) newNode->pointer = varNode;
	tree->SetLeft(newNode);
	tree = tree->GetLeft();
	tree->SetRight(NULL);
	Tree* tmpTree = tree;
	tree = tree->GetRight();

	OperatorsAndDescriptions();

	type = Scan(lex);
	if (type != typeRightBrace)
	{
		scaner->PrintError("ожидалась }, ", lex);
	}
	tree = tmpTree;
}

void Diagram::OperatorsAndDescriptions() 
{
	type_lex lex;
	int type = LookForward(1);

	while (type != typeRightBrace) 
	{
		if (type == typeInt || type == typeShort || type == typeLong || type == typeChar)
		{
			Data();
		}
		else Operator();
		type = LookForward(1);
	}
}

void Diagram::Operator() 
{
	type_lex lex;
	int type = LookForward(1);

	/*if (type == typeReturn)
	{
		ReturnStatement();
		return;
	}*/

	if (type == typeSemicolon) 
	{
		type = Scan(lex);
		return;
	}

	if (type == typeLeftBrace) 
	{
		CompositeOperator();
		return;
	}

	if (type == typeWhile)
	{
		Cycle();
		return;
	}

	int type2 = LookForward(2);
	if (type == typeId && type2 == typeLeftBracket) 
	{
		FunctionCall();
		return;
	}

	if (type == typeId && type2 == typeEval) 
	{
		Assignment(0);
		type = Scan(lex);
		if (type != typeSemicolon)
			scaner->PrintError("ожидалась ;, ", lex);
		return;
	}

	if (type == typeId) 
	{
		Expression();
		return;
	}

	type = Scan(lex);
	scaner->PrintError("ожидался оператор, ", lex);
}

void Diagram::ReturnStatement() 
{
	type_lex lex;
	int type = Scan(lex);

	/*if (type != typeReturn)
	{
		scaner->PrintError("ожидалось return, ", lex);
	}*/

	Expression();

	type = Scan(lex);
	if (type != typeSemicolon) 
	{
		scaner->PrintError("ожидалась ; после return <выражение>, ", lex);
	}
}

void Diagram::Cycle()
{
	type_lex lex;
	int type = Scan(lex);

	if (type != typeWhile) 
	{
		scaner->PrintError("ожидалось while, ", lex);
	}

	type = Scan(lex);
	if (type != typeLeftBracket) 
	{
		scaner->PrintError("ожидалась (, ", lex);
	}

	Expression();

	type = Scan(lex);
	if (type != typeRightBracket) 
	{
		scaner->PrintError("ожидалась ), ", lex);
	}

	Operator();
}

void Diagram::FunctionCall() 
{
	type_lex lex;
	int type = Scan(lex);

	if (type != typeId)
	{
		scaner->PrintError("ожидался идентификатор, ", lex);
	}

	// Ищем узел, называющийся объектом
	Tree* objectNode = tree->FindUp(lex);
	// Получаем указатель на функцию из функции с тем же именем в родительском узле
	Tree* methodNode = objectNode->FindFunction(lex); // Функция для поиска по имени функции в родительском узле на findUp
	if (methodNode == NULL) {
		scaner->PrintError("Функция не найдена", lex);
		return; // Выход из функции при ошибке
	}

	// Проверяем, что узел является функцией
	if (methodNode->GetSelfObjectType() != OBJ_FUNC) {
		scaner->PrintError("Не является функцией", lex);
	}


	type = Scan(lex);
	if (type != typeLeftBracket)
	{
		scaner->PrintError("ожидалась (, ", lex);
	}

	type = LookForward(1); 
	if (type != typeRightBracket) {
		ArgumentList();
	}

	type = Scan(lex);
	if (type != typeRightBracket)
	{
		scaner->PrintError("ожидалась ), ", lex);
	}

	type = Scan(lex);
	if (type != typeSemicolon)
	{
		scaner->PrintError("ожидалась ;, ", lex);
	}
}

void Diagram::ArgumentList() {
	type_lex lex;
	int type;

	// Обработка первого аргумента
	Expression(); // Аргумент передается как выражение
	type = LookForward(1);

	// Обработка остальных аргументов через запятую
	while (type == typeComma) {
		type = Scan(lex);
		Expression();
		type = LookForward(1);
	}
}

void Diagram::Comparison() 
{
	type_lex lex;
	BitwiseShift();
	int type = LookForward(1);
	while (type == typeLess || type == typeLessOrEq || type == typeMore || type == typeMoreOrEq) 
	{
		type = Scan(lex);
		Tree* node = tree->FindUp(lex);
		BitwiseShift();
		type = LookForward(1);
	}
}

void Diagram::BitwiseShift()
{
	type_lex lex;
	int type;
	Summand();
	type = LookForward(1);
	while (type == typeBitwiseRight || type == typeBitwiseLeft) 
	{
		type = Scan(lex);
		Summand();
		type = LookForward(1);
	}
}

void Diagram::Summand() 
{
	type_lex lex;
	int type;
	Multiplier();
	type = LookForward(1);
	while (type == typePlus || type == typeMinus) 
	{
		type = Scan(lex);
		Multiplier();
		type = LookForward(1);
	}
}

void Diagram::Multiplier() 
{
	type_lex lex;
	int type;
	UnaryOperation();
	type = LookForward(1);
	while (type == typeMul || type == typeDiv || type == typeMod) 
	{
		type = Scan(lex);
		Tree* node = tree->FindUp(lex);
		UnaryOperation();
		type = LookForward(1);
	}
}

void Diagram::UnaryOperation() 
{
	type_lex lex;
	int type = LookForward(1);

	if (type == typePlus || type == typeMinus)
	{
		type = Scan(lex);
		ElementaryExpression();
	}
	else
	{
		ElementaryExpression();
	}
}


void Diagram::ElementaryExpression() 
{
	type_lex lex;
	int type = LookForward(1);
	if (type == typeId) 
	{
		type = Scan(lex);
		Tree* node = tree->FindUp(lex);
		if (node == nullptr)
		{
			scaner->PrintError("Семантическая ошибка. Переменная не найдена", lex);
		}
		if (node != NULL)
			if (node->IsSelfInit() == 0)
			{
				scaner->PrintError("Семантическая ошибка. Переменная не инициализирована", lex);
			}
		return;
	}
	if (type == typeShort || type == typeChar || type == typeInt || type == typeLong || type == typeConst10 || type == typeConst16 || type == constChar) 
	{
		type = Scan(lex);
		return;
	}
	if (type == typeLeftBracket) 
	{
		type = Scan(lex);
		Expression();
		type = Scan(lex);
		if (type != typeRightBracket)
		{
			scaner->PrintError("ожидалась ), ", lex);
		}
		return;
	}
	type = Scan(lex);
	scaner->PrintError("ожидалось выражение, ", lex);
}