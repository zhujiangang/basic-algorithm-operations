#include <stack>
#include "DataScanner.h"
#include "Arithmetic.h"

DataScannerDecorator::DataScannerDecorator(DataScanner* pDataScanner) 
	: DataScanner(pDataScanner->GetData()), m_dataScanner(pDataScanner), lastProcessed(false)
{

}

void DataScannerDecorator::First()
{
	m_dataScanner->First();
	lastProcessed = false;
	m_symbol.SetType(Symbol::TYPE_INVALID);
}

bool DataScannerDecorator::IsDone()
{
	return m_dataScanner->IsDone() && lastProcessed && m_symbol.GetType() == Symbol::TYPE_INVALID;
}

void DataScannerDecorator::Next()
{
	if(!m_dataScanner->IsDone())
	{
		m_dataScanner->Next();
	}	
	if(!m_dataScanner->IsDone())
	{		
		return;
	}
	if(!lastProcessed)
	{
		m_symbol.Init("#", 0, 0, Symbol::TYPE_OPERATOR);
		lastProcessed = true;
		return;
	}
	m_symbol.SetType(Symbol::TYPE_INVALID);
	//
	//printf("[ERROR]: Should not go here\n");
	return;
}

void DataScannerDecorator::CurrentItem(Symbol& symbol)
{
	if(!m_dataScanner->IsDone())
	{
		m_dataScanner->CurrentItem(symbol);
		return;
	}
	symbol = m_symbol;
}


Arithmetic::OP_RESULT Arithmetic::PRIORITY_TABLE[][7] = 
{
	{ABOVE, ABOVE, BELOW, BELOW, BELOW, ABOVE, ABOVE},
	{ABOVE, ABOVE, BELOW, BELOW, BELOW, ABOVE, ABOVE},
	{ABOVE, ABOVE, ABOVE, ABOVE, BELOW, ABOVE, ABOVE},
	{ABOVE, ABOVE, ABOVE, ABOVE, BELOW, ABOVE, ABOVE},
	{BELOW, BELOW, BELOW, BELOW, BELOW, EQUAL, INVALID},
	{ABOVE, ABOVE, ABOVE, ABOVE, INVALID, ABOVE, ABOVE},
	{BELOW, BELOW, BELOW, BELOW, BELOW, INVALID, EQUAL},
};

int Arithmetic::mapping(char c)
{
	int result = -1;
	switch(c)
	{
	case '+':
		result = 0;
		break;
	case '-':
		result = 1;
		break;
	case '*':
		result = 2;
		break;
	case '/':
		result = 3;
		break;
	case '(':
		result = 4;
		break;
	case ')':
		result = 5;
		break;
	case '#':
		result = 6;
		break;
	default:
		result = -1;
		break;
	}
	return result;
}

Arithmetic::OP_RESULT Arithmetic::precede(char c1, char c2)
{
	int i = mapping(c1);
	int j = mapping(c2);
	
	return PRIORITY_TABLE[i][j];
}

double Arithmetic::operate(double a, char theta, double b)
{
	double result = 0;
	switch(theta)
	{
	case '+':
		result = a + b;
		break;
	case '-':
		result = a - b;
		break;
	case 'x':
	case '*':
		result = a * b;
		break;
	case '/':
		if(b > -0.0000001F && b < 0.0000001F)
		{
			printf("[ERROR]: 0 for divide\n");
			exit(-3);
		}
		result = a / b;
		break;
	default:
		result = 0;
		break;
	}
	return result;
}

char Arithmetic::GetFirstChar(Symbol& symbol)
{
	if(symbol.GetString() != 0 && symbol.GetString()[0] != 0)
	{
		return symbol.GetString()[0];
	}
	return (char)0xFF;
}

Arithmetic::Arithmetic()
{
}

Arithmetic::~Arithmetic()
{
}

double Arithmetic::calc(DataScanner& scanner)
{
	double result = 0;
	
	std::stack<char> optr;
	optr.push('#');
	
	std::stack<double> opnd;
	
	int operatorIndex = -1;
	char c;
	
	Symbol symbol;

	scanner.First();
	if(scanner.IsDone())
	{
		return result;
	}
	while( (scanner.CurrentItem(symbol), c = GetFirstChar(symbol)) != '#' || optr.top() != '#')
	{	
		if(symbol.GetType() == Symbol::TYPE_DIGITAL)
		{
			double d = atof(symbol.GetString());
			opnd.push(d);
			scanner.Next();
			
			continue;
		}
		if(optr.empty())
		{
			printf("optr stack empty now 0. \n");
			exit(-1);
		}
		switch(precede(optr.top(), c))
		{
		case BELOW:
			{
				optr.push(c);
				scanner.Next();
			}			
			break;
		case EQUAL:
			{
				optr.pop();
				scanner.Next();
			}
			break;
		case ABOVE:
			{
				char theta = optr.top();
				optr.pop();
				
				if(opnd.empty())
				{
					printf("opnd stack empty now 1. \n");
					exit(-2);
				}
				double b = opnd.top();
				opnd.pop();
				
				if(opnd.empty())
				{
					printf("opnd stack empty now 2. \n");
					exit(-3);
				}
				double a = opnd.top();
				opnd.pop();
				
				opnd.push(operate(a, theta, b));
			}
			break;
		default:
			printf("Invalid\n");
			break;
		}
	}
	
	result = opnd.top();

	return result;
}

double ToPostOrder::calc(DataScanner& scanner)
{
	double result = 0;
	
	std::stack<char> optr;
	optr.push('#');
	
	std::stack<double> opnd;
	
	int operatorIndex = -1;
	char c;
	
	Symbol symbol;
	
	scanner.First();
	if(scanner.IsDone())
	{
		return result;
	}
	while( (scanner.CurrentItem(symbol), c = GetFirstChar(symbol)) != '#' || optr.top() != '#')
	{	
		if(symbol.GetType() == Symbol::TYPE_DIGITAL)
		{
			double d = atof(symbol.GetString());
			opnd.push(d);
			scanner.Next();
			
			continue;
		}
		if(optr.empty())
		{
			printf("optr stack empty now 0. \n");
			exit(-1);
		}
		switch(precede(optr.top(), c))
		{
		case BELOW:
			{
				optr.push(c);
				scanner.Next();
			}			
			break;
		case EQUAL:
			{
				optr.pop();
				scanner.Next();
			}
			break;
		case ABOVE:
			{
				char theta = optr.top();
				optr.pop();
				
				if(opnd.empty())
				{
					printf("opnd stack empty now 1. \n");
					exit(-2);
				}
				double b = opnd.top();
				opnd.pop();
				
				if(opnd.empty())
				{
					printf("opnd stack empty now 2. \n");
					exit(-3);
				}
				double a = opnd.top();
				opnd.pop();
				
				opnd.push(operate(a, theta, b));
				printf("[%d,%d%c]\n", (int)a, (int)b, theta);
			}
			break;
		default:
			printf("Invalid\n");
			break;
		}
	}
	
	result = opnd.top();
	
	return result;	
}
