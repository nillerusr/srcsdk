/*
funutils.h -- little C function utility library
Copyright (C) 2017 a1batross

This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
*/
#pragma once
#ifndef FUNUTILS_H

#if defined(ANDROID) && defined(__arm__)
#define HIJACK_SIZE 12
#else
#error "Port me"
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	
// Generate jump to data instruction.
// This calls fun_rewrite, so there is no need to call it again after
void simple_hook( void *dst, void *data, void *srcBackup );

// Find function by image offset(from readelf, for example)
void *dl_offset_sym( const void *handle, const size_t offset );

// More "portable" version of dl_offset_sym, which don't uses private data from linker
void *dl_rel_offset_sym( const void *handle, const char *sym, const ptrdiff_t offset );

// Replace function in "dst" from "src". Backup original "src" code to "srcBackup" if not NULL.
// Clear instruction cache after. 
// Return 0 if successful, -1 if not
// !!! This function require mprotect()
int fun_rewrite( void *dst, const void *src, const size_t bytes, void *srcBackup );

// Rewrite vtable located in dst by vtable located in src
// Entries filled with __cxa_pure_virtual are ignored and will not be written to dst
size_t vtable_rewrite( void **dst, void **src );

inline void **get_vtable( void *classptr ) { return *(void***)classptr; }


// TODO: vtable hook

#ifdef __cplusplus
}
#endif

#endif // FUNUTILS_H
