#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stack>

#include "DataScanner.h"

// action declarations
typedef void (*Fct)();

void printStack();
void splitStack();

#define MAX_TRANS 2
#define MAX_STACK 0xFF

#define STATE_OPERATOR 0
#define STATE_OPERAND  1

// expected tokens list
char* Grammar[][MAX_TRANS] = {
	{"0123456789.", ""},
	{"0123456789.", ""}
};

// action executed on state changed
Fct Action[][MAX_TRANS] = {
	{0,&printStack},
	{0,&splitStack}
};

// next state Ids
char Successor[][MAX_TRANS] = {
	{STATE_OPERAND,STATE_OPERATOR},
	{STATE_OPERAND,STATE_OPERATOR}
};

// token stack
char tokStack[MAX_STACK];
// token stack cursor
int  tokCurs = 0;

// print the token stack content
void printStack()
{
	if(tokStack[tokCurs - 1] == ' ')
	{
		tokCurs--;
	}
	if(tokCurs <= 0)
	{
		return;
	}
	tokStack[tokCurs] = 0;
	printf("%s|\n", tokStack);
}

void splitStack()
{
	if(tokCurs < 2)
	{
		tokStack[tokCurs] = 0;
		printf("This should not happen. stack=%s\n", tokStack);
		exit(-1);
	}
	char c = tokStack[tokCurs - 1];

	tokStack[tokCurs - 1] = 0;
	printf("%s|\n", tokStack);

	if(c != ' ')
	{
		printf("%c|\n", c);
	}	
}


void funcion_in_c(char* argv[])
{
	short trans;
	char* token;
	char state = STATE_OPERATOR;
	// parse input string
	for (token = argv[1]; *token != 0; token++)
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
					(*Action[state][trans])();
					tokCurs = 0; // reset token stack
				}
				// update parser state
				state = Successor[state][trans];
				break;
			}
		}
	}
	
	if(state == STATE_OPERAND)
	{
		printStack();
	}
}

int main(int argc, char* argv[])
{	
	if (argc < 2 || !strcmp(argv[1], "/?"))
	{
		printf("wrong input\n");		
		return -1;
 	}
	short arg;
	// concate all arguments
	for (arg = 1; arg < argc-1; arg++)
		argv[arg][strlen(argv[arg])] = ' ';
//	funcion_in_c(argv);

	Symbol symbol;
	DataScanner scanner(argv[1]);
	for(scanner.First(); !scanner.IsDone(); )
	{
		scanner.Next();
		scanner.CurrentItem(symbol);
		printf("%s\n", symbol.GetString());
	}
	return 0;
}
