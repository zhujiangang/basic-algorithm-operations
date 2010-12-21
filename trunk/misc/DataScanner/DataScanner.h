#ifndef _DATA_SCANNER_H_
#define _DATA_SCANNER_H_


class Symbol
{
public:
	static const int TYPE_INVALID;
	static const int TYPE_DIGITAL;
	static const int TYPE_OPERATOR;
public:
	Symbol(const char* str = 0, int len = 0);
	Symbol(const Symbol& other);
	Symbol& operator=(const Symbol& other);
	virtual ~Symbol();
	virtual void Init(const char* str, int startIndex, int endIndex, int type);
	virtual const char* GetString();
	virtual int GetType() const;
	virtual void SetType(int type)
	{
		this->type = type;
	}
private:
	char* data;
	int type;
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
	const char* GetData() const
	{
		return data;
	}
protected:	
	char* data;
	Symbol m_symbol;

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

	bool Retrieve(int type);
	bool RetrieveOperator();
	bool RetrieveOperand();

	// token stack
	char tokStack[MAX_STACK];
	// token stack cursor
	int tokOffset;
	int tokCurs;

	char* token;
	char state;
};

#endif