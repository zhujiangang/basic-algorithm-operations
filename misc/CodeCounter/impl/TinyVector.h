#ifndef _TINY_VECTOR_H_
#define _TINY_VECTOR_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>


template<class T>
class CTinyVector
{
public:
	typedef size_t size_type;
	CTinyVector() : m_size(0), m_pVectorExt(NULL)
	{
	}
	CTinyVector(const T& data) : m_size(1), m_pVectorExt(NULL), m_data(data)
	{
	}
	~CTinyVector()
	{
		RemoveAll();
	}
	void Add(const T& data)
	{
		if(m_size == 0)
		{
			m_data = data;
		}
		else if(m_size == 1)
		{
			m_pVectorExt = new std::vector<T>();
			m_pVectorExt->reserve(2);
			m_pVectorExt->push_back(m_data);
			m_pVectorExt->push_back(data);
		}
		else
		{
			ASSERT(m_pVectorExt != NULL);
			ASSERT(m_pVectorExt->size() >= 2);
			m_pVectorExt->push_back(data);
		}
		m_size++;
	}
	T& GetAt(size_type pos)
	{
		ASSERT(pos < m_size);
		if(m_size == 1)
		{
			return m_data;
		}
		return m_pVectorExt->at(pos);
	}
	const T& GetAt(size_type pos) const
	{
		ASSERT(pos < m_size);
		if(m_size == 1)
		{
			return m_data;
		}
		return m_pVectorExt->at(pos);
	}
	size_type GetSize() const
	{
		return m_size;
	}
	void RemoveAll()
	{
		if(m_size > 1)
		{
			ASSERT(m_pVectorExt != NULL);
			m_pVectorExt->clear();
			delete m_pVectorExt;
			m_pVectorExt = NULL;
		}
		m_size = 0;
	}
protected:
	size_type m_size;
	std::vector<T>* m_pVectorExt;
	T m_data;
};

#endif