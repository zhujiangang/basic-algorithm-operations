#ifndef _LIST_CTRL_SORTER_H_
#define _LIST_CTRL_SORTER_H_

#pragma once

class CSorter
{
public:
	virtual void Sort(int iColumn);
	virtual void Sort (int iColumn, BOOL bAscending, BOOL bAdd = FALSE);
};


#endif