#include <iostream>
#include <semaphore.h>
#include <pthread.h>

constexpr int BSIZE = 10;

typedef struct {
    char buf[BSIZE];
    sem_t occupied;
    sem_t empty;
    int nextin;
    int nextout;
    sem_t pmut;
    sem_t cmut;
} buffer_t;

sem_t io_mut;


void send_msg(buffer_t *b, char msg) 
{
    sem_wait(&b->empty);
    sem_wait(&b->pmut);


    b->buf[b->nextin] = msg;
    b->nextin++;
    b->nextin %= BSIZE;

    sem_post(&b->pmut);
    sem_post(&b->occupied);

	// This in incredibly innificient and can even change the nature of a program when using it for debugging
	sem_wait(&io_mut);
	std::cout << "send_msg(): '" << msg << "'" << std::endl;	// not a good idea in a locked region!
	std::cout << std::flush;
    sem_post(&io_mut);
}
void *producer(void *arg)
{
	buffer_t *b = (buffer_t*)arg;
	for (char ch='a'; ch<='z'; ++ch)
		send_msg(b, ch);

	send_msg(b, 0);	// tell the consumer we are done

	return nullptr;
}


char recv_msg(buffer_t *b) {
    char msg;

    sem_wait(&b->occupied);
    sem_wait(&b->cmut);


    msg = b->buf[b->nextout];
    b->nextout++;
    b->nextout %= BSIZE;


    sem_post(&b->cmut);
    sem_post(&b->empty);

	sem_wait(&io_mut);
	std::cout << "recv_msg(): '" << msg << "'" << std::endl;	// not a good idea in a locked region!
	std::cout << std::flush;
    sem_post(&io_mut);

    return(msg);
}

void *consumer(void *arg)
{
	buffer_t *b = (buffer_t*)arg;
	char msg;
	while((msg=recv_msg(b)) != 0)
		;

	return nullptr;
}


int main()
{
	buffer_t buffer;

	sem_init(&buffer.occupied, 0, 0);
	sem_init(&buffer.empty,0, BSIZE);
	sem_init(&buffer.pmut, 0, 1);
	sem_init(&buffer.cmut, 0, 1);
	buffer.nextin = buffer.nextout = 0;
	sem_init(&io_mut, 0, 1);


     pthread_t thread1, thread2;
     int  iret1, iret2;

     iret1 = pthread_create(&thread1, nullptr, consumer, (void*)&buffer);
     iret2 = pthread_create(&thread2, nullptr, producer, (void*)&buffer);

     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     pthread_join(thread1, nullptr);
     pthread_join(thread2, nullptr); 

}
