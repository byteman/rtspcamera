#ifndef Core_ThreadPool_H
#define Core_ThreadPool_H


#include "Core.h"
#include "Core_Thread.h"
#include "Core_Mutex.h"
#include "Core_Event.h"
#include "SingletonHolder.h"
#include <vector>


namespace CORE {


#define DefaultCore_ThreadPool \
	CORE::Core_ThreadPool::defaultCore_ThreadPool


class Runnable;
class PooledCore_Thread;


class CORE_API Core_ThreadPool
{
public:
	
	Core_ThreadPool(Int32 capacity = 16);
	
	~Core_ThreadPool();
	
		/// @brief
		/// returns the maxinum capacity of the threadpool.
	Int32 capacity();

	void setCapacity(int capacity);
	
		/// @brief
		/// returns the count of the used threads in the threadpool.
	Int32 used();

	bool start(Runnable& target);

		/// @brief
		/// You must call this when use TheadPool
	void startAll();
	
		/// @brief
		/// Stops all running threads.
		/// Will also delete all thread objects.
		/// If used, this method should be the last action before
		/// the thread pool is deleted.
	void stopAll();

	void waitValidThread();

	static Core_ThreadPool& instance();

protected:
	
	PooledCore_Thread* getThread();
	
	PooledCore_Thread* createThread();

private:
	
	Core_ThreadPool(const Core_ThreadPool& pool);
	
	Core_ThreadPool& operator = (const Core_ThreadPool& pool);

	typedef std::vector<PooledCore_Thread*> Core_ThreadVec;

	Int32			 m_nCapacity;
	Core_ThreadVec		 m_threads;
	Core_FastMutex		m_mutex;
	Core_Event			m_event;

	friend class PooledCore_Thread;
};


}


#endif 

