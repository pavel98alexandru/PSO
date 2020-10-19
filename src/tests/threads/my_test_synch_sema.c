#include "threads/thread.h"
#include <debug.h>
#include <stddef.h>
#include <random.h>
#include <stdio.h>
#include <string.h>
#include "threads/flags.h"
#include "threads/interrupt.h"
#include "threads/intr-stubs.h"
#include "threads/palloc.h"
#include "threads/switch.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#include "threads/interrupt.h"

struct semaphore * sem;

void thread_function()
{
    printf("Thread ID : %d before critical region!",thread_current()->tid);
    sema_down(sem);
    printf("Thread ID : %d inside critical region!",thread_current()->tid);
    sema_up(sem);
    printf("Thread ID : %d after the critical region!",thread_current()->tid);
}

void my_test_synch_sema()
{
    sema_init_name(sem,0,"the_test_semaphore");

    thread_create("Thread 1",PRI_DEFAULT,thread_function,NULL);
    thread_create("Thread 2",PRI_DEFAULT,thread_function,NULL);
    thread_create("Thread 3",PRI_DEFAULT,thread_function,NULL);
    thread_create("Thread 4",PRI_DEFAULT,thread_function,NULL);
    thread_create("Thread 5",PRI_DEFAULT,thread_function,NULL);
    thread_create("Thread 6",PRI_DEFAULT,thread_function,NULL);
    thread_create("Thread 7",PRI_DEFAULT,thread_function,NULL);
    thread_create("Thread 8",PRI_DEFAULT,thread_function,NULL);
    thread_create("Thread 9",PRI_DEFAULT,thread_function,NULL);
    thread_create("Thread 10",PRI_DEFAULT,thread_function,NULL);

  
}