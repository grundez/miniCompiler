// file diagram.cpp

#include "diagram.hpp"

int TDiagram::look_forward(int pos) {
	type_lex lex;
	int saved_pointer = scaner->get_pointer();
	int next_type;
	for (int i = 0; i < pos; i++)
		next_type = scaner->scaner(lex);
	scaner->set_pointer(saved_pointer);
	return next_type;
}

int TDiagram::scan(type_lex lex) {
	return scaner->scaner(lex);
}

void TDiagram::program() {
	type_lex lex;
	int type;
	type = look_forward(1);
	while (type == TVoid || type == TInt || type == TShort || type == TLong || type == TChar) {
		description();
		type = look_forward(1);
	}
	if (type != TEnd) {
		type = scan(lex);
		scaner->print_error("Expected end got", lex);
	}
}

void TDiagram::description() {
	type_lex lex;
	int type;
	type = look_forward(1);
	if (type == TInt || type == TShort || type == TLong || type == TChar) {
		data();
		return;
	}
	if (type == TVoid) {
		function();
		return;
	}
	type = scan(lex);
	scaner->print_error("Expected void or type got", lex);
}

void TDiagram::data() {
	type_lex lex;
	int type_;

	type();

	list();

	type_ = scan(lex);
	if (type_ != TSemicolon)
		scaner->print_error("Expected ; got", lex);
}


//////////////// НЕ РАБОТАЕТ КАК ДОЛЖНО - 
void TDiagram::dataFuncParams(Tree* func_node) {
	type_lex lex;
	int type_;


	type();
	elementInParams();

	TData* param = new TData;
	switch (last_type_data)
	{
	case TYPE_INT: param->type = TYPE_INT; param->value.data_as_int = 0; break;
	case TYPE_SHORT: param->type = TYPE_SHORT; param->value.data_as_short = 0; break;
	case TYPE_LONG: param->type = TYPE_LONG; param->value.data_as_long = 0; break;
	case TYPE_CHAR: param->type = TYPE_CHAR; param->value.data_as_char = ' '; break;
	}

	// ДОБАВЛЕНИЕ ПАРАМЕТРОВ В ВЕКТОР ЛОМАЕТ ПРОГРАММУ !!!!!!!!!!!!!!!!!!!!
	//func_node->get_node()->parameters.push_back(param); 
	func_node->get_node()->param_count++; // Увеличиваю количество объявленных параметров

	type_ = look_forward(1);
	scan(lex);
	while (type_ == TComma) {
		type();
		elementInParams();
		func_node->get_node()->param_count++; // Увеличиваю количество объявленных параметров
		type_ = look_forward(1);
		if (type_ == TComma) {
			 scan(lex);
		}
	}
	//std::cout << look_forward(1);
}


void TDiagram::function() {
	type_lex lex;
	int type, pointer;

	type = scan(lex);
	if (type != TVoid)
		scaner->print_error("Expected void got", lex);

	type = scan(lex);
	if (type != TIdent && type != TMain)
		scaner->print_error("Expected identificator got", lex);

	if (type == TMain)
		root->flag_interp = true;
	else
		root->flag_interp = false;

	Tree* t = root->semantic_include(lex, OBJECT_FUNCTION, TYPE_VOID);
	t->get_node()->param_count = 0; // Изначально 0, будет увеличено при обработке параметров

	type = scan(lex);
	if (type != TLeftBracket)
		scaner->print_error("Expected ( got", lex);

	// Обрабатываем параметры функции
    if (look_forward(1) != TRightBracket) {
        ParameterList(t);
    }

	type = scan(lex);
	if (type != TRightBracket)
		scaner->print_error("Expected ) got", lex);

	t->get_node()->addr = scaner->get_pointer();
	function_body();

	root->set_current(t);
}

void TDiagram::ParameterList(Tree* func_node) {
	type_lex lex;
	int type;

	// Описание параметров функции
	type = look_forward(1);
	if (type != TInt && type != TShort && type != TLong && type != TChar) {
		scaner->print_error("Expected parameter type (int, short, long, char), got", lex);
	}
	else {
		dataFuncParams(func_node); // Передаем также дерево
	}
}



void TDiagram::Parameter() {
	type_lex lex;
	int type = look_forward(1);

	// Проверка типа параметра
	if (type != TInt && type != TShort && type != TLong && type != TChar) {
		scan(lex);
		scaner->print_error("ожидался тип параметра (int, short, long, char), ", lex);
	}

	type = scan(lex); // Считываем идентификатор
	type = look_forward(1);
	if (type != TIdent) {
		scaner->print_error("ожидался идентификатор параметра, ", lex);
	}
	type = scan(lex);
}

void TDiagram::function_body() {
	type_lex lex;
	int type, pointer;

	type = scan(lex);
	if (type != TLeftBrace)
		scaner->print_error("Expected { got", lex);

	operators_and_descriptions();

	type = scan(lex);
	if (type != TRightBrace)
		scaner->print_error("Expected } got", lex);
}

void TDiagram::type() {
	type_lex lex;
	int type;
	type = scan(lex);
	if (type != TInt && type != TShort && type != TLong  && type != TChar)
		scaner->print_error("Expected type (int, short, long, char) got", lex);

	// set last type for semantic analyzer
	if (type == TInt)
		last_type_data = TYPE_INT;
	else if (type == TShort)
		last_type_data = TYPE_SHORT;
	else if (type == TLong)
		last_type_data = TYPE_LONG;
	else if (type == TChar)
		last_type_data = TYPE_CHAR;
	else
		last_type_data = TYPE_UNKNOWN;
}

