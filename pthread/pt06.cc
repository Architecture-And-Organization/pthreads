#include <iostream>
#include <stdlib.h>
#include <pthread.h>	// for pthread_create, join,...

static pthread_mutex_t io_lock = PTHREAD_MUTEX_INITIALIZER;

extern "C" { void *thread_trampoline(void *); }

class thread
{
public:
	thread(int id, const std::string &msg) : id(id), msg(msg) 
	{
		int st = pthread_create(&pth, nullptr, thread_trampoline, (void*)this);
		if (st != 0)
		{
			errno = st; 
			perror("cannot create thread 1");
			exit(1);
		}
	}

	void run();
	void join() 						{ pthread_join(pth, nullptr); }
	int get_id() const					{ return id; }
	const std::string &get_msg() const	{ return msg; } 

private:
	pthread_t	pth;
	int			id;	
	std::string	msg;
};

/**
* A trampoline to invoke run() on an instance of a C++ class!
****************************************************************************/
void *thread_trampoline(void *p)
{
	((thread*)p)->run();
	return nullptr;
}

/**
* This method is the entry-point for the execution of a thread.
****************************************************************************/
void thread::run()
{
	for (int i=0; i<4; ++i)
	{
		pthread_mutex_lock(&io_lock);
		std::cout << "Thread " << get_id() << ": '" << get_msg() << "' " << i << std::endl;
		pthread_mutex_unlock(&io_lock);
	}
}


/**
* Create and run 4 threads.
****************************************************************************/
int main()
{
	const char* msgs[4] = {"zero", "one", "two", "three" };
	thread *th[4];
	
	for (int i=0; i<4; ++i)
		th[i] = new thread(i, msgs[i]);

	for (int i=0; i<4; ++i)
	{
		th[i]->join();
		delete th[i];
	}
		
	std::cout << "All threads have completed!" << std::endl;

	return 0;
}

