#include "filesys/inode.h"
#include <list.h>
#include <debug.h>
#include <round.h>
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/free-map.h"
#include "threads/malloc.h"

/* pj4 */
/*******/
#include "filesys/cache.h"
#include <stdio.h>
/*******/

/* Identifies an inode. */
#define INODE_MAGIC 0x494e4f44

/* pj4 */
/*******/
/* Direct block cnt in inode_disk. */
#define MAX_DIRECT_BLOCK_CNT 64
#define MAX_LOGICAL_BLOCK_CNT (MAX_DIRECT_BLOCK_CNT + 128 + 16384)  
/*******/

/* On-disk inode.
   Must be exactly DISK_SECTOR_SIZE bytes long. */
struct inode_disk
{
    off_t length;                                       /* File size in bytes. */
    unsigned magic;                                     /* Magic number. */

    /* pj4 */
    /*******/
    disk_sector_t direct_blocks[MAX_DIRECT_BLOCK_CNT];  /* Direct blocks. */
    disk_sector_t indirect_block;                       /* Indirect block. */
    disk_sector_t doubly_indirect_block;                /* Doubly indirect block. */
    uint32_t unused[60];                                /* Not used. */
    /*******/
};

/* Returns the number of sectors to allocate for an inode SIZE
   bytes long. */
static inline size_t
bytes_to_sectors (off_t size)
{
  return DIV_ROUND_UP (size, DISK_SECTOR_SIZE);
}



/* In-memory inode. */
struct inode 
{
    struct list_elem elem;              /* Element in inode list. */
    disk_sector_t sector;               /* Sector number of disk location. */
    int open_cnt;                       /* Number of openers. */
    bool removed;                       /* True if deleted, false otherwise. */
    int deny_write_cnt;                 /* 0: writes ok, >0: deny writes. */
    struct inode_disk data;             /* Inode content. */
};


/* pj4 */
/*******/
/* Rename bytes_to_sectors. */
/* Changes length(bytes) to logical blocks. */
static inline size_t length_to_logical_blocks(off_t length)
{
  return DIV_ROUND_UP(length, DISK_SECTOR_SIZE);
}
/*******/


/* pj4 */
/*******/
/* New static function declarations. */
static size_t get_additional_blocks(struct inode_disk* disk_inode, off_t final_length);
static size_t get_child_blocks(off_t length);
static void inode_release(struct inode* inode);
static void file_growth(struct inode* inode, off_t final_length);

/* New static function definitions. */
static size_t get_additional_blocks(struct inode_disk* disk_inode, off_t final_length)
{
  off_t current_length = disk_inode->length;
  ASSERT(current_length <= final_length);
  //trivial case.
  if(current_length == final_length)
  {
    return 0;
  }

  size_t additional_blocks = 0;
  size_t current_child_blocks = get_child_blocks(current_length);
  size_t final_child_blocks = get_child_blocks(final_length);
  additional_blocks = final_child_blocks - current_child_blocks;

  return additional_blocks;
}

static size_t get_child_blocks(off_t length)
{
  size_t logical_blocks = length_to_logical_blocks(length);
  //(i)
  if(logical_blocks <= MAX_DIRECT_BLOCK_CNT)
  {
    return logical_blocks;
  }
  //(ii) 
  else if(logical_blocks <= MAX_DIRECT_BLOCK_CNT + 128)
  {
    return logical_blocks + 1;
  }
  //(iii)
  else
  {
    size_t intermediate_blocks = (logical_blocks - 193)/128 + 1;
    return logical_blocks + 1 + 1 + intermediate_blocks;
  }
}

