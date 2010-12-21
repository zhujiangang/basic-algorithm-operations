#include <stdio.h>
#include <string.h>
#include "DataScanner.h"

const int Symbol::TYPE_INVALID = -1;
const int Symbol::TYPE_DIGITAL = 0;
const int Symbol::TYPE_OPERATOR = 1;

Symbol::Symbol(const char* str, int len) : data(0), type(TYPE_INVALID)
{
	Init(str, 0, len - 1, TYPE_INVALID);
}

Symbol::~Symbol()
{
	if(data != 0)
	{
		delete data;
	}
}

Symbol::Symbol(const Symbol& other) : data(0), type(TYPE_INVALID)
{
	Init(other.data, 0, strlen(other.data) - 1, other.type);
}
Symbol& Symbol::operator=(const Symbol& other)
{
	Init(other.data, 0, strlen(other.data) - 1, other.type);
	return *this;
}

void Symbol::Init(const char* str, int startIndex, int endIndex, int type)
{
	if(data != 0)
	{
		delete data;
		data = 0;
	}
	int len = endIndex - startIndex + 1;
	if(len <= 0)
	{
		return;
	}
	data = new char[len + 1];
	memcpy(data, str + startIndex, len);
	
	data[len] = 0;

	this->type = type;
}

const char* Symbol::GetString()
{
	return data;
}
int Symbol::GetType() const
{
	return type;
}

DataScanner::DataScanner(const char* str) : data(0), Action(0), tokCurs(0), m_symbol(0, 0)
{
	if(str != 0)
	{
		int len = strlen(str);
		data = new char[len + 1];
		strcpy(data, str);

		data[len] = 0;
	}

	int i;
	//Init Action
	Action = new Fct*[MAX_TRANS];
	for(i = 0; i < MAX_TRANS; i++)
	{
//		Action[i] = new Fct[MAX_TRANS];
		Action[i] = (Fct*)new char[MAX_TRANS * 4];
	}

	Action[0][0] = 0;
	Action[0][1] = &DataScanner::RetrieveOperator;
	Action[1][0] = 0;
	Action[1][1] = &DataScanner::RetrieveOperand;
}

DataScanner::~DataScanner()
{
	if(Action != 0)
	{
		int i;
		for(i = 0; i < MAX_TRANS; i++)
		{
			if(Action[i] != 0)
			{
				delete [] Action[i];
				Action[i] = 0;
			}			
		}
		delete [] Action;
		Action = 0;
	}	

	if(data != 0)
	{
		delete data;
		data = 0;
	}
}

void DataScanner::First()
{
	token = data;

	tokOffset = 0;
	tokCurs = 0;
	state = STATE_OPERATOR;

	if(*token != 0)
	{
		Next();
	}
}
void DataScanner::Next()
{
	if(tokCurs == 0 && *token == 0)
	{
		m_symbol.SetType(Symbol::TYPE_INVALID);
		return;
	}
	if(tokCurs > 0)
	{
		m_symbol.Init(tokStack, tokOffset, tokCurs - 1, Symbol::TYPE_OPERATOR);

		tokOffset = 0;
		tokCurs = 0;
		return;
	}
	short trans;
	bool foundSymbol = false;
	for (; *token != 0; token++)
	{
		// does token trigger a transition ?
		for (trans = 0; trans < MAX_TRANS && Grammar[state][trans] != 0; trans++)
		{
			// check if token belongs to the expected tokens list ? 
			// a empty list acts as a wildchar token
			if (strchr(Grammar[state][trans], *token) || *Grammar[state][trans] == 0)
			{
				// push token into the stack
				if (tokCurs < MAX_STACK)
					tokStack[tokCurs++] = *token;

				// execute the action associated to the transition (if any)
				if (Action[state][trans] != 0)
				{
					foundSymbol = (this->*Action[state][trans])();
				}
				// update parser state
				state = Successor[state][trans];
				break;
			}
		}
		if(foundSymbol)
		{
			token++;
			break;
		}
	}

	if(*token == 0 && tokOffset == 0 && tokCurs > 0)
	{
		Retrieve(Symbol::TYPE_DIGITAL);
		state = STATE_INVALID;
	}	
}
bool DataScanner::IsDone()
{
//	return *token == 0 && tokCurs == 0;
	return m_symbol.GetType() == Symbol::TYPE_INVALID;
}
void DataScanner::CurrentItem(Symbol& symbol)
{
	symbol = m_symbol;
}
bool DataScanner::Retrieve(int type)
{
	if(tokStack[tokCurs - 1] == ' ')
	{
		tokCurs--;
	}
	if(tokCurs <= 0)
	{
		return false;
	}
	m_symbol.Init(tokStack, tokOffset, tokCurs - 1, type);
	
	tokOffset = 0;
	tokCurs = 0;
	
	return true;
}
bool DataScanner::RetrieveOperator()
{
	return Retrieve(Symbol::TYPE_OPERATOR);
}
bool DataScanner::RetrieveOperand()
{
	if(tokCurs < 2)
	{
		tokStack[tokCurs] = 0;
		printf("[ERROR]: This should not happen. stack=%s\n", tokStack);
		return false;
	}

	m_symbol.Init(tokStack, tokOffset, tokCurs - 2, Symbol::TYPE_DIGITAL);


	tokOffset = tokCurs - 1;
	if(tokStack[tokCurs - 1] == ' ')
	{
		tokOffset = 0;
		tokCurs = 0;
	}

	return true;
}

char* DataScanner::Grammar[][MAX_TRANS] = 
{
	{"0123456789.", ""},
	{"0123456789.", ""}
};

// next state Ids
char DataScanner::Successor[][MAX_TRANS] = 
{
	{STATE_OPERAND,STATE_OPERATOR},
	{STATE_OPERAND,STATE_OPERATOR}
};