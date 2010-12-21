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

	char buf[2048];
	sprintf(buf, "%s#", argv[1]);

	Symbol symbol;
	DataScanner scanner(buf);	
	for(scanner.First(); !scanner.IsDone(); scanner.Next())
	{		
		scanner.CurrentItem(symbol);
		printf("%s\n", symbol.GetString());
	}

// 	DataScannerDecorator scannerDecorator(&scanner);
// 	for(scannerDecorator.First(); !scannerDecorator.IsDone(); scannerDecorator.Next())
// 	{		
// 		scannerDecorator.CurrentItem(symbol);
// 		printf("%s\n", symbol.GetString());
// 	}
	ToPostOrder arith;
	double result = arith.calc(scanner);

	if( (result - (int)result) > -0.000001F && (result - (int)result) < 0.000001F)
	{
		printf("%d\n", (int)result);
	}
	else
	{
		printf("%f\n", result);
	}

	return 0;
}
