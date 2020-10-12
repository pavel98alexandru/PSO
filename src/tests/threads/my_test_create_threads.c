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

void thread_function()
{
    printf("%s\n",thread_current()->name);
    print_thread_info(thread_current());
}

void my_test_create_threads()
{
    thread_create("1st Thread",PRI_DEFAULT,thread_function,NULL);
    thread_create("2nd Thread",PRI_DEFAULT,thread_function,NULL);
    thread_create("3nd Thread",PRI_DEFAULT,thread_function,NULL);
    thread_create("4nd Thread",PRI_DEFAULT,thread_function,NULL);
    thread_create("5nd Thread",PRI_DEFAULT,thread_function,NULL);
}