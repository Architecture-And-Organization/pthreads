#include <iostream>
#include <stdlib.h>
#include <pthread.h>

void *thread_func(void *p)
{
	char *msg;
	msg = (char *) p;
	std::cout << msg << std::endl;
	return nullptr;
}

int main()
{
	pthread_t thread1;
	pthread_t thread2;
	const char *message1 = "Hello from thread 1";
	const char *message2 = "Hello from thread 2";
	int	st;

	st = pthread_create(&thread1, nullptr, thread_func, (void*) message1);
	if (st != 0)
	{
		errno = st; 
		perror("cannot create thread 1");
		exit(1);
	}
	st = pthread_create(&thread2, nullptr, thread_func, (void*) message2);
	if (st != 0)
	{
		errno = st; 
		perror("cannot create thread 1");
		exit(1);
	}

	// wait for the threads to finish their work

	pthread_join(thread1, nullptr);
	pthread_join(thread2, nullptr); 

	std::cout << "All threads have completed!" << std::endl;

	return 0;
}

