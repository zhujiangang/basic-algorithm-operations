// ChipTest.h: interface for the CChipTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHIPTEST_H__AA27BECF_0921_4694_8BAF_64EFEF332A05__INCLUDED_)
#define AFX_CHIPTEST_H__AA27BECF_0921_4694_8BAF_64EFEF332A05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChipTest  
{
public:
	CChipTest();
	virtual ~CChipTest();

public:
	static unsigned char Test(char chips[], int x, int y);
	static int FindGood(char chips[], int n);
};

void testChipTest();

#endif // !defined(AFX_CHIPTEST_H__AA27BECF_0921_4694_8BAF_64EFEF332A05__INCLUDED_)
