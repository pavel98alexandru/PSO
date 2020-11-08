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

	int syscall_nr = (int)((int*)f->esp)[0]; // prima valoare de pe stiva e ultima pusa in asamblare = syscall_nr 
	int fd;
	char* buff;
	int size;


	printf ("system call kernel, syscall_nr = %d \n", syscall_nr);

	switch(syscall_nr)
	{
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

		case SYS_READ:
			fd = (int)((int*)f->esp)[1]; 
			buff = (char*)((int*)f->esp)[2]; 
			size = (int)((int*)f->esp)[3];

			if(1 == fd)
			{

				int lenght = 0;

				while ( lenght < size )
				{
					*(buff+lenght) = input_getc();
					lenght++;
				}

			}
			f->eax = 0;

		default:
			thread_exit ();
	}

}