static void inode_release(struct inode* inode)
{
  free_map_release (inode->sector, 1);
  //free_map_release (inode->data.start,bytes_to_sectors (inode->data.length)); 
  struct inode_disk* disk_inode = &inode->data;
  size_t length = disk_inode->length;

  if(length > 0)
  {
    size_t total_logical_blocks = length_to_logical_blocks(length);
    disk_sector_t* indirect_block_buffer=NULL;
    disk_sector_t* doubly_indirect_block_buffer = NULL;
    disk_sector_t* tmp_intermediate_block_buffer = NULL;

    // indirect
    if(total_logical_blocks > MAX_DIRECT_BLOCK_CNT)
    {
      indirect_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
      if(indirect_block_buffer == NULL)
      {
        PANIC("not enough memory to allocate indirect_block_buffer");
      }
    }
    // doubly indirect
    if(total_logical_blocks > MAX_DIRECT_BLOCK_CNT + 128)
    {
      doubly_indirect_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
      if(doubly_indirect_block_buffer == NULL)
      {
        SAFE_FREE(indirect_block_buffer);
        PANIC("not enough memory to allocate doubly_indirect_block_buffer");        
      }
      tmp_intermediate_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
      if(tmp_intermediate_block_buffer == NULL)
      {
        SAFE_FREE(indirect_block_buffer);
        SAFE_FREE(doubly_indirect_block_buffer);
        PANIC("not enough memory to allocate tmp_intermediate_block_buffer");
      }
    }

    size_t i;
    disk_sector_t tmp_sec_no;
    for(i=0; i<total_logical_blocks; i++)
    {
      //(i) direct
      if(i<MAX_DIRECT_BLOCK_CNT)
      {
        tmp_sec_no = disk_inode->direct_blocks[i];
        free_map_release(tmp_sec_no, 1);
      }
      //(ii) indirect
      else if(i < MAX_DIRECT_BLOCK_CNT + 128)
      {
        if(i == MAX_DIRECT_BLOCK_CNT)
        {
          tmp_sec_no = disk_inode->indirect_block;
          buffer_cache_read_at(tmp_sec_no, indirect_block_buffer, DISK_SECTOR_SIZE, 0);
          free_map_release(tmp_sec_no, 1);
        }
        size_t j = i - MAX_DIRECT_BLOCK_CNT;
        tmp_sec_no = indirect_block_buffer[j];
        free_map_release(tmp_sec_no, 1);
      }
      //(iii) doubly indirect
      else
      {
        if( i== MAX_DIRECT_BLOCK_CNT + 128)
        {
          tmp_sec_no = disk_inode->doubly_indirect_block;
          buffer_cache_read_at(tmp_sec_no, doubly_indirect_block_buffer, DISK_SECTOR_SIZE, 0);
          free_map_release(tmp_sec_no, 1);
        }

        size_t delta = i - MAX_DIRECT_BLOCK_CNT - 128;
        size_t j = delta/128;
        size_t k = delta % 128;

        if( k == 0 )
        {
          tmp_sec_no = doubly_indirect_block_buffer[j];
          buffer_cache_read_at(tmp_sec_no, tmp_intermediate_block_buffer,DISK_SECTOR_SIZE, 0);
          free_map_release(tmp_sec_no, 1);
        }
        tmp_sec_no = tmp_intermediate_block_buffer[k];
        free_map_release(tmp_sec_no, 1);

      }
    }

    SAFE_FREE(indirect_block_buffer);
    SAFE_FREE(doubly_indirect_block_buffer);
    SAFE_FREE(tmp_intermediate_block_buffer);

  }


}


