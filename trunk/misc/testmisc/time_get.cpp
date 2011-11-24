#include "time_get.h"
#include <windows.h>
#include <time.h>
#include <stdio.h>

size_t get_ms_by_clock()
{
	return clock();
}
size_t get_ms_by_tick()
{
	return GetTickCount();
}

void test_time_get()
{
	size_t c1, c2, c3;
	size_t t1, t2, t3;

	c2 = get_ms_by_clock();
	t2 = get_ms_by_tick();

	for(int i = 0; i < 10; i++)
	{
		c1 = c2;
		t1 = t2;
		
		Sleep(500);

		c2 = get_ms_by_clock();
		c3 = c2 - c1;

		t2 = get_ms_by_tick();
		t3 = t2 - t1;

		printf("c3=%u, t3=%u, c1=%05u, c2=%05u, t1=%10u, t2=%10u\n", c3, t3, c1, c2, t1, t2);
	}
}