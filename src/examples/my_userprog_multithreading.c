/*
 * my_userprog_test.c
 *
 *  Created on: Nov 25, 2012
 *      Author: acolesa
 */


#include <stdio.h>
#include <syscall.h>

int th_fc(int arg)
{
	//while (1)
	printf("Process id = %d, thread id=%d, fc. arg=%d\n", uthread_getpid(), uthread_gettid(), arg);

	uthread_exit(arg);
}

int
main (int argc, char **argv)
{
	int th_id[TH_NO];
	int th_status[TH_NO];
	int i;

	for (i=0; i<TH_NO; i++) {
		th_id[i] = uthread_create(th_fc, i);
		if (th_id[i] == -1)
			printf("Error: thread %d could not be created\n", i);
		else
			printf("OK: thread %d was created with tid=th_id[%d]\n", i, th_id[i]);
	}
	
	for (i=0; i<TH_NO; i++) {
		if (th_id[i] != -1) {
			uthread_join(th_id[i], &th_status[i]);
			printf("Thread %d was finished with state=%d\n", th_id[i], th_status[i]);
		}
	}
	
	printf("ALL THREADS FINISHED\n");

	return EXIT_SUCCESS;
}


