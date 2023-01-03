#include <iostream>
#include <stdlib.h>
#include <pthread.h>

void *thread_func(void *p)
{
	const char *msg = (const char*)p;
	std::cout << msg << std::endl;
	return nullptr;
}

int main()
{
	pthread_t th;		// the object representing the thread
	int	st;

	// initialize the thread object & start it running
	st = pthread_create(&th, nullptr, thread_func, (void*)"Hello message!");
	if (st != 0)
	{
		errno = st;
		perror("cannot create thread");
		exit(1);
	}

	// wait for the thread to finish its work

	pthread_join(th, nullptr);

	std::cout << "The thread has completed!" << std::endl;

	return 0;
}

