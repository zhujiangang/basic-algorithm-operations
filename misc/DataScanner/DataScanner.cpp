#include <stdio.h>
#include <string.h>
#include "DataScanner.h"

Symbol::Symbol(const char* str, int len) : data(0)
{
	Init(str, len);
}

Symbol::~Symbol()
{
	if(data != 0)
	{
		delete data;
	}
}

Symbol::Symbol(const Symbol& other)
{
	Init(other.data, strlen(other.data));
}
Symbol& Symbol::operator=(const Symbol& other)
{
	Init(other.data, strlen(other.data));
	return *this;
}

void Symbol::Init(const char* str, int len)
{
	Init(str, 0, len - 1);
}

void Symbol::Init(const char* str, int startIndex, int endIndex)
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
}

const char* Symbol::GetString()
{
	return data;
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
}
void DataScanner::Next()
{
	if(tokCurs > 0)
	{
		m_symbol.Init(tokStack, tokOffset, tokCurs - 1);

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

	if(!foundSymbol && tokCurs > 0 && state == STATE_OPERAND)
	{
		RetrieveOperator();
		state = STATE_INVALID;
	}	
}
bool DataScanner::IsDone()
{
	return state == STATE_INVALID || *token == 0;
}
void DataScanner::CurrentItem(Symbol& symbol)
{
	symbol = m_symbol;
}

bool DataScanner::RetrieveOperator()
{
	if(tokStack[tokCurs - 1] == ' ')
	{
		tokCurs--;
	}
	if(tokCurs <= 0)
	{
		return false;
	}
	m_symbol.Init(tokStack, tokOffset, tokCurs - 1);

	tokOffset = 0;
	tokCurs = 0;

	return true;
}
bool DataScanner::RetrieveOperand()
{
	if(tokCurs < 2)
	{
		tokStack[tokCurs] = 0;
		printf("[ERROR]: This should not happen. stack=%s\n", tokStack);
		return false;
	}

	m_symbol.Init(tokStack, tokOffset, tokCurs - 2);


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