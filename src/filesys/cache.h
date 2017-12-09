#ifndef FILESYS_CACHE_H
#define FILESYS_CACHE_H

#include "devices/disk.h"
#include "threads/synch.h"
#include <string.h>
#include "filesys/off_t.h"
#include <list.h>

#define BUFFER_CACHE_SIZE 64
#define SEC_NO_DEFAULT -1
/* Buffer cache entry. */
struct buffer_cache_entry
{
  bool valid;                               /* Loaded in buffer cache: true, Else: false. */
  disk_sector_t sec_no;                     /* Key for buffer cache. */
  bool accessed;                            /* Accessed. */
  bool dirty;                               /* Dirty. */
  uint8_t buffer[DISK_SECTOR_SIZE];         /* Block(sector) size buffer. */
};


/* Global vairables. */
struct buffer_cache_entry buffer_cache[BUFFER_CACHE_SIZE];  /* Buffer Cache. */
struct lock buffer_cache_lock;                              /* Lock for buffer cache. */
int clock_index;                                            /* Clock index for cache replacement algorithm. */                                     

struct read_ahead_list_entry
{
  disk_sector_t sec_no;                     /* Sector number for read ahead. */
  struct list_elem elem;                    /* List element for read ahead list. */
};

struct list read_ahead_list;                /* Read ahead list. */
struct lock read_ahead_lock;                /* Lock for read ahead list. */
struct semaphore read_ahead_sema;           /* Semaphore for read ahead. */


void buffer_cache_init(void);
void buffer_cache_read_at(disk_sector_t sec_no, void* buffer, off_t size, off_t offset);
void buffer_cache_write_at(disk_sector_t sec_no, const void* buffer, off_t size, off_t offset);
struct buffer_cache_entry* buffer_cache_find(disk_sector_t sec_no);
struct buffer_cache_entry* buffer_cache_find_victim(void);
void buffer_cache_flush_all(void);
void buffer_cache_flush(struct buffer_cache_entry* bce);

void buffer_cache_read_ahead(disk_sector_t sec_no);

#endif
