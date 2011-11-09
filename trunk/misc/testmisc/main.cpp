#include <stdlib.h>
#include <stdio.h>
#include "env_variable.h"
#include "file_and_folder.h"
#include "internet.h"
#include "multi_thread.h"

void test_evn_variable();

int main(int argc, char* argv[])
{
//	test_evn_variable();
//	show_shell_folders();
//	show_internet_options();
//	test_create_folders();
	test_sys_thread_pool();
	return 0;
}

void test_evn_variable()
{
	printf("show env by run time api:\n");
	printf("=================================================\n");
	show_env_api_rt();
	printf("=================================================\n");
	printf("=================================================\n");
	printf("show env by windows api:\n");
	printf("=================================================\n");
	show_env_api_win();
	printf("=================================================\n");
	printf("=================================================\n");
	printf("test_env_modify\n");
	printf("=================================================\n");
	test_env_modify();
	printf("=================================================\n");
}