void TDiagram::list() {
	type_lex lex;
	int type, pointer;

	element();

	type = look_forward(1);

	while (type == TComma) {
		type = scan(lex);
		element();
		type = look_forward(1);
	}
}

void TDiagram::elementInParams() {
	type_lex lex;
	int type;
	type = look_forward(1);
	int type2 = look_forward(2);
	if (type == TIdent && (type2 == TComma || type2 == TRightBracket)) {
		int pointer = scaner->get_pointer();
		type = scan(lex);
		Tree* t = root->semantic_include(lex, OBJECT_VARIABLE, last_type_data);
		scaner->set_pointer(pointer);
		//type = scan(lex);
		return;
	}
	else {
		type = scaner->scaner(lex);
		scaner->print_error("Expected element got", lex);
	}
}

void TDiagram::element() {

	type_lex lex;
	int type;
	type = look_forward(1);
	int type2 = look_forward(2);
	if (type == TIdent && type2) {
		variable();
		return;
	}
	else {
		type = scaner->scaner(lex);
		scaner->print_error("Expected element got", lex);
	}
}

void TDiagram::variableInParams() {
	type_lex lex;
	int type;

	type = look_forward(1);

	if (type != TIdent) {
		type = scan(lex);
		scaner->print_error("Expected identificator got", lex);
	}

	int pointer = scaner->get_pointer();
	type = scan(lex);
	Tree* t = root->semantic_include(lex, OBJECT_VARIABLE, last_type_data);
	scaner->set_pointer(pointer);

	type = look_forward(2);
	if (type == TComma) {
		assignment();
		root->semantic_set_init(t, 1);
		return;
	}
	type = scan(lex);
}

void TDiagram::variable() {
	type_lex lex;
	int type;

	type = look_forward(1);

	if (type != TIdent) {
		type = scan(lex);
		scaner->print_error("Expected identificator got", lex);
	}

	int pointer = scaner->get_pointer();
	type = scan(lex);
	Tree* t = root->semantic_include(lex, OBJECT_VARIABLE, last_type_data);
	scaner->set_pointer(pointer);

	type = look_forward(2);
	if (type == TEval) {
		assignment();
		root->semantic_set_init(t, 1);
		return;
	}
	type = scan(lex);
}





void TDiagram::assignment() {
	type_lex lex;
	int type;

	type = scan(lex);
	if (type != TIdent) {
		scaner->print_error("Expected identificator got", lex);
	}

	Tree* t = root->semantic_get_type(lex, OBJECT_VARIABLE);
	TData* data = &t->get_node()->data;
	root->semantic_set_init(t, 1);

	type = scan(lex);
	if (type != TEval)
		scaner->print_error("Expected = got", lex);

	TData* expression_data = new TData;
	expression(expression_data);

	switch (data->type) {
	case TYPE_INT:
		switch (expression_data->type) {
		case TYPE_INT: data->value.data_as_int = expression_data->value.data_as_int; break;
		case TYPE_SHORT: data->value.data_as_int = expression_data->value.data_as_short; break;
		case TYPE_LONG: data->value.data_as_int = expression_data->value.data_as_long; scaner->print_warning("Warning: Conversion from long to int", ""); break;		
		case TYPE_CHAR: data->value.data_as_int = expression_data->value.data_as_char; scaner->print_warning("Warning: Conversion from char to int", ""); break;
		}
		break;
	case TYPE_SHORT:
		switch (expression_data->type) {
		case TYPE_INT: data->value.data_as_short = expression_data->value.data_as_int; //scaner->print_warning("Warning: Conversion from int to short", ""); 
			break;
		case TYPE_SHORT: data->value.data_as_short = expression_data->value.data_as_short; break;
		case TYPE_LONG: data->value.data_as_short = expression_data->value.data_as_long; scaner->print_warning("Warning: Conversion from long to short", ""); break;		
		case TYPE_CHAR: data->value.data_as_short = expression_data->value.data_as_char; break;
		}
		break;
	case TYPE_LONG:
		switch (expression_data->type) {
		case TYPE_INT: data->value.data_as_long = expression_data->value.data_as_int; break;
		case TYPE_SHORT: data->value.data_as_long = expression_data->value.data_as_short; break;
		case TYPE_LONG: data->value.data_as_long = expression_data->value.data_as_long; break;		
		case TYPE_CHAR: data->value.data_as_long = expression_data->value.data_as_char; break;
		}
		break;
	
	case TYPE_CHAR:
		switch (expression_data->type) {
		case TYPE_INT: 
			if (expression_data->value.data_as_int > -1 && expression_data->value.data_as_int < 256) {
				data->value.data_as_char = expression_data->value.data_as_int;
				scaner->print_warning("Warning: Conversion from int to char", "");
			}
			else {
				scaner->print_warning("Warning: overflow in conversion from int to char changes value from 0-255. Possible data loss.", "");
				exit(EXIT_FAILURE); // Останавливаем программу
			}
			break;
		case TYPE_SHORT: data->value.data_as_char = expression_data->value.data_as_short; scaner->print_warning("Warning: Conversion from short to char", ""); break;
		case TYPE_LONG: data->value.data_as_char = expression_data->value.data_as_long; scaner->print_warning("Warning: Conversion from long to char", ""); break;		
		case TYPE_CHAR: data->value.data_as_char = expression_data->value.data_as_char; break;
		}
		break;
	}
	print_variable(t);
}



