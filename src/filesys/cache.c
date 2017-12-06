#include "filesys/cache.h"
#include "filesys/filesys.h"
#include <debug.h>
#include "threads/thread.h"
#include "devices/timer.h"
#include "threads/malloc.h"


#define WRITE_BEHIND_PERIOD 1000

static void write_behind_thread(void* aux);
static void read_ahead_thread(void* aux);


static void write_behind_thread(void* aux UNUSED)
{
  while(true)
  {
    buffer_cache_flush_all();
    timer_sleep(WRITE_BEHIND_PERIOD);

  }
}


static void read_ahead_thread(void* aux UNUSED)
{
  while(true)
  {
    sema_down(&read_ahead_sema);

    lock_acquire(&read_ahead_lock);
    struct read_ahead_list_entry* rale = list_entry(list_pop_front(&read_ahead_list) , struct read_ahead_list_entry, elem);
    lock_release(&read_ahead_lock);

    lock_acquire(&buffer_cache_lock);
    disk_sector_t sec_no = rale->sec_no;
    struct buffer_cache_entry* bce = buffer_cache_find(sec_no);
    if(bce == NULL)
    {
      /* Handling cache miss. */
      struct buffer_cache_entry* victim_bce = buffer_cache_find_victim();

      /* Actual replacement. */
      ASSERT(victim_bce->accessed == false);
      /* (i) victim_bce is not valid. */
      if(victim_bce->valid == false)
      {
        ASSERT(victim_bce->dirty == false);
        /* Update the victim_bce's buffer. */
        disk_read(filesys_disk, sec_no, victim_bce->buffer);
        victim_bce->sec_no = sec_no;
        victim_bce->valid = true;           

      }
      /* (ii) victim_bce is valid. */
      else
      {
        /* Evict the victim_bce. If victim_bce is dirty, flush it. */
        if(victim_bce->dirty == true)
        {
          buffer_cache_flush(victim_bce);
        }
        victim_bce->sec_no = SEC_NO_DEFAULT;
        victim_bce->valid=false;

        /* Update the victim_bce's buffer. */
        disk_read(filesys_disk, sec_no, victim_bce->buffer);
        victim_bce->sec_no = sec_no;
        victim_bce->valid = true;
      }
    }
    SAFE_FREE(rale);
    lock_release(&buffer_cache_lock);
  }
}

void buffer_cache_read_ahead(disk_sector_t sec_no)
{
  lock_acquire(&read_ahead_lock);
  struct read_ahead_list_entry* rale = (struct read_ahead_list_entry*)malloc(sizeof(struct read_ahead_list_entry));
  if(rale == NULL)
  {
    PANIC("not enough memory to allocate rale");
  }
  rale->sec_no = sec_no;
  list_push_back(&read_ahead_list, &rale->elem);
  lock_release(&read_ahead_lock);

  sema_up(&read_ahead_sema);

}



/* Initialize buffer cache module. */
void buffer_cache_init(void)
{
  int index;
  struct buffer_cache_entry* bce;
  for(index =0; index < BUFFER_CACHE_SIZE; index++)
  {
    bce = &buffer_cache[index];
    bce->valid = false;
    bce->sec_no = SEC_NO_DEFAULT;
    bce->accessed = false;
    bce->dirty = false;
  }
  lock_init(&buffer_cache_lock);
  thread_create("write_behind_thread", PRI_MIN, write_behind_thread, NULL);

  list_init(&read_ahead_list);
  lock_init(&read_ahead_lock);
  sema_init(&read_ahead_sema, 0);
  thread_create("read_ahead_thread", PRI_MIN, read_ahead_thread, NULL);
  clock_index = -1;
}

/* Reads data from bce which sec_no is SEC_NO to BUFFER. The amount of to_read bytes is SIZE and start position is OFFSET. */
void buffer_cache_read_at(disk_sector_t sec_no, void* buffer, off_t size, off_t offset)
{
  lock_acquire(&buffer_cache_lock);
  /* Find bce which sec_no is SEC_NO. */
  struct buffer_cache_entry* bce = buffer_cache_find(sec_no);
  /* If bce == NULL, handle cache miss. */
  if(bce == NULL)
  {
    /* Handling cache miss. */
    struct buffer_cache_entry* victim_bce = buffer_cache_find_victim();

    /* Actual replacement. */
    ASSERT(victim_bce->accessed == false);
    /* (i) victim_bce is not valid. */
    if(victim_bce->valid == false)
    {
      ASSERT(victim_bce->dirty == false);
      /* Update the victim_bce's buffer. */
      disk_read(filesys_disk, sec_no, victim_bce->buffer);
      victim_bce->sec_no = sec_no;
      victim_bce->valid = true;           

    }
    /* (ii) victim_bce is valid. */
    else
    {
      /* Evict the victim_bce. If victim_bce is dirty, flush it. */
      if(victim_bce->dirty == true)
      {
        //printf("victim_bce->sec_no: %d\n", victim_bce->sec_no);
        buffer_cache_flush(victim_bce);
      }
      victim_bce->sec_no = SEC_NO_DEFAULT;
      victim_bce->valid=false;

      /* Update the victim_bce's buffer. */
      disk_read(filesys_disk, sec_no, victim_bce->buffer);
      victim_bce->sec_no = sec_no;
      victim_bce->valid = true;

    }
    /* Get the buffer cache entry. */
    bce = victim_bce;
  }
  ASSERT(bce != NULL);
  ASSERT(bce->sec_no == sec_no);
  

  /* Actual read. */
  memcpy(buffer, bce->buffer + offset, size);
  bce->accessed = true;

  lock_release(&buffer_cache_lock);
}

