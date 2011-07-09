#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "env_variable.h"

//see the code in GETENV.C(222), the implementation of getenv
void show_env_api_rt()
{
	char **search = _environ;
	// Variable strings are separated by NULL byte, and the block is 
	// terminated by a NULL byte. 
	for( ; *search; search++)
	{
		printf("%s\n", *search);
	}
}
void show_env_api_win()
{
	LPTSTR lpszVariable; 
	LPVOID lpvEnv; 
	
	// Get a pointer to the environment block. 	
	lpvEnv = GetEnvironmentStrings(); 
	
	// Variable strings are separated by NULL byte, and the block is 
	// terminated by a NULL byte. 
	for (lpszVariable = (LPTSTR) lpvEnv; *lpszVariable; lpszVariable++) 
	{ 
		printf("%s\n", lpszVariable);
		lpszVariable += strlen(lpszVariable);
	}
}
void test_env_modify()
{
	char *var;
	
	var = getenv("mytestvar");
	if(var != NULL)
		printf( "Original variable is: %s\n", var );
	

	_putenv("mytestvar=abcd");
	var = getenv("mytestvar");
	if(var != NULL)
		printf( "New variable is: %s\n", var );

	_putenv("mytestvar=");
	var = getenv("mytestvar");
	if(var != NULL)
		printf( "After remove the variable: %s\n", var );
}