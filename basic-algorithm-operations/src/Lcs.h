// Lcs.h: interface for the CLcs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LCS_H__5A691598_657C_49B6_BDB2_66C600CB2F5B__INCLUDED_)
#define AFX_LCS_H__5A691598_657C_49B6_BDB2_66C600CB2F5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLcs  
{
public:
	CLcs();
	~CLcs();
public:
	//http://blog.csdn.net/hackbuteer1/article/details/6686931
	static int longest_common_substring(char *str1, char *str2);
	static int longest_common_substring2(char *str1, char *str2);
};

void testLCS();

#endif // !defined(AFX_LCS_H__5A691598_657C_49B6_BDB2_66C600CB2F5B__INCLUDED_)