static void file_growth(struct inode* inode, off_t final_length)
{
  struct inode_disk* disk_inode = &inode->data;
  off_t current_length = disk_inode->length;
  if(current_length >= final_length)
  {
    //file growth is not needed.
    return;
  }

  size_t additional_blocks = 0;
  size_t current_child_blocks = get_child_blocks(current_length);
  size_t final_child_blocks = get_child_blocks(final_length);
  additional_blocks = final_child_blocks - current_child_blocks;

  if(additional_blocks > 0)
  {
    if(free_map_possible(additional_blocks) == false)
    {
      PANIC("not enough sectors in filesys_disk to allocate additional blocks for file growth");
    }
    size_t start = current_child_blocks;  //start logical block number.
    size_t end = final_child_blocks-1;  //end logical block number.

    size_t i;


    disk_sector_t* indirect_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
    if(indirect_block_buffer == NULL)
    {
      PANIC("not enough memory to allocate indirect_block_buffer");
    }
    disk_sector_t* doubly_indirect_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
    if(doubly_indirect_block_buffer == NULL)
    {
      SAFE_FREE(indirect_block_buffer);
      PANIC("not enough memory to allocate doubly_indirect_block_buffer");      
    }
    disk_sector_t* tmp_intermediate_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
    if(tmp_intermediate_block_buffer == NULL)
    {
      SAFE_FREE(indirect_block_buffer);
      SAFE_FREE(doubly_indirect_block_buffer);
      PANIC("not enough memory to allocate tmp_intermediate_block_buffer");

    }

    bool is_indirect_block_buffer_ready = false;
    bool is_doubly_indirect_block_buffer_ready = false;
    bool is_tmp_intermediate_block_buffer_ready = false;

    disk_sector_t tmp_sec_no;
    for(i = start; i<= end; i++)
    {
      //(i) direct
      if( i < MAX_DIRECT_BLOCK_CNT )
      {
        free_map_allocate(1, &tmp_sec_no);
        disk_inode->direct_blocks[i] = tmp_sec_no;
      }
      //(ii) indirect
      else if( i < MAX_DIRECT_BLOCK_CNT + 128)
      {
        if(is_indirect_block_buffer_ready == false)
        {
          if( i == MAX_DIRECT_BLOCK_CNT)
          {
            free_map_allocate(1, &tmp_sec_no);
            disk_inode->indirect_block = tmp_sec_no;
          } 
          else
          {
            tmp_sec_no = disk_inode->indirect_block;
            buffer_cache_read_at(tmp_sec_no, indirect_block_buffer, DISK_SECTOR_SIZE, 0);
          }
          is_indirect_block_buffer_ready = true;
        }

        size_t j = i - MAX_DIRECT_BLOCK_CNT;
        free_map_allocate(1, &tmp_sec_no);
        indirect_block_buffer[j] = tmp_sec_no;

        if( j == 127 || i == end )
        {
          tmp_sec_no = disk_inode->indirect_block;
          buffer_cache_write_at(tmp_sec_no, indirect_block_buffer, DISK_SECTOR_SIZE, 0);
        } 

      }
      //(iii) doubly indirect
      else
      {
        if(is_doubly_indirect_block_buffer_ready == false)
        {
          if( i == MAX_DIRECT_BLOCK_CNT + 128)
          {
            free_map_allocate(1, &tmp_sec_no);
            disk_inode->doubly_indirect_block = tmp_sec_no;
          }
          else
          {
            tmp_sec_no = disk_inode->doubly_indirect_block;
            buffer_cache_read_at(tmp_sec_no, doubly_indirect_block_buffer, DISK_SECTOR_SIZE, 0);
          }
          is_doubly_indirect_block_buffer_ready = true;
        }

        size_t delta = i - MAX_DIRECT_BLOCK_CNT - 128;
        size_t j = delta/128;
        size_t k = delta%128;

        if(is_tmp_intermediate_block_buffer_ready == false)
        {
          if(k == 0)
          {
            free_map_allocate(1, &tmp_sec_no);
            doubly_indirect_block_buffer[j] = tmp_sec_no;
          }
          else
          {
            tmp_sec_no = doubly_indirect_block_buffer[j];
            buffer_cache_read_at(tmp_sec_no,tmp_intermediate_block_buffer, DISK_SECTOR_SIZE, 0);
          }
          is_tmp_intermediate_block_buffer_ready = true;
        }

        free_map_allocate(1, &tmp_sec_no);
        tmp_intermediate_block_buffer[k] = tmp_sec_no;

        if(k == 127 || i == end)
        {
          tmp_sec_no = doubly_indirect_block_buffer[j];
          buffer_cache_write_at(tmp_sec_no, tmp_intermediate_block_buffer, DISK_SECTOR_SIZE, 0);
        } 

        if( i== end)
        {
          tmp_sec_no = disk_inode->doubly_indirect_block;
          buffer_cache_write_at(tmp_sec_no,doubly_indirect_block_buffer, DISK_SECTOR_SIZE, 0);
        }
      }
    }


    disk_inode->length = final_length;

    /* Finally write disk_inode into buffer cache. */
    buffer_cache_write_at(inode->sector, disk_inode, DISK_SECTOR_SIZE, 0);

    SAFE_FREE(tmp_intermediate_block_buffer);
    SAFE_FREE(doubly_indirect_block_buffer);
    SAFE_FREE(indirect_block_buffer);



  }
  else
  {
    disk_inode->length = final_length;
    buffer_cache_write_at(inode->sector, disk_inode, DISK_SECTOR_SIZE, 0);
  }

}

/*******/

/* Returns the disk sector that contains byte offset POS within
   INODE.
   Returns -1 if INODE does not contain data for a byte at offset
   POS. */
