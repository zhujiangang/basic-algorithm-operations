#ifndef _DATA_SCANNER_H_
#define _DATA_SCANNER_H_


class Symbol
{
public:
	Symbol(const char* str = 0, int len = 0);
	Symbol(const Symbol& other);
	Symbol& operator=(const Symbol& other);
	virtual ~Symbol();
	virtual void Init(const char* str, int len);
	virtual void Init(const char* str, int startIndex, int endIndex);
	virtual const char* GetString();
private:
	char* data;	
};

class DataScanner
{
public:
	DataScanner(const char* str = 0);
	virtual ~DataScanner();

	virtual void First();
	virtual void Next();
	virtual bool IsDone();
	virtual void CurrentItem(Symbol& symbol);

private:
	char* data;

private:

#define MAX_TRANS 2
#define MAX_STACK 0xFF
	
#define STATE_INVALID  -1
#define STATE_OPERATOR 0
#define STATE_OPERAND  1

	// expected tokens list
	static char* Grammar[][MAX_TRANS];	
	// next state Ids
	static char Successor[][MAX_TRANS];

	typedef bool (DataScanner::*Fct)();

	// action executed on state changed
	Fct** Action;

	bool RetrieveOperator();
	bool RetrieveOperand();

	// token stack
	char tokStack[MAX_STACK];
	// token stack cursor
	int tokOffset;
	int tokCurs;
	Symbol m_symbol;

	char* token;
	char state;
};

#endif