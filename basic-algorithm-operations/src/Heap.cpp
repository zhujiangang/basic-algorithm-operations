#include "Heap.h"
#include "config.h"
#include "MyUtil.h"

Heap::Heap() : m_compare(NULL), m_destroy(NULL)
{
}

Heap::~Heap()
{
	destroy();
}

void Heap::setCompareFunc(HeapCompareFunc compareFunc)
{
	m_compare = compareFunc;
}

void Heap::setDestroyFunc(HeapDestroyFunc destroyFunc)
{
	m_destroy = destroyFunc;
}

void Heap::init(const void_ptr* elements, int n)
{
	destroy();

	for(int i = 0; i < n; i++)
	{
		m_data.push_back(elements[i]);
	}

	buildHeap();
}

int Heap::compare(const void* arg1, const void* arg2)
{
	if(m_compare)
	{
		return m_compare(arg1, arg2);
	}
	else
	{
		return (int)arg1 - (int)arg2;
	}
}

void Heap::swap(int i, int j)
{
	void* tmp = m_data[i];
	m_data[i] = m_data[j];
	m_data[j] = tmp;
}

void Heap::shiftDown(int pos)
{
	int n = m_data.size(), p, c;

	for(p = pos, c = p * 2 + 1; c < n; p = c, c = p * 2 + 1)
	{
		if(c < n - 1 && compare(m_data[c], m_data[c + 1]) > 0)
		{
			c++;
		}

		if(compare(m_data[p], m_data[c]) <= 0)
		{
			break;
		}
		swap(p, c);
	}
}

void Heap::shiftUp(int pos)
{
	int p, c;
	for(c = pos, p = (c + 1) / 2 - 1; p >= 0; c = p, p = (c + 1) / 2 - 1)
	{
		if(compare(m_data[p], m_data[c]) <= 0)
		{
			break;
		}

		swap(p, c);
	}
}

void Heap::buildHeap()
{
	int n = m_data.size();
	if(n <= 1)
	{
		return;
	}
	for(int i = n / 2 - 1; i >= 0; i--)
	{
		shiftDown(i);
	}
}

void Heap::insert(const void_ptr entry)
{
	m_data.push_back(entry);

	shiftUp(m_data.size() - 1);
}

void* Heap::getTop()
{
	if(m_data.empty())
	{
		return NULL;
	}
	return m_data[0];
}

void* Heap::removeTop()
{
	return remove(0);
}

void* Heap::get(int pos)
{
	if(pos < 0 || pos >= m_data.size())
	{
		return NULL;
	}
	return m_data[pos];
}

void* Heap::remove(int pos)
{
	int n = m_data.size();
	if(pos < 0 || pos >= n)
	{
		return NULL;
	}

	void* ret = m_data[pos];
	if(n == 1)
	{
		m_data.clear();
		return ret;
	}
	
	swap(pos, m_data.size() - 1);
	m_data.erase(m_data.end() - 1);
	shiftDown(pos);
	
	return ret;
}

void Heap::destroy()
{
	if(m_destroy)
	{
		for(int i = 0, size = m_data.size(); i < size; i++)
		{
			m_destroy(m_data[i]);
		}
	}
	m_data.clear();
}

int Heap::size() const
{
	return m_data.size();
}

struct Entry
{
	int index;
	int val;
};

int EntryCompare(const void* arg1, const void* arg2)
{
	const Entry* e1 = (const Entry*)arg1;
	const Entry* e2 = (const Entry*)arg2;

	return e1->val - e2->val;
}

void testHeap()
{
#if ((HEAP_TEST) == 1)
	int a[] = {2, 5, 7, 10, 32, 6, 7, 1, 16, 20};

	int n, i, val;

	n = COUNTOF(a);
	output(a, n);

	void_ptr* b = new void_ptr[n];
	for(i = 0; i < n; i++)
	{
		b[i] = (void_ptr)a[i];
	}

	Heap* pHeap = new Heap();
	pHeap->init(b, n);

	int heapSize = pHeap->size();
	for(i = 0; i < heapSize; i++)
	{
		val = (int)pHeap->removeTop();
		printf("%d ", val);
	}
	printf("\n");

	Entry** entries = new Entry*[n];
	for(i = 0; i < n; i++)
	{
		entries[i] = new Entry();
		entries[i]->index = i;
		entries[i]->val = a[i];
	}
	pHeap->setCompareFunc(EntryCompare);
	pHeap->init((const void_ptr*)entries, n);

	heapSize = pHeap->size();
	for(i = 0; i < heapSize; i++)
	{
		Entry* entry = (Entry*)pHeap->removeTop();
		printf("(%d,%d) ", entry->val, entry->index);
	}
	printf("\n");

	pHeap->destroy();

	for(i = 0; i < n; i++)
	{
		Entry* pEntry = new Entry();
		pEntry->index = i;
		pEntry->val = a[i];

		pHeap->insert(pEntry);
	}

	heapSize = pHeap->size();
	for(i = 0; i < heapSize; i++)
	{
		Entry* entry = (Entry*)pHeap->removeTop();
		printf("(%d,%d) ", entry->val, entry->index);
	}
	printf("\n");
	
	printSep(__FILE__);
#endif
}

void printVec(const std::vector<void*>& vec)
{
	for(int i = 0, size = vec.size(); i < size; i++)
	{
		int val = (int)vec.at(i);
		printf("%d ", val);
	}
	printf("\n");
}