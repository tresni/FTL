// Multiple mutex test
#define __USE_XOPEN
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t threadlock;

void enable_thread_lock(void)
{
	int ret = pthread_mutex_lock(&threadlock);

	if(ret != 0)
		printf("Thread lock error: %i\n",ret);
}

void disable_thread_lock(void)
{
	int ret = pthread_mutex_unlock(&threadlock);

	if(ret != 0)
		printf("Thread unlock error: %i\n",ret);
}

void sleepms(int milliseconds)
{
	struct timeval tv;
	tv.tv_sec = milliseconds / 1000;
	tv.tv_usec = (milliseconds % 1000) * 1000;
	select(0, NULL, NULL, NULL, &tv);
}

void *thread1_func(void *val)
{
	printf("Thread 1 launched and waiting at lock.\n");
	enable_thread_lock();
	printf("Thread 1 passed lock.\n");
	sleepms(10000);
	printf("Thread 1 finished.\n");
	disable_thread_lock();
}

void *thread2_func(void *val)
{
	printf("Thread 2 launched and waiting at lock.\n");
	enable_thread_lock();
	printf("Thread 2 passed lock.\n");
	sleepms(5000);
	printf("Thread 2 finished.\n");
	disable_thread_lock();
}

void *thread3_func(void *val)
{
	printf("Thread 3 launched and waiting at lock.\n");
	enable_thread_lock();
	printf("Thread 3 passed lock.\n");
	sleepms(3000);
	printf("Thread 3 finished.\n");
	disable_thread_lock();
}

void *thread4_func(void *val)
{
	printf("Thread 4 launched and waiting at lock.\n");
	enable_thread_lock();
	printf("Thread 4 passed lock.\n");
	sleepms(7000);
	printf("Thread 4 finished.\n");
	disable_thread_lock();
}

void *thread5_func(void *val)
{
	printf("Thread 5 launched and waiting at lock.\n");
	enable_thread_lock();
	printf("Thread 5 passed lock.\n");
	sleepms(1000);
	printf("Thread 5 finished.\n");
	disable_thread_lock();
}

int main(int argc, char **argv)
{
	if (pthread_mutex_init(&threadlock, NULL) != 0)
	{
		printf("FATAL: Thread mutex init failed\n");
		// Return failure
		exit(EXIT_FAILURE);
	}

	// We will use the attributes object later to start all threads in detached mode
	pthread_attr_t attr;
	// Initialize thread attributes object with default attribute values
	pthread_attr_init(&attr);
	// When a detached thread terminates, its resources are automatically released back to
	// the system without the need for another thread to join with the terminated thread
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_t thread1;
	if(pthread_create( &thread1, &attr, thread1_func, NULL ) != 0)
	{
		printf("Unable to open thread 1. Exiting...\n");
		// Return failure
		exit(EXIT_FAILURE);
	}

	sleepms(100);

	pthread_t thread2;
	if(pthread_create( &thread2, &attr, thread2_func, NULL ) != 0)
	{
		printf("Unable to open thread 2. Exiting...\n");
		// Return failure
		exit(EXIT_FAILURE);
	}

	sleepms(100);

	pthread_t thread3;
	if(pthread_create( &thread3, &attr, thread3_func, NULL ) != 0)
	{
		printf("Unable to open thread 3. Exiting...\n");
		// Return failure
		exit(EXIT_FAILURE);
	}

	sleepms(100);

	pthread_t thread4;
	if(pthread_create( &thread4, &attr, thread4_func, NULL ) != 0)
	{
		printf("Unable to open thread 4. Exiting...\n");
		// Return failure
		exit(EXIT_FAILURE);
	}

	sleepms(100);

	pthread_t thread5;
	if(pthread_create( &thread5, &attr, thread5_func, NULL ) != 0)
	{
		printf("Unable to open thread 5. Exiting...\n");
		// Return failure
		exit(EXIT_FAILURE);
	}

	// Wait 30 seconds to prevent the main program from terminating too early
	sleepms(30000);

	return EXIT_SUCCESS;
}
