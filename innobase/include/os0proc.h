/******************************************************
The interface to the operating system
process control primitives

(c) 1995 Innobase Oy

Created 9/30/1995 Heikki Tuuri
*******************************************************/

#ifndef os0proc_h
#define os0proc_h

#include "univ.i"

typedef void*			os_process_t;
typedef unsigned long int	os_process_id_t;

/* The cell type in os_awe_allocate_mem page info */
#ifdef __NT__
typedef ULONG_PTR	os_awe_t;
#else
typedef ulint		os_awe_t;
#endif

/* Physical page size when Windows AWE is used. This is the normal
page size of an Intel x86 processor. We cannot use AWE with 2 MB or 4 MB
pages. */
#define	OS_AWE_X86_PAGE_SIZE	4096

/********************************************************************
Windows AWE support. Tries to enable the "lock pages in memory" privilege for
the current process so that the current process can allocate memory-locked
virtual address space to act as the window where AWE maps physical memory. */

ibool
os_awe_enable_lock_pages_in_mem(void);
/*=================================*/
				/* out: TRUE if success, FALSE if error;
				prints error info to stderr if no success */
/********************************************************************
Allocates physical RAM memory up to 64 GB in an Intel 32-bit x86
processor. */

ibool
os_awe_allocate_physical_mem(
/*=========================*/
				/* out: TRUE if success */
	os_awe_t** page_info,	/* out, own: array of opaque data containing
				the info for allocated physical memory pages;
				each allocated 4 kB physical memory page has
				one slot of type os_awe_t in the array */
	ulint	  n_megabytes);	/* in: number of megabytes to allocate */
/********************************************************************
Allocates a window in the virtual address space where we can map then
pages of physical memory. */

byte*
os_awe_allocate_virtual_mem_window(
/*===============================*/
			/* out, own: allocated memory, or NULL if did not
			succeed */
	ulint	size);	/* in: virtual memory allocation size in bytes, must
			be < 2 GB */
/********************************************************************
With this function you can map parts of physical memory allocated with
the ..._allocate_physical_mem to the virtual address space allocated with
the previous function. Intel implements this so that the process page
tables are updated accordingly. A test on a 1.5 GHz AMD processor and XP
showed that this takes < 1 microsecond, much better than the estimated 80 us
for copying a 16 kB page memory to memory. But, the operation will at least
partially invalidate the translation lookaside buffer (TLB) of all
processors. Under a real-world load the performance hit may be bigger. */

ibool
os_awe_map_physical_mem_to_window(
/*==============================*/
					/* out: TRUE if success; the function
					calls exit(1) in case of an error */
	byte*		ptr,		/* in: a page-aligned pointer to
					somewhere in the virtual address
					space window; we map the physical mem
					pages here */
	ulint		n_mem_pages,	/* in: number of 4 kB mem pages to
					map */
	os_awe_t*	page_info);	/* in: array of page infos for those
					pages; each page has one slot in the
					array */
/********************************************************************
Converts the current process id to a number. It is not guaranteed that the
number is unique. In Linux returns the 'process number' of the current
thread. That number is the same as one sees in 'top', for example. In Linux
the thread id is not the same as one sees in 'top'. */

ulint
os_proc_get_number(void);
/*====================*/
/********************************************************************
Allocates non-cacheable memory. */

void*
os_mem_alloc_nocache(
/*=================*/
			/* out: allocated memory */
	ulint	n);	/* in: number of bytes */
#ifdef notdefined
/********************************************************************
Creates a new process. */

ibool
os_process_create(
/*==============*/
	char*		name,	/* in: name of the executable to start
				or its full path name */
	char*		cmd,	/* in: command line for the starting
				process, or NULL if no command line
				specified */
	os_process_t*	proc,	/* out: handle to the process */
	os_process_id_t* id);	/* out: process id */
/**************************************************************************
Exits a process. */

void
os_process_exit(
/*============*/
	ulint	code);	/* in: exit code */
/**************************************************************************
Gets process exit code. */

ibool
os_process_get_exit_code(
/*=====================*/
				/* out: TRUE if succeed, FALSE if fail */
	os_process_t	proc,	/* in: handle to the process */
	ulint*		code);	/* out: exit code */
#endif
/********************************************************************
Sets the priority boost for threads released from waiting within the current
process. */

void
os_process_set_priority_boost(
/*==========================*/
	ibool	do_boost);	/* in: TRUE if priority boost should be done,
				FALSE if not */

#ifndef UNIV_NONINL
#include "os0proc.ic"
#endif

#endif 
