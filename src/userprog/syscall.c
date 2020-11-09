#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "userprog/process.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f)
{
	int syscall_no = ((int*)f->esp)[0];

	printf ("system call no %d!\n", syscall_no);

	int fd,size;
	char * buff;

	switch (syscall_no) {
		case SYS_EXIT:
			printf ("SYS_EXIT system call!\n");
			thread_exit();
			break;
		case SYS_WRITE:

			fd = (int)((int*)f->esp)[1]; // urmatoarea valoare pe stiva e penultima pusa in asamblare .... si tot asa
			buff = (char*)((int*)f->esp)[2]; // noi pe stiva tot int* am pus (de fapt, void*), si ca sa luam bine de acolo ce avem nevoie, initial castam la int, apoi la char *
			size = (int)((int*)f->esp)[3];

			if (1 == fd)
			{
				putbuf(buff, size);
			}
			f->eax = 0; // return code-ul. In functie de speficatii, putem sa ii dam si f->eax = size -> returneaza numarul de caractere scrise;

			break;
		case SYS_PAGE_NUMBER:
			f->eax=get_process_page_no();
			return;
	}

	thread_exit ();
}