void TDiagram::composite_operator() {
	type_lex lex;
	int type;

	type = scan(lex);
	if (type != TLeftBrace)
		scaner->print_error("Expected { got", lex);

	Tree* t = root->semantic_include(lex, OBJECT_UNKNOWN, TYPE_UNKNOWN);

	operators_and_descriptions();

	type = scan(lex);
	if (type != TRightBrace)
		scaner->print_error("Expected } got", lex);

	root->set_current(t);
}

void TDiagram::operators_and_descriptions() {
	type_lex lex;
	int type;

	type = look_forward(1);
	while (type != TRightBrace) {
		if (type == TInt || type == TShort || type == TLong || type == TChar) {
			data();
		}
		else operator_();
		type = look_forward(1);

	}
}

void TDiagram::cycle()
{
	type_lex lex;
	int type = scan(lex);

	if (type != TWhile)
	{
		scaner->print_error("ожидалось while, ", lex);
	}

	type = scan(lex);
	if (type != TLeftBracket)
	{
		scaner->print_error("ожидалась (, ", lex);
	}

	TData* expression_data = new TData;
	expression(expression_data);

	type = scan(lex);
	if (type != TRightBracket)
	{
		scaner->print_error("ожидалась ), ", lex);
	}

	operator_();
}

void TDiagram::operator_() {
	type_lex lex;
	int type;

	type = look_forward(1);
	int type2 = look_forward(2);

	if (type == TIdent && type2 == TEval) {

		assignment();
		type = scan(lex);
		if (type != TSemicolon)
			scaner->print_error("Expected ; got", lex);
		return;
	}

	if (type == TSemicolon) {
		type = scan(lex);
		return;
	}

	if (type == TWhile) {
		condition();
		return;
	}

	if (type == TLeftBrace) {
		composite_operator();
		return;
	}

	if (type == TIdent && type2 == TLeftBracket) {
		function_call();
		return;
	}

	type = scan(lex);
	scaner->print_error("Expected operator got", lex);
}

void TDiagram::function_call() {
	type_lex lex;
	int type;

	// Чтение идентификатора функции
	type = scan(lex);
	if (type != TIdent)
		scaner->print_error("Expected identificator got", lex);

	Tree* t = root->semantic_get_type(lex, OBJECT_FUNCTION);

	// Проверка на открывающую скобку
	type = scan(lex);
	if (type != TLeftBracket)
		scaner->print_error("Expected ( got", lex);

	int param_count = 0;

	// Если есть параметры, обрабатываем их
	type = look_forward(1);
	if (type != TRightBracket) { // Если параметры есть
		do {
			TData* expression_data = new TData;
			expression(expression_data);  // Обработка выражения для каждого параметра
			param_count++;  // Увеличиваем счетчик параметров

			type = look_forward(1);
			if (type == TComma)
				type = scan(lex);  // Если есть запятая, пропускаем ее
		} while (type != TRightBracket);  // Пока не встретится закрывающая скобка
	}

	// Проверка на правильность количества параметров
	int declared_param_count = t->get_node()->param_count;
	if (param_count != declared_param_count) {
		scaner->print_error("Incorrect number of parameters.", "");
		exit(EXIT_FAILURE);
	}

	// Чтение закрывающей скобки
	type = scan(lex);
	if (type != TRightBracket)
		scaner->print_error("Expected ) got", lex);

	// Ожидаем точку с запятой в конце вызова функции
	type = scan(lex);
	if (type != TSemicolon)
		scaner->print_error("Expected ; got", lex);

	
	if (root->flag_interp) {
		Tree* current = root->get_current();
		root->set_current(t);
		int pointer = scaner->get_pointer();
		scaner->set_pointer(t->get_node()->addr);
		function_body();
		root->set_current(current);
		scaner->set_pointer(pointer);
	}
	
	
}

void TDiagram::condition() {

	type_lex lex;
	int type;
	bool local_flag_interp = root->flag_interp;

	type = scan(lex);
	if (type != TWhile)
		scaner->print_error("Expected while got", lex);

	type = scan(lex);
	if (type != TLeftBracket)
		scaner->print_error("Expected ( got", lex);

	int scanPointerExpression = scaner->get_pointer();
	TData* expression_data = new TData;

	// Если интерпретация вкл 
	if (local_flag_interp) {
		expression(expression_data);
	}
	// Если нет, пропускаем оператор while до }
	else {
		type = look_forward(1);
		while (type != TRightBracket) {
			type = scan(lex);
		}

		type = look_forward(1);
		if (type == TLeftBrace) {
			while (type != TRightBrace) {
				type = scan(lex);
			}
		}
		else {
			type = look_forward(1);
			while (type != TRightBrace) {
				type = scan(lex);
				type = look_forward(1);
			}
		}
		return;
	}

	int helpPointer = scaner->get_pointer();
	while (type != TRightBrace) {
		type = scan(lex);
	}
	int endWhilePointer = scaner->get_pointer();
	scaner->set_pointer(helpPointer);

	type = scan(lex);
	if (type != TRightBracket)
		scaner->print_error("Expected ) got", lex);

	int scanPointer = scaner->get_pointer();
	bool expression_value = false;
	do {
		switch (expression_data->type) {
		case TYPE_INT: expression_value = expression_data->value.data_as_int; break;
		case TYPE_SHORT: expression_value = expression_data->value.data_as_short; break;
		case TYPE_LONG: expression_value = expression_data->value.data_as_long; break;
		case TYPE_CHAR: expression_value = expression_data->value.data_as_char; break;
		}

		if (root->flag_interp && expression_value) {
			root->flag_interp = true;

			scaner->set_pointer(scanPointer);
			int type_ = look_forward(1);
			operator_();
			endWhilePointer = scaner->get_pointer();

			scaner->set_pointer(scanPointerExpression);  // Перемещаем обратно перед проверкой условия
			expression(expression_data);
		}
		else {
			scaner->set_pointer(endWhilePointer);
			root->flag_interp = false;
			break;
		}
	} while (root->flag_interp);

	root->flag_interp = local_flag_interp;
}

