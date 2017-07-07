/*
funutils.c -- little C function utility library
Copyright (C) 2017 a1batross

This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
*/

#if defined(ANDROID) && defined(__arm__)
#include <sys/mman.h>
#include <string.h>
#include "funutils.h"

//#define PAGE_SIZE 4096UL
//#define PAGE_MASK (~(PAGE_SIZE-1))
#define PAGE_ALIGN(addr) ((((size_t)addr)+PAGE_SIZE-1)&PAGE_MASK)

// copied from Bionic linker. I hope this will not be changed someday.
struct soinfo
{
  char name[128];
  void* phdr;
  size_t phnum;
  uint32_t entry;
  uint32_t base;
};

void simple_hook( void *dst, void *func, void *srcBackup )
{
	unsigned char n_code[HIJACK_SIZE];
	
	if( ((unsigned long)dst & 3) == 0 )
	{
        // ldr pc, [pc, #0]; .long addr; .long addr
        memcpy(n_code, "\x00\xf0\x9f\xe5\x00\x00\x00\x00\x00\x00\x00\x00", HIJACK_SIZE);
        *(unsigned long *)&n_code[4] = (unsigned long)func;
        *(unsigned long *)&n_code[8] = (unsigned long)func;
    }
    else // Thumb
    {
        // add r0, pc, #4; ldr r0, [r0, #0]; mov pc, r0; mov pc, r0; .long addr
        memcpy(n_code, "\x01\xa0\x00\x68\x87\x46\x87\x46\x00\x00\x00\x00", HIJACK_SIZE);
        *(unsigned long *)&n_code[8] = (unsigned long)func;
        dst--;
    }
    
    fun_rewrite( dst, n_code, HIJACK_SIZE, srcBackup );
}

void *dl_offset_sym( const void *handle, size_t offset )
{
	const struct soinfo *library = ( const struct soinfo* )( handle );
	
	return (void*)(library->base + offset);
}

void *dl_rel_offset_sym( const void *handle, const char *sym, const ptrdiff_t offset )
{
	unsigned char *base = (unsigned char*)dlsym( (void*)handle, sym );
	
	return (void*)(base + offset);
}

int fun_rewrite( void *dst, const void *src, const size_t bytes, void *srcBackup )
{
	void *start_page;
	size_t size_of_page;
	
	if( !( dst && src && bytes ) )
	{
		return -1;
	}
	
	// At first, backup original src bytes
	if( srcBackup )
	{
		memcpy( srcBackup, src, bytes );
	}
	
	// Calculate page for mprotect
	start_page = (void*)(PAGE_ALIGN( dst ) - PAGE_SIZE);

	if( (size_t)((char*)dst + bytes) > PAGE_ALIGN( dst ) )
	{
		// bytes are located on two pages
		size_of_page = PAGE_SIZE*2;
	}
	else
	{
		// bytes are located entirely on one page.
		size_of_page = PAGE_SIZE;
	}
	
	// Call mprotect, so dst memory will be writable
	if( mprotect( start_page, size_of_page, PROT_READ | PROT_WRITE | PROT_EXEC ) ) // This will succeeded only if dst was allocated by mmap().
	{
		return -1; 
	}
	
	// rewrite function
	memcpy( dst, src, bytes );
	
	// just in case
	if( mprotect( start_page, size_of_page, PROT_READ | PROT_EXEC ) )
	{
		return -1;
	}
	
	// clear instruction caches
	__clear_cache( (char*)start_page, (char*)start_page + size_of_page );
	
	return 0;	
}
#else

#error "Port me!"

#endif
