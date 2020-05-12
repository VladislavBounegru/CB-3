#include "RingBufferMemoryAllocation.h"
template<class T>
RingBufferMemoryAllocation<T>::RingBufferMemoryAllocation(size_t size) :
    ringBuffer(std::unique_ptr<T[]>(new T[size])),
    maxSize(size)
{
    head = 0;
    tail = 0;
    currentSize = 1;
    full = false;
    terminateFill = false;
}

template<class T>
void RingBufferMemoryAllocation<T>::push(T value)
{

    if (currentSize != 1 && currentSize <= maxSize && !terminateFill)
    {
        std::unique_ptr<T[]> tempRingBuffer(new T[currentSize - 1]);
        swap(tempRingBuffer, ringBuffer);
        ringBuffer.reset(new T[currentSize]);

        swap(ringBuffer, tempRingBuffer);
        ringBuffer[currentSize - 1] = value;
        tempRingBuffer.reset();
    }
    else
    {
        ringBuffer[head] = value;
    }

    if (currentSize == maxSize)
    {
        terminateFill = true;
    }

    currentSize = (currentSize != maxSize) ? ++currentSize : currentSize;
    head = (head + 1) % maxSize;

    if (full)
    {
        tail = (tail + 1) % maxSize;
    }
    full = head == tail ? true : false;
}

template<class T>
T RingBufferMemoryAllocation<T>::get()
{
    if (!isEmpty())
    {
        size_t tempTail = tail;
        tail = (tail + 1) % maxSize;
        full = false;
        return ringBuffer[tempTail];
    }
    return T();
}


template<class T>
void RingBufferMemoryAllocation<T>::clear()
{
    ringBuffer.reset(new T[maxSize]);
    head = 0;
    tail = 0;
    full = false;
}

template<class T>
bool RingBufferMemoryAllocation<T>::isEmpty() const
{
    return (!full && (head == tail));
}

template<class T>
bool RingBufferMemoryAllocation<T>::isFull() const
{
    return full;
}

template<class T>
size_t RingBufferMemoryAllocation<T>::getMaxSize() const
{
    return maxSize;
}

template<class T>
size_t RingBufferMemoryAllocation<T>::getCurrentSize() const
{
    return (currentSize == 1) ? currentSize : currentSize - 1;
}

template<class T>
size_t RingBufferMemoryAllocation<T>::sizeQueue() const
{
    size_t size = maxSize;

    if (!full)
    {
        if (head >= tail)
        {
            size = head - tail;
        }
        else
        {
            size = maxSize + head - tail;
        }
    }
    return size;
}

template<class T>
void RingBufferMemoryAllocation<T>::swap(std::unique_ptr<T[]>& lhs, std::unique_ptr<T[]>& rhs)
{
    for (size_t i = 0; i != currentSize - 1; ++i)
    {
        lhs[i] = rhs[i];
    }
}