void TDiagram::expression(TData* data) {
	type_lex lex;
	int type;
	comparison(data);

	type = look_forward(1);
	while (type == TEq || type == TNe) {
		int operation = scan(lex);
		TData* expression_data = new TData;
		comparison(expression_data);
		type = look_forward(1);
		switch (data->type) {
		case TYPE_INT:
			data->type = TYPE_CHAR;
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TEq)
					data->value.data_as_int == expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_int != expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			case TYPE_SHORT:
				if (operation == TEq)
					data->value.data_as_int == expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_int != expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			case TYPE_LONG:
				if (operation == TEq)
					data->value.data_as_int == expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_int != expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			
			case TYPE_CHAR:
				if (operation == TEq)
					data->value.data_as_int == expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_int != expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			}
			break;
		case TYPE_SHORT:
			data->type = TYPE_CHAR;
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TEq)
					data->value.data_as_short == expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_short != expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			case TYPE_SHORT:
				if (operation == TEq)
					data->value.data_as_short == expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_short != expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			case TYPE_LONG:
				if (operation == TEq)
					data->value.data_as_short == expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_short != expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			
			case TYPE_CHAR:
				if (operation == TEq)
					data->value.data_as_short == expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_short != expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			}
			break;
		case TYPE_LONG:
			data->type = TYPE_CHAR;
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TEq)
					data->value.data_as_long == expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_long != expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			case TYPE_SHORT:
				if (operation == TEq)
					data->value.data_as_long == expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_long != expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			case TYPE_LONG:
				if (operation == TEq)
					data->value.data_as_long == expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_long != expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			
			case TYPE_CHAR:
				if (operation == TEq)
					data->value.data_as_long == expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_long != expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			}
			break;
		
		case TYPE_CHAR:
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TEq)
					data->value.data_as_char == expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_char != expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			case TYPE_SHORT:
				if (operation == TEq)
					data->value.data_as_char == expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_char != expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			case TYPE_LONG:
				if (operation == TEq)
					data->value.data_as_char == expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_char != expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			
			case TYPE_CHAR:
				if (operation == TEq)
					data->value.data_as_char == expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TNe)
					data->value.data_as_char != expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			}
			break;
		}
	}
}