static disk_sector_t
byte_to_sector (const struct inode *inode, off_t pos) 
{
  ASSERT (inode != NULL);
  if (pos < inode->data.length)
  {
    //return inode->data.start + pos / DISK_SECTOR_SIZE;
    struct inode_disk* disk_inode = (struct inode_disk*)&inode->data;
    disk_sector_t ret_sec_no = -1;
    size_t i = pos/DISK_SECTOR_SIZE; //logical block number of pos.
    if(i <MAX_DIRECT_BLOCK_CNT)
    {
      ret_sec_no = disk_inode->direct_blocks[i];
      return ret_sec_no;
    }
    else if(MAX_DIRECT_BLOCK_CNT <= i && i < MAX_DIRECT_BLOCK_CNT + 128)
    {
      disk_sector_t* indirect_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
      if(indirect_block_buffer == NULL)
      {
        return -1;
      }
      buffer_cache_read_at(disk_inode->indirect_block, indirect_block_buffer, DISK_SECTOR_SIZE, 0);
      size_t j = i - MAX_DIRECT_BLOCK_CNT;
      ret_sec_no = indirect_block_buffer[j];
      SAFE_FREE(indirect_block_buffer);
      return ret_sec_no;
    }
    else
    {

      disk_sector_t* doubly_indirect_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
      if(doubly_indirect_block_buffer == NULL)
      {
        return -1;
      }
      disk_sector_t* tmp_intermediate_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
      if(tmp_intermediate_block_buffer == NULL)
      {
        SAFE_FREE(doubly_indirect_block_buffer);
        return -1;
      }

      buffer_cache_read_at(disk_inode->doubly_indirect_block, doubly_indirect_block_buffer, DISK_SECTOR_SIZE, 0);

      size_t delta = i - MAX_DIRECT_BLOCK_CNT - 128;
      size_t j = delta/128;

      buffer_cache_read_at(doubly_indirect_block_buffer[j],tmp_intermediate_block_buffer, DISK_SECTOR_SIZE, 0); 

      size_t k = delta % 128;
      ret_sec_no= tmp_intermediate_block_buffer[k];
      SAFE_FREE(doubly_indirect_block_buffer);
      SAFE_FREE(tmp_intermediate_block_buffer);
      return ret_sec_no;
    }

  }
  else
    return -1;
}
/* List of open inodes, so that opening a single inode twice
   returns the same `struct inode'. */
static struct list open_inodes;

/* Initializes the inode module. */
void
inode_init (void) 
{
  list_init (&open_inodes);
}

/* Initializes an inode with LENGTH bytes of data and
   writes the new inode to sector SECTOR on the file system
   disk.
   Returns true if successful.
   Returns false if memory or disk allocation fails. */
