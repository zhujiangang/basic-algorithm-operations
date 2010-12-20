#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stack>
#include "DataScanner.h"
#include "Arithmetic.h"

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

	Symbol symbol;
	DataScanner scanner(argv[1]);
	for(scanner.First(); !scanner.IsDone(); )
	{
		scanner.Next();
		scanner.CurrentItem(symbol);
		printf("%s\n", symbol.GetString());
	}

	DataScannerDecorator scannerDecorator(&scanner);
	Arithmetic arith;
	double result = arith.calc(scannerDecorator);

	printf("%f\n", result);

	return 0;
}