void TDiagram::comparison(TData* data) {
	type_lex lex;
	int type;
	BitwiseShift(data);
	
	type = look_forward(1);
	while (type == TLt || type == TLe || type == TGt || type == TGe) {
		int operation = scan(lex);
		TData* expression_data = new TData;
		addendum(expression_data);
		type = look_forward(1);

		switch(data->type) {
		case TYPE_INT:
			data->type = TYPE_CHAR;
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TLt)
					data->value.data_as_int < expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TLe)
					data->value.data_as_int <= expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TGt)
					data->value.data_as_int > expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TGe)
					data->value.data_as_int >= expression_data->value.data_as_int ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			case TYPE_SHORT:
				if (operation == TLt)
					data->value.data_as_int < expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TLe)
					data->value.data_as_int <= expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TGt)
					data->value.data_as_int > expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TGe)
					data->value.data_as_int >= expression_data->value.data_as_short ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			case TYPE_LONG:
				if (operation == TLt)
					data->value.data_as_int < expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TLe)
					data->value.data_as_int <= expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TGt)
					data->value.data_as_int > expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TGe)
					data->value.data_as_int >= expression_data->value.data_as_long ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			
			case TYPE_CHAR:
				if (operation == TLt)
					data->value.data_as_int < expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TLe)
					data->value.data_as_int <= expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TGt)
					data->value.data_as_int > expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				else if (operation == TGe)
					data->value.data_as_int >= expression_data->value.data_as_char ? data->value.data_as_int = 1 : data->value.data_as_int = 0;
				break;
			}
			break;
		case TYPE_SHORT:
			data->type = TYPE_CHAR;
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TLt)
					data->value.data_as_short < expression_data->value.data_as_int ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TLe)
					data->value.data_as_short <= expression_data->value.data_as_int ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TGt)
					data->value.data_as_short > expression_data->value.data_as_int ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TGe)
					data->value.data_as_short >= expression_data->value.data_as_int ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				break;
			case TYPE_SHORT:
				if (operation == TLt)
					data->value.data_as_short < expression_data->value.data_as_short ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TLe)
					data->value.data_as_short <= expression_data->value.data_as_short ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TGt)
					data->value.data_as_short > expression_data->value.data_as_short ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TGe)
					data->value.data_as_short >= expression_data->value.data_as_short ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				break;
			case TYPE_LONG:
				if (operation == TLt)
					data->value.data_as_short < expression_data->value.data_as_long ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TLe)
					data->value.data_as_short <= expression_data->value.data_as_long ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TGt)
					data->value.data_as_short > expression_data->value.data_as_long ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TGe)
					data->value.data_as_short >= expression_data->value.data_as_long ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				break;
			
			case TYPE_CHAR:
				if (operation == TLt)
					data->value.data_as_short < expression_data->value.data_as_char ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TLe)
					data->value.data_as_short <= expression_data->value.data_as_char ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TGt)
					data->value.data_as_short > expression_data->value.data_as_char ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				else if (operation == TGe)
					data->value.data_as_short >= expression_data->value.data_as_char ? data->value.data_as_short = 1 : data->value.data_as_short = 0;
				break;
			}
			break;
		case TYPE_LONG:
			data->type = TYPE_CHAR;
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TLt)
					data->value.data_as_long < expression_data->value.data_as_int ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TLe)
					data->value.data_as_long <= expression_data->value.data_as_int ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TGt)
					data->value.data_as_long > expression_data->value.data_as_int ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TGe)
					data->value.data_as_long >= expression_data->value.data_as_int ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				break;
			case TYPE_SHORT:
				if (operation == TLt)
					data->value.data_as_long < expression_data->value.data_as_short ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TLe)
					data->value.data_as_long <= expression_data->value.data_as_short ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TGt)
					data->value.data_as_long > expression_data->value.data_as_short ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TGe)
					data->value.data_as_long >= expression_data->value.data_as_short ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				break;
			case TYPE_LONG:
				if (operation == TLt)
					data->value.data_as_long < expression_data->value.data_as_long ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TLe)
					data->value.data_as_long <= expression_data->value.data_as_long ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TGt)
					data->value.data_as_long > expression_data->value.data_as_long ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TGe)
					data->value.data_as_long >= expression_data->value.data_as_long ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				break;
			
			case TYPE_CHAR:
				if (operation == TLt)
					data->value.data_as_long < expression_data->value.data_as_char ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TLe)
					data->value.data_as_long <= expression_data->value.data_as_char ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TGt)
					data->value.data_as_long > expression_data->value.data_as_char ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				else if (operation == TGe)
					data->value.data_as_long >= expression_data->value.data_as_char ? data->value.data_as_long = 1 : data->value.data_as_long = 0;
				break;
			}
			break;
		
		case TYPE_CHAR:
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TLt)
					data->value.data_as_char < expression_data->value.data_as_int ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				else if (operation == TLe)
					data->value.data_as_char <= expression_data->value.data_as_int ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				else if (operation == TGt)
					data->value.data_as_char > expression_data->value.data_as_int ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				else if (operation == TGe)
					data->value.data_as_char >= expression_data->value.data_as_int ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				break;
			case TYPE_SHORT:
				if (operation == TLt)
					data->value.data_as_char < expression_data->value.data_as_short ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				else if (operation == TLe)
					data->value.data_as_char <= expression_data->value.data_as_short ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				else if (operation == TGt)
					data->value.data_as_char > expression_data->value.data_as_short ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				else if (operation == TGe)
					data->value.data_as_char >= expression_data->value.data_as_short ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				break;
			case TYPE_LONG:
				if (operation == TLt)
					data->value.data_as_char < expression_data->value.data_as_long ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				else if (operation == TLe)
					data->value.data_as_char <= expression_data->value.data_as_long ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				else if (operation == TGt)
					data->value.data_as_char > expression_data->value.data_as_long ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				else if (operation == TGe)
					data->value.data_as_char >= expression_data->value.data_as_long ? data->value.data_as_char = 1 : data->value.data_as_char = 0;
				break;
			}
			break;
		}
	}
}

