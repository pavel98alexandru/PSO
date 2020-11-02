#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
	printf ("system call kernel \n");

	int syscall_nr = ((int*) f->esp)[0];

	switch(syscall_nr)
	{

		case SYS_WRITE:
		{
			int fd = 0; 
    		char * buff;
    		int size;
			
    		f->eax = 0;

    		break;
		}

	}
	thread_exit ();
}
