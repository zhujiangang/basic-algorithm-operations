#ifndef _ARITHMETIC_H_
#define _ARITHMETIC_H_

class DataScanner;
class Symbol;

class DataScannerDecorator : public virtual DataScanner
{
public:
	DataScannerDecorator(DataScanner* pDataScanner);
	void First();
	void Next();
	bool IsDone();
	void CurrentItem(Symbol& symbol);
private:
	DataScanner* m_dataScanner;
	bool lastProcessed;
};

class Arithmetic
{
public:
	Arithmetic();
	virtual ~Arithmetic();
	virtual double calc(DataScanner& scanner);
protected:
	enum OP_RESULT
	{
		INVALID = -1,
		ABOVE,
		EQUAL,
		BELOW
	};
	static OP_RESULT PRIORITY_TABLE[][7];
	static int mapping(char c);
	static OP_RESULT precede(char c1, char c2);
	static double operate(double a, char theta, double b);
	static char GetFirstChar(Symbol& symbol);
};

class ToPostOrder : public Arithmetic 
{
public:
	double calc(DataScanner& scanner);
};
#endif