bool
inode_create (disk_sector_t sector, off_t length)
{
  struct inode_disk *disk_inode = NULL;
  ASSERT (length >= 0);

  /* If this assertion fails, the inode structure is not exactly
     one sector in size, and you should fix that. */
  ASSERT (sizeof *disk_inode == DISK_SECTOR_SIZE);
  disk_inode = calloc (1, sizeof *disk_inode);

  /*
  if (disk_inode != NULL)
  {
    size_t sectors = bytes_to_sectors (length);
    disk_inode->length = length;
    disk_inode->magic = INODE_MAGIC;
    if (free_map_allocate (sectors, &disk_inode->start))
    {

      //disk_write (filesys_disk, sector, disk_inode);
      buffer_cache_write_at(sector, disk_inode, DISK_SECTOR_SIZE, 0);
      if (sectors > 0) 
      {
        static char zeros[DISK_SECTOR_SIZE];
        size_t i;

        for (i = 0; i < sectors; i++)
        { 
          //disk_write (filesys_disk, disk_inode->start + i, zeros); 
          buffer_cache_write_at(disk_inode->start+i, zeros, DISK_SECTOR_SIZE,0);
        }
      }
      success = true; 
    } 
    free (disk_inode);
  }
  return success;
  */

  /* pj4 */
  /*******/
  if( disk_inode != NULL)
  { 
    disk_inode->length = 0;
    disk_inode->magic = INODE_MAGIC;
    off_t final_length = length;
    size_t total_logical_blocks = length_to_logical_blocks(length);

    /* Get additional blocks to allocate. */
    size_t additional_blocks = get_additional_blocks(disk_inode, final_length); 
    
    //printf("final_length: [%d], total_logical_blocks: [%d], additional_blocks: [%d]\n", final_length, total_logical_blocks, additional_blocks); 

    /* If additional blocks are not needed, just initialize disk_inode with simple informations
       and write it into the buffer cache. Then free disk_inode and return true. */
    if(additional_blocks == 0)
    {
      disk_inode->length = final_length;
      buffer_cache_write_at(sector, disk_inode, DISK_SECTOR_SIZE, 0);
      SAFE_FREE(disk_inode);
      return true;
    }

    /* Check there is enough sectors in the filesys_disk. */
    if(free_map_possible(additional_blocks) == false)
    {
      SAFE_FREE(disk_inode);
      return false;
    }

    
    /* Zero-filled data block. */
    static char zeros[DISK_SECTOR_SIZE]; 

    disk_sector_t* indirect_block_buffer = NULL;
    disk_sector_t* doubly_indirect_block_buffer = NULL;
    disk_sector_t* tmp_intermediate_block_buffer = NULL;

    // indirect
    if(total_logical_blocks > MAX_DIRECT_BLOCK_CNT)
    {
      indirect_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
      if(indirect_block_buffer == NULL)
      {
        SAFE_FREE(disk_inode);
        return false;
      }
      
    }

    // doubly indirect
    if(total_logical_blocks > MAX_DIRECT_BLOCK_CNT + 128)
    {
      doubly_indirect_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
      if(doubly_indirect_block_buffer == NULL)
      {
        SAFE_FREE(indirect_block_buffer);
        SAFE_FREE(disk_inode);
        return false;
      }
      tmp_intermediate_block_buffer = (disk_sector_t*)calloc(128, sizeof(disk_sector_t));
      if(tmp_intermediate_block_buffer == NULL)
      {
        SAFE_FREE(doubly_indirect_block_buffer);
        SAFE_FREE(indirect_block_buffer);
        SAFE_FREE(disk_inode);
        return false;
      }
    }


    /* Now, we are ready to allocate the additional blocks.
       We don't have to worry about failing. */
    disk_inode->length = final_length;


    size_t i;
    disk_sector_t tmp_sec_no;
    for(i=0; i<total_logical_blocks; i++)
    {
      //(i) direct
      if(i<MAX_DIRECT_BLOCK_CNT)
      {
        free_map_allocate(1, &tmp_sec_no);
        disk_inode->direct_blocks[i] = tmp_sec_no;
        buffer_cache_write_at(tmp_sec_no, zeros, DISK_SECTOR_SIZE, 0);
      }
      //(ii) indirect
      else if( i <MAX_DIRECT_BLOCK_CNT + 128)
      {
        if(i == MAX_DIRECT_BLOCK_CNT)
        {
          free_map_allocate(1, &tmp_sec_no);
          disk_inode->indirect_block = tmp_sec_no;
        }

        free_map_allocate(1, &tmp_sec_no);
        size_t j = i - MAX_DIRECT_BLOCK_CNT;
        indirect_block_buffer[j] = tmp_sec_no;
        buffer_cache_write_at(tmp_sec_no, zeros,DISK_SECTOR_SIZE, 0);

        if(j == 127 || i == total_logical_blocks - 1)
        {
          tmp_sec_no = disk_inode->indirect_block;
          buffer_cache_write_at(tmp_sec_no, indirect_block_buffer, DISK_SECTOR_SIZE, 0);
        }

      }
      //(iii) doubly indirect
      else
      {
        if(i == MAX_DIRECT_BLOCK_CNT + 128)
        {
          free_map_allocate(1, &tmp_sec_no);
          disk_inode->doubly_indirect_block = tmp_sec_no;
        }

        size_t delta = i - MAX_DIRECT_BLOCK_CNT - 128;
        size_t j = delta / 128;
        size_t k = delta % 128;

        if(k == 0)
        {
          free_map_allocate(1, &tmp_sec_no);
          doubly_indirect_block_buffer[j] = tmp_sec_no;
        }

        free_map_allocate(1, &tmp_sec_no);
        tmp_intermediate_block_buffer[k] = tmp_sec_no;
        buffer_cache_write_at(tmp_sec_no, zeros, DISK_SECTOR_SIZE, 0);

        if(k == 127 || i == total_logical_blocks - 1)
        {
          tmp_sec_no = doubly_indirect_block_buffer[j];
          buffer_cache_write_at(tmp_sec_no, tmp_intermediate_block_buffer, DISK_SECTOR_SIZE, 0);
        }

        if(i==total_logical_blocks - 1)
        {
          tmp_sec_no = disk_inode->doubly_indirect_block;
          buffer_cache_write_at(tmp_sec_no, doubly_indirect_block_buffer , DISK_SECTOR_SIZE, 0);
        }

      }
    }

    /* Finally write disk_inode into buffer cache. */
    buffer_cache_write_at(sector, disk_inode, DISK_SECTOR_SIZE, 0);

    SAFE_FREE(tmp_intermediate_block_buffer);
    SAFE_FREE(doubly_indirect_block_buffer);
    SAFE_FREE(indirect_block_buffer);
    SAFE_FREE(disk_inode);
    return true;
  }
  else
  {
    return false;
  }


  /*******/
}

