#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "process.h"
#include "lib/kernel/hash.h"
#include "filesys/filesys.h"


static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f)
{
	int fd, no, i;
	char *buf, *file_name;
	void* mapping_addr;
	int mapping_id;
	off_t fileSize;
	int nrPag;

	int syscall_no = ((int*)f->esp)[0];
	struct supl_pte *spte;

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
		case SYS_HALT:
			shutdown_power_off();
			return;

		case SYS_OPEN:
			file_name = (char*) ((int*)f->esp)[1];

			for (fd=0; fd<MAX_OPEN_FILES; fd++)
				if (thread_current()->open_files[fd] == NULL)
					break;

			if (fd >= MAX_OPEN_FILES) {
				f->eax = -1;
				return;
			}


			thread_current()->open_files[fd] = filesys_open(file_name);

			f->eax = fd + 2;
			return;
		case SYS_READ:
			fd = ((int*)f->esp)[1];
			buf = (char*) ((int*)f->esp)[2];
			no = ((int*)f->esp)[3];

			if (thread_current()->open_files[fd-2] == NULL) {
				f->eax = -1;
				return;
			}

			f->eax = file_read(thread_current()->open_files[fd-2], buf, no);

			return;
		case SYS_CLOSE:
			fd = ((int*)f->esp)[1];

			if (thread_current()->open_files[fd-2] == NULL) {
				f->eax = -1;
				return;
			}

			file_close(thread_current()->open_files[fd-2]);
			thread_current()->open_files[fd-2] = NULL;
			f->eax = 0;
			return;
		case SYS_MMAP:
			fd = ((int*)f->esp)[1];
			mapping_addr = (void*) ((int*)f->esp)[2];

			// Lazy mapping of the file. Similar to lazy loading the contents of an executable file.
			// Calculate the total number of virtual pages needed to map the entire file.
			// Take care that the last page could not be entirely used, so the trailing bytes should be zeroed and not written back in the file.
			// Keep track for each mapped virtual page the offset in file it must be loaded from.
			// Use supplemental page table to store this information.
			// TO DO
			fileSize = file_lenght(thread_current()->open_files[fd-2]);
			nrPag = fileSize / PGSIZE;
			if (fileSize > PGSIZE*nrPag)
			{
				nrPag++;
			}

			off_t crt_ofs = 0;
			uint32_t zero_bytes = nrPag * PGSIZE - fileSize;
			while (fileSize > 0 || zero_bytes > 0)
			{
				/* Calculate how to fill this page.
				   We will read PAGE_READ_BYTES bytes from FILE
				   and zero the final PAGE_ZERO_BYTES bytes. */
				size_t page_read_bytes = fileSize < PGSIZE ? fileSize : PGSIZE;
				size_t page_zero_bytes = PGSIZE - page_read_bytes;

				// Added by Adrian Colesa - VM

				spte = malloc(sizeof(struct supl_pte));
				spte->virt_page_addr = mapping_addr;
				spte->virt_page_no = ((unsigned int)mapping_addr) / PGSIZE;
				spte->ofs = crt_ofs;
				spte->page_read_bytes = page_read_bytes;
				spte->page_zero_bytes = page_zero_bytes;
				spte->writable = true;
				spte->file = open_files[fs - 2];
				spte->map_id = 10;
				hash_insert(&thread_current()->supl_pt, &spte->he);

				printf("[load_segment] spte->virt_page_addr=0x%x, spte->virt_page_no=%d, spte->ofs=%d, spte->page_read_bytes=%d, spte->page_zero_bytes=%d, spte->writable=%d\n",
					spte->virt_page_addr, spte->virt_page_no, spte->ofs, spte->page_read_bytes, spte->page_zero_bytes, spte->writable);

				crt_ofs += page_read_bytes;

				/* Advance. */
				fileSize -= page_read_bytes;
				zero_bytes -= page_zero_bytes;
				upage += PGSIZE;
			}

			f->eax = 10;
			return;
		case SYS_MUNMAP:

			// Remove from the supplemental page table the elements corresponding to the unmapped pages.
			// TO DO
			mapp_id = { (int*)f->esp }[1];
			struct hash_iterator hash_iter;
			hash_first(&hash_iter,&thread_current()->supl_pt);
			while (hash_nexy(&hash_iter))
			{
				struct supl_pte *spte = hash_entry(hash_cur(&hash_iter), struct supl_pte, he);
				if (spte->map_id= mapp_id)
				{
					hash_delete)&thread_current()->supl_pt, spte);
					printf("page deleted!\n\n");
				}
			}
			f->eax = 0;
			return;

		case SYS_GET_SWAP_SIZE:
			// TO DO
			f->eax = 0;
			return;

		case SYS_GET_SWAP_NAME:
			// TO DO
			f->eax = 0;
			return;

		case SYS_GET_SWAP_READ_CNT:
			// TO DO
			f->eax = 0;
			return;

		case SYS_GET_SWAP_WRITE_CNT:
			// TO DO
			f->eax = 0;
			return;

		case SYS_SWAP_OUT:
			// TO DO
			f->eax = 0;
			return;

	}

	thread_exit ();
}
