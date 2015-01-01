#ifndef _HEAP_H_
#define _HEAP_H_

#include <vector>

typedef int (*HeapCompareFunc) (const void * arg1, const void * arg2);
typedef void (*HeapDestroyFunc) (void * entry);
typedef void* void_ptr;

//Min heap
class Heap  
{
public:
	Heap();
	virtual ~Heap();

	void setCompareFunc(HeapCompareFunc compareFunc);
	void setDestroyFunc(HeapDestroyFunc removeFunc);

	void init(const void_ptr* entries, int n);

	void insert(const void_ptr entry);

	void* getTop();
	void* removeTop();

	void* get(int pos);
	void* remove(int pos);

	void destroy();

	int size() const;

private:
	void buildHeap();
	void swap(int i, int j);
	void shiftDown(int pos);
	void shiftUp(int pos);
	int compare(const void* arg1, const void* arg2);

private:
	HeapCompareFunc m_compare;
	HeapDestroyFunc m_destroy;
	std::vector<void*> m_data;
};

void testHeap();
void printVec(const std::vector<void*>& vec);

#endif