/* Reads an inode from SECTOR
   and returns a `struct inode' that contains it.
   Returns a null pointer if memory allocation fails. */
struct inode *
inode_open (disk_sector_t sector) 
{
  struct list_elem *e;
  struct inode *inode;

  /* Check whether this inode is already open. */
  for (e = list_begin (&open_inodes); e != list_end (&open_inodes);
   e = list_next (e)) 
  {
    inode = list_entry (e, struct inode, elem);
    if (inode->sector == sector) 
    {
      inode_reopen (inode);
      return inode; 
    }
  }

  /* Allocate memory. */
  inode = malloc (sizeof *inode);
  if (inode == NULL)
    return NULL;

  /* Initialize. */
  list_push_front (&open_inodes, &inode->elem);
  inode->sector = sector;
  inode->open_cnt = 1;
  inode->deny_write_cnt = 0;
  inode->removed = false;
  /* pj4 */
  /*******/
  //disk_read (filesys_disk, inode->sector, &inode->data);
  buffer_cache_read_at(sector, &inode->data, DISK_SECTOR_SIZE, 0);
  /*******/
  return inode;
}

/* Reopens and returns INODE. */
struct inode *
inode_reopen (struct inode *inode)
{
  if (inode != NULL)
    inode->open_cnt++;
  return inode;
}

/* Returns INODE's inode number. */
disk_sector_t
inode_get_inumber (const struct inode *inode)
{
  return inode->sector;
}

/* Closes INODE and writes it to disk.
   If this was the last reference to INODE, frees its memory.
   If INODE was also a removed inode, frees its blocks. */
void
inode_close (struct inode *inode) 
{
  /* Ignore null pointer. */
  if (inode == NULL)
    return;

  /* Release resources if this was the last opener. */
  if (--inode->open_cnt == 0)
  {
      /* Remove from inode list and release lock. */
    list_remove (&inode->elem);

      /* Deallocate blocks if removed. */
    if (inode->removed) 
    {
      /* pj4 */
      /*******/
      inode_release(inode);
      /*******/ 
    }

    free (inode); 
  }
}

/* Marks INODE to be deleted when it is closed by the last caller who
   has it open. */
void
inode_remove (struct inode *inode) 
{
  ASSERT (inode != NULL);
  inode->removed = true;
}

/* Reads SIZE bytes from INODE into BUFFER, starting at position OFFSET.
   Returns the number of bytes actually read, which may be less
   than SIZE if an error occurs or end of file is reached. */
off_t
inode_read_at (struct inode *inode, void *buffer_, off_t size, off_t offset) 
{
  uint8_t *buffer = buffer_;
  off_t bytes_read = 0;
  /* pj4 */
  /*******/
  //uint8_t *bounce = NULL;
  /*******/

  while (size > 0) 
  {
      /* Disk sector to read, starting byte offset within sector. */
    disk_sector_t sector_idx = byte_to_sector (inode, offset);
    int sector_ofs = offset % DISK_SECTOR_SIZE;

      /* Bytes left in inode, bytes left in sector, lesser of the two. */
    off_t inode_left = inode_length (inode) - offset;
    int sector_left = DISK_SECTOR_SIZE - sector_ofs;
    int min_left = inode_left < sector_left ? inode_left : sector_left;

      /* Number of bytes to actually copy out of this sector. */
    int chunk_size = size < min_left ? size : min_left;
    if (chunk_size <= 0)
      break;

      /* pj4 */
      /*******/
      /*
      if (sector_ofs == 0 && chunk_size == DISK_SECTOR_SIZE) 
        {
          // Read full sector directly into caller's buffer. 
          disk_read (filesys_disk, sector_idx, buffer + bytes_read);       
        }
      else 
        {
          // Read sector into bounce buffer, then partially copy into caller's buffer.
          if (bounce == NULL) 
            {
              bounce = malloc (DISK_SECTOR_SIZE);
              if (bounce == NULL)
                break;
            }
          disk_read (filesys_disk, sector_idx, bounce);
          memcpy (buffer + bytes_read, bounce + sector_ofs, chunk_size);
        }
      */

      /* You should get rid of bounce buffers. Instead, copy data into and out of sectors in the buffer cache directly. */
    buffer_cache_read_at(sector_idx, buffer+bytes_read, chunk_size, sector_ofs);

    disk_sector_t next_sector_idx = sector_idx + 1;
    buffer_cache_read_ahead(next_sector_idx);
      /*******/

      /* Advance. */
    size -= chunk_size;
    offset += chunk_size;
    bytes_read += chunk_size;
  }
  //free (bounce);

  return bytes_read;
}

