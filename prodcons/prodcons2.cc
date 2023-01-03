#include <iostream>
#include <semaphore.h>
#include <pthread.h>

constexpr int BSIZE = 10;

class fifo
{
public:
	fifo();
	void send(char msg);
	char recv();

private:
    char buf[BSIZE];
    sem_t occupied;
    sem_t empty;
    int nextin;
    int nextout;
    sem_t pmut;
    sem_t cmut;
};

sem_t io_mut;

fifo::fifo()
{
	sem_init(&occupied, 0, 0);
	sem_init(&empty,0, BSIZE);
	sem_init(&pmut, 0, 1);
	sem_init(&cmut, 0, 1);
	nextin = 0;
	nextout = 0;
}


void fifo::send(char msg) 
{
    sem_wait(&empty);
    sem_wait(&pmut);

    buf[nextin] = msg;
    nextin++;
    nextin %= BSIZE;

    sem_post(&pmut);
    sem_post(&occupied);

	// This in incredibly inefficient and can even change the nature of a program when using it for debugging
	sem_wait(&io_mut);
	std::cout << "send(): '" << msg << "'" << std::endl;	// not a good idea in a locked region!
	std::cout << std::flush;
    sem_post(&io_mut);
}



char fifo::recv() 
{
    char msg;

    sem_wait(&occupied);
    sem_wait(&cmut);

    msg = buf[nextout];
    nextout++;
    nextout %= BSIZE;

    sem_post(&cmut);
    sem_post(&empty);

	sem_wait(&io_mut);
	std::cout << "recv(): '" << msg << "'" << std::endl;	// not a good idea in a locked region!
	std::cout << std::flush;
    sem_post(&io_mut);

    return(msg);
}






void *producer(void *arg)
{
	fifo *f = (fifo*)arg;
	for (char ch='a'; ch<='z'; ++ch)
		f->send(ch);

	f->send(0);	// tell the consumer we are done!

	return nullptr;
}

void *consumer(void *arg)
{
	fifo *f = (fifo*)arg;
	char msg;
	while((msg=f->recv()) != 0)
		;

	return nullptr;
}


int main()
{
	fifo f;

	sem_init(&io_mut, 0, 1);

    pthread_t thread1, thread2;
    int  iret1, iret2;

    iret1 = pthread_create(&thread1, nullptr, consumer, (void*)&f);
    iret2 = pthread_create(&thread2, nullptr, producer, (void*)&f);

    /* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */

    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr); 

	return 0;
}
