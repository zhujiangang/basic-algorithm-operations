// OptionExp.cpp: implementation of the OptionExp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionExp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OptionExp::OptionExp() : m_pParent(NULL), m_pChildren(NULL), m_pLocalContext(NULL)
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

	if(m_pLocalContext != NULL)
	{
		std::string* ptr;
		FieldMap::iterator iter = m_pLocalContext->begin();
		for( ; iter != m_pLocalContext->end(); iter++)
		{
			ptr = iter->second;
			if(ptr != NULL)
			{
				delete ptr;
			}
		}

		delete m_pLocalContext;
		m_pLocalContext = NULL;
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

OptionExp& OptionExp::SetField(int key, const char* val)
{
	if(val == NULL)
	{
		return *this;
	}
	if(m_pLocalContext == NULL)
	{
		m_pLocalContext = new FieldMap;
	}
	
	FieldMap::iterator iter = m_pLocalContext->find(key);
	if(iter != m_pLocalContext->end())
	{
		iter->second->assign(val);
	}
	else
	{
		m_pLocalContext->insert(std::make_pair<int, std::string*>(key, new std::string(val)));
	}
	return *this;
}

std::string* OptionExp::GetField(int key)
{
	if(m_pLocalContext == NULL)
	{
		return false;
	}
	FieldMap::iterator iter = m_pLocalContext->find(key);
	if(iter == m_pLocalContext->end())
	{
		return false;
	}
	return (iter->second);
}

const char* OptionExp::GetFieldStr(int key)
{
	std::string* ptr = GetField(key);
	return (ptr ? ptr->c_str() : NULL);
}