void TDiagram::BitwiseShift(TData* data) {
	type_lex lex;
	int type;

	// Выполнить начальную операцию (например, вызов addendum)
	addendum(data);

	type = look_forward(1);
	while (type == TLeftShift || type == TRightShift) {
		int operation = scan(lex);
		TData* expression_data = new TData;
		addendum(expression_data);
		type = look_forward(1);

		switch (data->type) {
		case TYPE_INT:
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TLeftShift) {
					data->value.data_as_int <<= expression_data->value.data_as_int;
				}
				else if (operation == TRightShift) {
					data->value.data_as_int >>= expression_data->value.data_as_int;
				}
				break;
			case TYPE_SHORT:
				if (operation == TLeftShift) {
					data->value.data_as_int <<= expression_data->value.data_as_short;
				}
				else if (operation == TRightShift) {
					data->value.data_as_int >>= expression_data->value.data_as_short;
				}
				break;
			case TYPE_LONG:
				if (operation == TLeftShift) {
					data->value.data_as_int <<= expression_data->value.data_as_long;
				}
				else if (operation == TRightShift) {
					data->value.data_as_int >>= expression_data->value.data_as_long;
				}
				break;
			case TYPE_CHAR:
				if (operation == TLeftShift) {
					data->value.data_as_int <<= expression_data->value.data_as_char;
				}
				else if (operation == TRightShift) {
					data->value.data_as_int >>= expression_data->value.data_as_char;
				}
				break;
			}
			break;

		case TYPE_SHORT:
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TLeftShift) {
					data->value.data_as_short <<= expression_data->value.data_as_int;
				}
				else if (operation == TRightShift) {
					data->value.data_as_short >>= expression_data->value.data_as_int;
				}
				break;
			case TYPE_SHORT:
				if (operation == TLeftShift) {
					data->value.data_as_short <<= expression_data->value.data_as_short;
				}
				else if (operation == TRightShift) {
					data->value.data_as_short >>= expression_data->value.data_as_short;
				}
				break;
			case TYPE_LONG:
				if (operation == TLeftShift) {
					data->value.data_as_short <<= expression_data->value.data_as_long;
				}
				else if (operation == TRightShift) {
					data->value.data_as_short >>= expression_data->value.data_as_long;
				}
				break;
			case TYPE_CHAR:
				if (operation == TLeftShift) {
					data->value.data_as_short <<= expression_data->value.data_as_char;
				}
				else if (operation == TRightShift) {
					data->value.data_as_short >>= expression_data->value.data_as_char;
				}
				break;
			}
			break;

		case TYPE_LONG:
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TLeftShift) {
					data->value.data_as_long <<= expression_data->value.data_as_int;
				}
				else if (operation == TRightShift) {
					data->value.data_as_long >>= expression_data->value.data_as_int;
				}
				break;
			case TYPE_SHORT:
				if (operation == TLeftShift) {
					data->value.data_as_long <<= expression_data->value.data_as_short;
				}
				else if (operation == TRightShift) {
					data->value.data_as_long >>= expression_data->value.data_as_short;
				}
				break;
			case TYPE_LONG:
				if (operation == TLeftShift) {
					data->value.data_as_long <<= expression_data->value.data_as_long;
				}
				else if (operation == TRightShift) {
					data->value.data_as_long >>= expression_data->value.data_as_long;
				}
				break;
			case TYPE_CHAR:
				if (operation == TLeftShift) {
					data->value.data_as_long <<= expression_data->value.data_as_char;
				}
				else if (operation == TRightShift) {
					data->value.data_as_long >>= expression_data->value.data_as_char;
				}
				break;
			}
			break;

		case TYPE_CHAR:
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TLeftShift) {
					data->value.data_as_char <<= expression_data->value.data_as_int;
				}
				else if (operation == TRightShift) {
					data->value.data_as_char >>= expression_data->value.data_as_int;
				}
				break;
			case TYPE_SHORT:
				if (operation == TLeftShift) {
					data->value.data_as_char <<= expression_data->value.data_as_short;
				}
				else if (operation == TRightShift) {
					data->value.data_as_char >>= expression_data->value.data_as_short;
				}
				break;
			case TYPE_LONG:
				if (operation == TLeftShift) {
					data->value.data_as_char <<= expression_data->value.data_as_long;
				}
				else if (operation == TRightShift) {
					data->value.data_as_char >>= expression_data->value.data_as_long;
				}
				break;
			case TYPE_CHAR:
				if (operation == TLeftShift) {
					data->value.data_as_char <<= expression_data->value.data_as_char;
				}
				else if (operation == TRightShift) {
					data->value.data_as_char >>= expression_data->value.data_as_char;
				}
				break;
			}
			break;
		}
	}
}


void TDiagram::addendum(TData* data) {
	type_lex lex;
	int type;

	multiplier(data);

	type = look_forward(1);
	while (type == TAdd || type == TSub) {
		int operation = scan(lex);
		TData* expression_data = new TData;
		multiplier(expression_data);
		type = look_forward(1);

		switch (data->type) {
		case TYPE_INT:
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TAdd)
					data->value.data_as_int += expression_data->value.data_as_int; 
				else if (operation == TSub)
					data->value.data_as_int -= expression_data->value.data_as_int;
				break;
			case TYPE_SHORT:
				if (operation == TAdd)
					data->value.data_as_int += expression_data->value.data_as_short;
				else if (operation == TSub)
					data->value.data_as_int -= expression_data->value.data_as_short;
				break;
			case TYPE_LONG:
				data->type = TYPE_LONG;
				if (operation == TAdd)
					data->value.data_as_int += expression_data->value.data_as_long;
				else if (operation == TSub)
					data->value.data_as_int -= expression_data->value.data_as_long;
				break;
			
			case TYPE_CHAR:
				if (operation == TAdd)
					data->value.data_as_int += expression_data->value.data_as_char;
				else if (operation == TSub)
					data->value.data_as_int -= expression_data->value.data_as_char;
				break;
			}
			break;
		case TYPE_SHORT:
			switch (expression_data->type) {
			case TYPE_INT:
				data->type = TYPE_INT;
				if (operation == TAdd)
					data->value.data_as_short += expression_data->value.data_as_int;
				else if (operation == TSub)
					data->value.data_as_short -= expression_data->value.data_as_int;
				break;
			case TYPE_SHORT:
				if (operation == TAdd)
					data->value.data_as_short += expression_data->value.data_as_short;
				else if (operation == TSub)
					data->value.data_as_short -= expression_data->value.data_as_short;
				break;
			case TYPE_LONG:
				data->type = TYPE_LONG;
				if (operation == TAdd)
					data->value.data_as_short += expression_data->value.data_as_long;
				else if (operation == TSub)
					data->value.data_as_short -= expression_data->value.data_as_long;
				break;
			
			case TYPE_CHAR:
				if (operation == TAdd)
					data->value.data_as_short += expression_data->value.data_as_char;
				else if (operation == TSub)
					data->value.data_as_short -= expression_data->value.data_as_char;
				break;
			}
			break;
		case TYPE_LONG:
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TAdd)
					data->value.data_as_long += expression_data->value.data_as_int;
				else if (operation == TSub)
					data->value.data_as_long -= expression_data->value.data_as_int;
				break;
			case TYPE_SHORT:
				if (operation == TAdd)
					data->value.data_as_long += expression_data->value.data_as_short;
				else if (operation == TSub)
					data->value.data_as_long -= expression_data->value.data_as_short;
				break;
			case TYPE_LONG:
				if (operation == TAdd)
					data->value.data_as_long += expression_data->value.data_as_long;
				else if (operation == TSub)
					data->value.data_as_long -= expression_data->value.data_as_long;
				break;
			
			case TYPE_CHAR:
				if (operation == TAdd)
					data->value.data_as_long += expression_data->value.data_as_char;
				else if (operation == TSub)
					data->value.data_as_long -= expression_data->value.data_as_char;
				break;
			}
			break;
		
		case TYPE_CHAR: 
			switch (expression_data->type) {
			case TYPE_INT:
				data->type = TYPE_INT;
				if (operation == TAdd)
					data->value.data_as_char += expression_data->value.data_as_int;
				else if (operation == TSub)
					data->value.data_as_char -= expression_data->value.data_as_int;
				break;
			case TYPE_SHORT:
				data->type = TYPE_SHORT;
				if (operation == TAdd)
					data->value.data_as_char += expression_data->value.data_as_short;
				else if (operation == TSub)
					data->value.data_as_char -= expression_data->value.data_as_short;
				break;
			case TYPE_LONG:
				data->type = TYPE_LONG;
				if (operation == TAdd)
					data->value.data_as_char += expression_data->value.data_as_long;
				else if (operation == TSub)
					data->value.data_as_char -= expression_data->value.data_as_long;
				break;
			
			case TYPE_CHAR:
				if (operation == TAdd)
					data->value.data_as_char += expression_data->value.data_as_char;
				else if (operation == TSub)
					data->value.data_as_char -= expression_data->value.data_as_char;
				break;
			}
			break;
		}
	}
}

