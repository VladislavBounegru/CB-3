#pragma once
#include <iostream>

template <class T>
class RingBufferMemoryAllocation
{
public:
	explicit RingBufferMemoryAllocation(size_t size);
	void push(T item);
	T get();
	void clear();
	bool isEmpty() const;
	bool isFull() const;
	size_t getMaxSize() const;
	size_t getCurrentSize() const;
	size_t sizeQueue() const;

private:
	std::unique_ptr<T[]> ringBuffer;
	size_t head;
	size_t tail;
	const size_t maxSize;
	size_t currentSize;
	bool full;
	bool terminateFill;

	void swap(std::unique_ptr<T[]>& lhs, std::unique_ptr<T[]>& rhs);
};


