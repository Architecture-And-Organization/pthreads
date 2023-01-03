#include <iostream>
#include <stdlib.h>
#include <pthread.h> // for pthread_create, join,...

void *thread_func(void *p)
{
	std::cout << "Hello!" << std::endl;
	return nullptr;
}

int main()
{
	pthread_t th;		// the object representing the thread

	// initialize the thread object & start it running
	int st = pthread_create(&th, nullptr, thread_func, nullptr);
	if (st != 0)
	{
		errno = st;	// this is a very unusual and arcane thing to do
		perror("cannot create thread");
		exit(1);
	}

	// wait for the thread to finish its work

	pthread_join(th, nullptr);

	std::cout << "The thread has completed!" << std::endl;

	return 0;
}