/* Writes BUFFER's data to bce which sec_no is SEC_NO. The amount of to_write bytes is SIZE and start position is OFFSET. */
void buffer_cache_write_at(disk_sector_t sec_no, const void* buffer, off_t size, off_t offset)
{
  lock_acquire(&buffer_cache_lock);
  /* Find bce which sec_no is SEC_NO. */
  struct buffer_cache_entry* bce = buffer_cache_find(sec_no);
  /* If bce == NULL, handle cache miss. */
  if(bce == NULL)
  {
    /* Handling cache miss. */
    struct buffer_cache_entry* victim_bce = buffer_cache_find_victim();

    /* Actual replacement. */
    ASSERT(victim_bce->accessed == false);
    /* (i) victim_bce is not valid. */
    if(victim_bce->valid == false)
    {
      victim_bce->sec_no = sec_no;
      victim_bce->valid = true; 
    }
    /* (ii) victim_bce is valid. */
    else
    {
      /* Evict the victim_bce. If victim_bce is dirty, flush it. */
      if(victim_bce->dirty == true)
      {
        buffer_cache_flush(victim_bce);
      }
      victim_bce->sec_no = SEC_NO_DEFAULT;
      victim_bce->valid=false;

      /* if offset != 0, update the victim_bce's buffer. */
      if(offset != 0)
      {
        disk_read(filesys_disk, sec_no, victim_bce->buffer);
      }
      victim_bce->sec_no = sec_no;
      victim_bce->valid = true;
    }
    bce = victim_bce;

  }
  ASSERT(bce != NULL);
  ASSERT(bce->sec_no == sec_no);

  /* Actual write. */
  memcpy(bce->buffer+offset, buffer, size);
  bce->accessed = true;
  bce->dirty = true;

  lock_release(&buffer_cache_lock);
}




/* Returns valid bce which sec_no is SEC_NO. If it is not exist, return NULL. Programmer needs mutual exclusion. */
struct buffer_cache_entry* buffer_cache_find(disk_sector_t sec_no)
{

  struct buffer_cache_entry* target_bce;
  int index;
  for(index = 0; index < BUFFER_CACHE_SIZE; index++)
  {
    target_bce = &buffer_cache[index];
    if((target_bce->sec_no == sec_no )&& target_bce->valid)
    {
      return target_bce;
    }
  } 
  return NULL;
}


/* Returns victim_bce. If buffer cache is not full, just returns useable empty victim_bce. However If buffer cache is full, does cache replacement algorithm and returns victim_bce. Programmer needs mutual exclusion. */
struct buffer_cache_entry* buffer_cache_find_victim(void)
{
  struct buffer_cache_entry* victim_bce=NULL;
  struct buffer_cache_entry* target_bce=NULL;
  
  /* Find empty bce. */
  int index;
  for(index = 0; index<BUFFER_CACHE_SIZE; index++)
  {
    target_bce = &buffer_cache[index];
    if(target_bce->valid == false)
    {
      victim_bce = target_bce;
      break;
    } 
  }

  /* If buffer_cache is full, do cache replacement algorithm. */
  if(victim_bce == NULL)
  {
    if(clock_index == -1)
    {
      clock_index = 0;
    }
    ASSERT(clock_index != -1);

    int start = clock_index;
    int last = clock_index == 0? BUFFER_CACHE_SIZE-1: clock_index - 1;

    struct buffer_cache_entry* iter_bce = NULL;
    while(true)
    {
      iter_bce = &buffer_cache[clock_index];
      
      if(iter_bce->accessed == true)
      {
        iter_bce->accessed = false;
        if(clock_index == last)
        {  
          clock_index = (clock_index+1) % BUFFER_CACHE_SIZE;
          break;
        }
        else
        {
          clock_index = (clock_index+1) % BUFFER_CACHE_SIZE;
        }
      }
      else
      {
        victim_bce = iter_bce;
        clock_index = (clock_index+1)%BUFFER_CACHE_SIZE;
        break;
      }
    }
    if(victim_bce ==NULL)
    {
      victim_bce = &buffer_cache[start];
      ASSERT(victim_bce->accessed == false);
      clock_index =(clock_index+1)% BUFFER_CACHE_SIZE;
    }
  }
  ASSERT(victim_bce != NULL);
  return victim_bce;
}





/* Flushes all dirty bce's buffers to disk. */
void buffer_cache_flush_all(void)
{
  lock_acquire(&buffer_cache_lock);
  int index;
  for(index=0; index<BUFFER_CACHE_SIZE; index++)
  {
    struct buffer_cache_entry* bce = &buffer_cache[index];
    buffer_cache_flush(bce);
  }
  lock_release(&buffer_cache_lock);
}

/* If bce is valid and dirty, flushes bce's buffer to disk. Else, does nothing. Programmer needs mutual exclusion. */
void buffer_cache_flush(struct buffer_cache_entry* bce)
{

  if(bce->valid == true && bce->dirty==true)
  {
    disk_write(filesys_disk, bce->sec_no, bce->buffer);
    bce->dirty=false;
  }

}



