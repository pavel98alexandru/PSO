#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

/* Added by Adrian Colesa - multithreading */
#include "userprog/process.h"
#include "threads/synch.h"

/* Added by Adrian Colesa - multithreading */
static struct semaphore join_sema[TH_NO];
static struct uthread_args process_uthreads_args[TH_NO];
static tid_t process_uthreads_tids[TH_NO];
static int process_uthread_status[TH_NO];

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/*
 * Added by Adrian Colesa - multithreading
 * This should be normally associated to each process, but since we currently support
 * just one process, declared it global
 */
static crt_thread_id = 0;


static void
syscall_handler (struct intr_frame *f)
{
	/* Added by Adrian Colesa - multithreading */
	int syscall_no = ((int*)f->esp)[0];
	int fd, no;
	char *buf;
	int waited_tid;
	int *status, th_status, i;

	printf ("system call no %d!\n", syscall_no);

	/* Added by Adrian Colesa - multithreading */
	switch (syscall_no) {
		case SYS_EXIT:
			printf ("SYS_EXIT system call from thread %d!\n", thread_current()->tid);
			thread_exit();
			return;
		case SYS_WRITE:
			printf ("SYS_WRITE system call from thread %d!\n", thread_current()->tid);

			fd = ((int*)f->esp)[1];
			buf = (char*) ((int*)f->esp)[2];
			no = ((int*)f->esp)[3];

			if (fd == 1) {
				/* TO DO: Call the corresponding console function to display the string no bytes in buf */
				// ...
				putbuf(buf,no);
			}

			f->eax = no;
			return;
		case SYS_UTHREAD_CREATE:
			printf ("SYS_UTHREAD_CREATE system call from thread %d!\n", thread_current()->tid);

			/* Thread function and its argument prepared for thread creation */
			process_uthreads_args[crt_thread_id].th_fc_addr = ((int*)f->esp)[1];
			process_uthreads_args[crt_thread_id].th_fc_arg = ((int*)f->esp)[2];

			/* Thread name is based on the process' one and thread no */
			snprintf(process_uthreads_args[crt_thread_id].th_name,
					sizeof(process_uthreads_args[crt_thread_id].th_name),
					"th-%d", crt_thread_id);

			/* Thread pid equals that of the current process */
			process_uthreads_args[crt_thread_id].th_pid = thread_current()->pid;

			/* Thread pagedir equals that of the current process */
			process_uthreads_args[crt_thread_id].th_pagedir = thread_current()->pagedir;

			/* Process internal thread id (no) */
			process_uthreads_args[crt_thread_id].th_no = crt_thread_id;

			/* Call the function to create the thread with the prepared arguments */
			process_uthreads_tids[crt_thread_id] = process_uthread_execute(&process_uthreads_args[crt_thread_id]);

			/* Check if thread successfully created */
			if (process_uthreads_tids[crt_thread_id] != -1) {
				/* TO DO: Initialize the corresponding semaphore for possible joiner */
				// ...


				sema_init(&join_sema[crt_thread_id],0);

				f->eax = crt_thread_id;
			} else {
				f->eax = -1;
			}

			crt_thread_id++;
			return;
		case SYS_UTHREAD_JOIN:
			printf ("SYS_UTHREAD_JOIN system call from thread %d!\n", thread_current()->tid);

			waited_tid = ((int*)f->esp)[1];
			status = (int*)((int*)f->esp)[2];

			if (process_uthreads_tids[waited_tid] != -1) {
				/* TO DO: Use the corresponding semaphore to wait for the desired thread */
				// ....

				sema_down(&join_sema[waited_tid]);

				/* TO DO: Return the exit code of the waited thread */
				// ...
				*status = process_uthread_status[waited_tid];

				/* Return success */
				f->eax = 0;
			} else
				/* Return error - non existent thread */
				f->eax = -1;

			return;
		case SYS_UTHREAD_EXIT:
			printf ("SYS_UTHREAD_EXIT system call from thread %d!\n", thread_current()->tid);
			th_status = ((int*)f->esp)[1];

			/* TO DO: Save the exit code for a possible joining thread */
			// ...

			int local_tid = -1;

			for(i=0; i< TH_NO; i++)
			{

				if(thread_current()->tid == process_uthreads_tids[i])
				{
					local_tid=process_uthreads_tids[i];
					break;
				}

			}

			if(local_tid == -1)
			{
				return;
			}

			process_uthread_status[local_tid] = th_status;

			/* TO DO: use the corresponding semaphore to wake up the possible blocked joining thread */
			// ...

			sema_up(&join_sema[local_tid]);

			// Terminate the thread
			thread_exit();
			// not reached

			f->eax=0;
			return;
		case SYS_UTHREAD_GETPID:
			printf ("SYS_UTHREAD_GETPID system call from thread %d!\n", thread_current()->tid);

			/* TO DO: return the process pid */
			// ...

			f->eax = thread_current()->pid;
			

			return;
		case SYS_UTHREAD_GETTID:
			printf ("SYS_UTHREAD_GETTID system call from thread %d!\n", thread_current()->tid);

			/* TO DO: return the thread id: this could be tid or the internal thread number */
			// ...

			f->eax = thread_current() -> tid;

			return;
	}

	thread_exit ();
}
