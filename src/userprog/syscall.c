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
syscall_handler (struct intr_frame *f)
{
	int fd, no;
	char *buf;

	int syscall_no = ((int*)f->esp)[0];

	//printf ("system call no %d!\n", syscall_no);

	switch (syscall_no) {
		case SYS_EXIT:
			//printf ("SYS_EXIT system call!\n");
			thread_exit();
			break;
		case SYS_WRITE:
			//printf ("SYS_WRITE system call from thread %d!\n", thread_current()->tid);

			fd = ((int*)f->esp)[1];
			buf = (char*) ((int*)f->esp)[2];
			no = ((int*)f->esp)[3];

			if (fd == 1)
				putbuf(buf, no);

			f->eax = no;
			return;
	}

	thread_exit ();
}