void TDiagram::multiplier(TData* data) {
	type_lex lex;
	int type;

	unary_operation(data);

	type = look_forward(1);
	while (type == TMul || type == TDiv || type == TMod) {
		int operation = scan(lex);
		TData* expression_data = new TData;
		unary_operation(expression_data);
		if (
			(expression_data->value.data_as_int == 0 && (operation == TDiv || operation == TMod) || 
			expression_data->value.data_as_short == 0 && (operation == TDiv || operation == TMod) ||
			expression_data->value.data_as_long == 0 && (operation == TDiv || operation == TMod) ||
			expression_data->value.data_as_char == 0 && (operation == TDiv || operation == TMod)) && root->flag_interp)
			scaner->print_error("Division by zero", "");
		type = look_forward(1);

		switch (data->type) {
		case TYPE_INT:
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TMul)
					data->value.data_as_int *= expression_data->value.data_as_int;
				else if (operation == TDiv)
					data->value.data_as_int /= expression_data->value.data_as_int;
				else if (operation == TMod)
					data->value.data_as_int %= expression_data->value.data_as_int;
				break;
			case TYPE_SHORT:
				if (operation == TMul)
					data->value.data_as_int *= expression_data->value.data_as_short;
				else if (operation == TDiv)
					data->value.data_as_int /= expression_data->value.data_as_short;
				else if (operation == TMod)
					data->value.data_as_int %= expression_data->value.data_as_short;
				break;
			case TYPE_LONG:
				data->type = TYPE_LONG;
				if (operation == TMul)
					data->value.data_as_int *= expression_data->value.data_as_long;
				else if (operation == TDiv)
					data->value.data_as_int /= expression_data->value.data_as_long;
				else if (operation == TMod)
					data->value.data_as_int %= expression_data->value.data_as_long;
				break;
			
			case TYPE_CHAR:
				if (operation == TMul)
					data->value.data_as_int *= expression_data->value.data_as_char;
				else if (operation == TDiv)
					data->value.data_as_int /= expression_data->value.data_as_char;
				else if (operation == TMod)
					data->value.data_as_int %= expression_data->value.data_as_char;
				break;
			}
			break;
		case TYPE_SHORT:
			switch (expression_data->type) {
			case TYPE_INT:
				data->type = TYPE_INT;
				if (operation == TMul)
					data->value.data_as_short *= expression_data->value.data_as_int;
				else if (operation == TDiv)
					data->value.data_as_short /= expression_data->value.data_as_int;
				else if (operation == TMod)
					data->value.data_as_short %= expression_data->value.data_as_int;
				break;
			case TYPE_SHORT:
				if (operation == TMul)
					data->value.data_as_short *= expression_data->value.data_as_short;
				else if (operation == TDiv)
					data->value.data_as_short /= expression_data->value.data_as_short;
				else if (operation == TMod)
					data->value.data_as_short %= expression_data->value.data_as_short;
				break;
			case TYPE_LONG:
				data->type = TYPE_LONG;
				if (operation == TMul)
					data->value.data_as_short *= expression_data->value.data_as_long;
				else if (operation == TDiv)
					data->value.data_as_short /= expression_data->value.data_as_long;
				else if (operation == TMod)
					data->value.data_as_short %= expression_data->value.data_as_long;
				break;
			
			case TYPE_CHAR:
				if (operation == TMul)
					data->value.data_as_short *= expression_data->value.data_as_char;
				else if (operation == TDiv)
					data->value.data_as_short /= expression_data->value.data_as_char;
				else if (operation == TMod)
					data->value.data_as_short %= expression_data->value.data_as_char;
				break;
			}
			break;
		case TYPE_LONG:
			switch (expression_data->type) {
			case TYPE_INT:
				if (operation == TMul)
					data->value.data_as_long *= expression_data->value.data_as_int;
				else if (operation == TDiv)
					data->value.data_as_long /= expression_data->value.data_as_int;
				else if (operation == TMod)
					data->value.data_as_long %= expression_data->value.data_as_int;
				break;
			case TYPE_SHORT:
				if (operation == TMul)
					data->value.data_as_long *= expression_data->value.data_as_short;
				else if (operation == TDiv)
					data->value.data_as_long /= expression_data->value.data_as_short;
				else if (operation == TMod)
					data->value.data_as_long %= expression_data->value.data_as_short;
				break;
			case TYPE_LONG:
				if (operation == TMul)
					data->value.data_as_long *= expression_data->value.data_as_long;
				else if (operation == TDiv)
					data->value.data_as_long /= expression_data->value.data_as_long;
				else if (operation == TMod)
					data->value.data_as_long %= expression_data->value.data_as_long;
				break;
			
			case TYPE_CHAR:
				if (operation == TMul)
					data->value.data_as_long *= expression_data->value.data_as_char;
				else if (operation == TDiv)
					data->value.data_as_long /= expression_data->value.data_as_char;
				else if (operation == TMod)
					data->value.data_as_long %= expression_data->value.data_as_char;
				break;
			}
			break;
		
		case TYPE_CHAR:
			switch (expression_data->type) {
			case TYPE_INT:
				data->type = TYPE_INT;
				if (operation == TMul)
					data->value.data_as_char *= expression_data->value.data_as_int;
				else if (operation == TDiv)
					data->value.data_as_char /= expression_data->value.data_as_int;
				else if (operation == TMod)
					data->value.data_as_char %= expression_data->value.data_as_int;
				break;
			case TYPE_SHORT:
				data->type = TYPE_SHORT;
				if (operation == TMul)
					data->value.data_as_char *= expression_data->value.data_as_short;
				else if (operation == TDiv)
					data->value.data_as_char /= expression_data->value.data_as_short;
				else if (operation == TMod)
					data->value.data_as_char %= expression_data->value.data_as_short;
				break;
			case TYPE_LONG:
				data->type = TYPE_LONG;
				if (operation == TMul)
					data->value.data_as_char *= expression_data->value.data_as_long;
				else if (operation == TDiv)
					data->value.data_as_char /= expression_data->value.data_as_long;
				else if (operation == TMod)
					data->value.data_as_char %= expression_data->value.data_as_long;
				break;
			
			case TYPE_CHAR:
				if (operation == TMul)
					data->value.data_as_char *= expression_data->value.data_as_char;
				else if (operation == TDiv)
					data->value.data_as_char /= expression_data->value.data_as_char;
				else if (operation == TMod)
					data->value.data_as_char %= expression_data->value.data_as_char;
				break;
			}
			break;
		}
	}
}

