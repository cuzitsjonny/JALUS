#pragma once
#include "Common.h"

#include <mutex>
#include <condition_variable>
#include <queue>

template <class T>
class BlockingQueue
{
private:
	mutex m;
	condition_variable c;
	queue<T> q;
public:
	inline void skip(size_t count = 1);
	inline void push(T element);
	inline T poll();

	size_t size()
	{
		return q.size();
	}
};

template<class T>
inline void BlockingQueue<T>::skip(size_t count)
{
	unique_lock<mutex> lock(m);

	for (int i = 0; i < count; i++)
	{
		q.pop();
	}

	lock.unlock();
	c.notify_one();
}

template<class T>
inline void BlockingQueue<T>::push(T element)
{
	unique_lock<mutex> lock(m);

	q.push(element);

	lock.unlock();
	c.notify_all();
}

template<class T>
inline T BlockingQueue<T>::poll()
{
	unique_lock<mutex> lock(m);

	if (q.size() > 0)
	{
		T t = q.front();
		q.pop();

		lock.unlock();
		return t;
	}
	else
	{
		c.wait(lock);
		lock.unlock();
		return poll();
	}
}