/* Writes SIZE bytes from BUFFER into INODE, starting at OFFSET.
   Returns the number of bytes actually written, which may be
   less than SIZE if end of file is reached or an error occurs.
   (Normally a write at end of file would extend the inode, but
   growth is not yet implemented.) */
off_t
inode_write_at (struct inode *inode, const void *buffer_, off_t size,
  off_t offset) 
{
  const uint8_t *buffer = buffer_;
  off_t bytes_written = 0;
  /* pj4 */
  /*******/
  //uint8_t *bounce = NULL;
  /*******/

  if (inode->deny_write_cnt)
    return 0;

  /* pj4 */
  /*******/
  off_t final_length = offset+ size;
  file_growth(inode, final_length);
  /*******/

  while (size > 0) 
  {
      /* Sector to write, starting byte offset within sector. */
    disk_sector_t sector_idx = byte_to_sector (inode, offset);
    int sector_ofs = offset % DISK_SECTOR_SIZE;

      /* Bytes left in inode, bytes left in sector, lesser of the two. */
    off_t inode_left = inode_length (inode) - offset;
    int sector_left = DISK_SECTOR_SIZE - sector_ofs;
    int min_left = inode_left < sector_left ? inode_left : sector_left;

      /* Number of bytes to actually write into this sector. */
    int chunk_size = size < min_left ? size : min_left;
    if (chunk_size <= 0)
      break;

      /* pj4 */
      /*******/
      /*
      if (sector_ofs == 0 && chunk_size == DISK_SECTOR_SIZE) 
        {
          // Write full sector directly to disk. 
          disk_write (filesys_disk, sector_idx, buffer + bytes_written); 
        }
      else 
        {
          // We need a bounce buffer. 
          if (bounce == NULL) 
            {
              bounce = malloc (DISK_SECTOR_SIZE);
              if (bounce == NULL)
                break;
            }
          // If the sector contains data before or after the chunk
          //   we're writing, then we need to read in the sector
          //   first.  Otherwise we start with a sector of all zeros. 
          if (sector_ofs > 0 || chunk_size < sector_left) 
            disk_read (filesys_disk, sector_idx, bounce);
          else
            memset (bounce, 0, DISK_SECTOR_SIZE);
          memcpy (bounce + sector_ofs, buffer + bytes_written, chunk_size);
          disk_write (filesys_disk, sector_idx, bounce); 
        }
      */

      /* You should get rid of bounce buffers. Instead, copy data into and out of sectors in the buffer cache directly. */
    buffer_cache_write_at(sector_idx, buffer+bytes_written, chunk_size, sector_ofs);
      /*******/  

      /* Advance. */
    size -= chunk_size;
    offset += chunk_size;
    bytes_written += chunk_size;
  }
  //free (bounce);

  return bytes_written;
}

/* Disables writes to INODE.
   May be called at most once per inode opener. */
void
inode_deny_write (struct inode *inode) 
{
  inode->deny_write_cnt++;
  ASSERT (inode->deny_write_cnt <= inode->open_cnt);
}

/* Re-enables writes to INODE.
   Must be called once by each inode opener who has called
   inode_deny_write() on the inode, before closing the inode. */
void
inode_allow_write (struct inode *inode) 
{
  ASSERT (inode->deny_write_cnt > 0);
  ASSERT (inode->deny_write_cnt <= inode->open_cnt);
  inode->deny_write_cnt--;
}

/* Returns the length, in bytes, of INODE's data. */
off_t
inode_length (const struct inode *inode)
{
  return inode->data.length;
}