void TDiagram::unary_operation(TData* data) {
	type_lex lex;
	int type = look_forward(1);

	if (type == TAdd)
	{
		type = scan(lex);
		elementary_expression(data);
	}
	else if (type == TSub)
	{
		type = scan(lex);
		elementary_expression(data);
		switch (data->type) {
			case TYPE_INT: data->value.data_as_int *= -1; break;
			case TYPE_SHORT: data->value.data_as_short *= -1; break;
			case TYPE_LONG: data->value.data_as_long *= -1; break;
			case TYPE_CHAR: data->value.data_as_char *= -1; break;
		}
	}
	else
		elementary_expression(data);
}


void TDiagram::elementary_expression(TData* data) {
	type_lex lex;
	int type = look_forward(1);
	if (type == TIdent) {
		type = scan(lex);
		int type = look_forward(1);
		Tree* t = root->semantic_get_type(lex, OBJECT_VARIABLE);
		if (t->get_node()->init != 1)
			scaner->print_error("Variable not initialized", lex);
		if (root->flag_interp == false) return;
		data->type = t->get_node()->data.type;
		data->value = t->get_node()->data.value;
		return;
	}
	else if (type == TConst10 || type == TConst16) {
		type = scan(lex);
		if (root->flag_interp == false) return;
		data->type = TYPE_INT;
		data->value.data_as_int = strtoll(lex, NULL, 0);
		return;
	}
	else if (type == TConstChar) {
		type = scan(lex);
		if (root->flag_interp == false) return;
		data->type = TYPE_CHAR;
		// Проверка, что строка содержит символ в формате 'a'
		if (lex[0] == '\'' && lex[2] == '\'' && strlen(lex) == 3) {
			data->value.data_as_char = lex[1]; // Извлекаем символ между кавычками
		}
		else {
			scaner->print_error("Invalid character constant format", lex);
			return;
		}
		return;
	}
	else if (type == TLeftBracket) {
		type = scan(lex);
		expression(data);
		type = scan(lex);
		if (type != TRightBracket)
			scaner->print_error("Expected ) got ", lex);
		return;
	}
	else {
		type = scan(lex);
		scaner->print_error("Expected expression got", lex);
	}
}


void TDiagram::print_variable(Tree* t) {
	switch(t->get_node()->data.type) {
	case TYPE_INT: std::cout << t->get_node()->lex << ", data: " << t->get_node()->data.value.data_as_int << " - type of int" << std::endl; break;
	case TYPE_SHORT: std::cout << t->get_node()->lex << ", data: " << t->get_node()->data.value.data_as_short << " - type of short" << std::endl; break;
	case TYPE_LONG: std::cout << t->get_node()->lex << ", data: " << t->get_node()->data.value.data_as_long << " - type of long" << std::endl; break;
	case TYPE_CHAR: std::cout << t->get_node()->lex << ", data: " << t->get_node()->data.value.data_as_char << " - type of char" << std::endl; break;
	}
}