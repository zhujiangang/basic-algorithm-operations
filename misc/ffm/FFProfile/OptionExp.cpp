// OptionExp.cpp: implementation of the OptionExp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionExp.h"
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OptionExp::OptionExp() : m_pParent(NULL), m_pChildren(NULL), m_pPtrStrMap(NULL), m_pPtrVoidMap(NULL)
{

}

OptionExp::~OptionExp()
{
	if(m_pChildren != NULL)
	{
		OptionExp* pChild;
		for(int i = m_pChildren->size() - 1; i >= 0; i--)
		{
			pChild = m_pChildren->at(i);
			if(pChild != NULL)
			{
				delete pChild;
			}
		}

		delete m_pChildren;
		m_pChildren = NULL;
	}

	if(m_pPtrStrMap != NULL)
	{
		std::string* ptr;
		ips_map::iterator iter = m_pPtrStrMap->begin();
		for( ; iter != m_pPtrStrMap->end(); iter++)
		{
			ptr = iter->second;
			if(ptr != NULL)
			{
				delete ptr;
			}
		}

		delete m_pPtrStrMap;
		m_pPtrStrMap = NULL;
	}

	if(m_pPtrVoidMap != NULL)
	{
		m_pPtrVoidMap->clear();
		delete m_pPtrVoidMap;
		m_pPtrVoidMap = NULL;
	}
}

OptionExp* OptionExp::GetParent()
{
	return m_pParent;
}
OptionExp* OptionExp::GetChild(int childIndex)
{
	if( (m_pChildren == NULL) || (childIndex < 0) || (childIndex >= m_pChildren->size()) )
	{
		return NULL;
	}
	return m_pChildren->at(childIndex);
}
int OptionExp::GetChildCount()
{
	return (m_pChildren ? m_pChildren->size() : 0);
}

int OptionExp::GetIndex(OptionExp* pChild)
{
	if(m_pChildren == NULL || pChild == NULL)
	{
		return -1;
	}
	for(int i = 0; i < m_pChildren->size(); i++)
	{
		if(pChild == m_pChildren->at(i))
		{
			return i;
		}
	}
	return -1;
}

OptionExp* OptionExp::GetRoot()
{
	OptionExp *pNode = this;
	while( pNode->GetParent() != NULL)
	{
		pNode = pNode->GetParent();
	}
	return pNode;
}

OptionExp& OptionExp::SetParent(OptionExp* pParent)
{
	ASSERT(m_pParent == NULL);
	m_pParent = pParent;
	return *this;
}
OptionExp& OptionExp::AddChild(OptionExp* pChild)
{
	if(m_pChildren == NULL)
	{
		m_pChildren = new OptionExpVec;
	}
	m_pChildren->push_back(pChild);
	pChild->SetParent(this);
	return *this;
}

OptionExp& OptionExp::AddChild(OptionExp* pChild, int nIndex)
{
	if(m_pChildren == NULL)
	{
		m_pChildren = new OptionExpVec;
	}
	if(nIndex < 0 || nIndex >= m_pChildren->size())
	{
		m_pChildren->push_back(pChild);
	}
	else
	{

		OptionExpVec::iterator iter = m_pChildren->begin();
		std::advance(iter, nIndex);
		iter = m_pChildren->insert(iter, pChild);
		ASSERT(pChild == (*iter));
	}
	pChild->SetParent(this);
	return *this;
}

OptionExp& OptionExp::SetField(int key, const char* val)
{
	return SetFieldPtrStr(key, val);
}

std::string* OptionExp::GetField(int key)
{
	return GetFieldPtrStr(key);
}

const char* OptionExp::GetFieldStr(int key)
{
	std::string* ptr = GetFieldPtrStr(key);
	return (ptr ? ptr->c_str() : NULL);
}

OptionExp& OptionExp::SetFieldPtrStr(int key, const char* val)
{
	if(val == NULL)
	{
		return *this;
	}
	if(m_pPtrStrMap == NULL)
	{
		m_pPtrStrMap = new ips_map;
	}
	
	ips_map::iterator iter = m_pPtrStrMap->find(key);
	if(iter != m_pPtrStrMap->end())
	{
		iter->second->assign(val);
	}
	else
	{
		m_pPtrStrMap->insert(std::make_pair<int, std::string*>(key, new std::string(val)));
	}
	return *this;
}
std::string* OptionExp::GetFieldPtrStr(int key)
{
	if(m_pPtrStrMap == NULL)
	{
		return NULL;
	}
	ips_map::iterator iter = m_pPtrStrMap->find(key);
	if(iter == m_pPtrStrMap->end())
	{
		return NULL;
	}
	return (iter->second);
}

OptionExp& OptionExp::SetFieldPtrVoid(int key, void* val)
{
	if(val == NULL)
	{
		return *this;
	}
	if(m_pPtrVoidMap == NULL)
	{
		m_pPtrVoidMap = new ipv_map;
	}
	
	ipv_map::iterator iter = m_pPtrVoidMap->find(key);
	if(iter != m_pPtrVoidMap->end())
	{
		iter->second = val;
	}
	else
	{
		m_pPtrVoidMap->insert(std::make_pair<int, void*>(key, val));
	}
	return *this;
}
void* OptionExp::GetFieldPtrVoid(int key)
{
	if(m_pPtrVoidMap == NULL)
	{
		return NULL;
	}
	ipv_map::iterator iter = m_pPtrVoidMap->find(key);
	if(iter == m_pPtrVoidMap->end())
	{
		return NULL;
	}
	return (iter->second);
}

OptionExp& OptionExp::SetFieldInt(int key, int val)
{
	return SetFieldPtrVoid(key, (void*)val);
}
int OptionExp::GetFieldInt(int key)
{
	return (int)GetFieldPtrVoid(key);
}

