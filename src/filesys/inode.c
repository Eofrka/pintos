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

#define DIRECT_BLOCK_CNT 64

/* On-disk inode.
   Must be exactly DISK_SECTOR_SIZE bytes long. */
struct inode_disk
{
    off_t length;                                       /* File size in bytes. */
    unsigned magic;                                     /* Magic number. */
    disk_sector_t direct_blocks[DIRECT_BLOCK_CNT];      /* Direct blocks. */  
    disk_sector_t indirect_block;                       /* Indirect block. */
    disk_sector_t doubly_indirect_block;                /* Doubly indirect block. */
    uint32_t unused[60];                                /* Not used. */
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



static void inode_release(struct inode* inode);

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
    size_t logical_block_no = pos/DISK_SECTOR_SIZE;
    if(logical_block_no <DIRECT_BLOCK_CNT)
    {
      return disk_inode->direct_blocks[logical_block_no];
    }
    else if(DIRECT_BLOCK_CNT <= logical_block_no && logical_block_no < DIRECT_BLOCK_CNT + 128)
    {
      disk_sector_t tmp_indirect_block[128];
      size_t indirect_block_offset = logical_block_no - DIRECT_BLOCK_CNT; 
      buffer_cache_read_at(disk_inode->indirect_block, tmp_indirect_block, DISK_SECTOR_SIZE, 0);
      return tmp_indirect_block[indirect_block_offset];
    }
    else
    {

      disk_sector_t tmp_doubly_indirect_block[128];
      buffer_cache_read_at(disk_inode->doubly_indirect_block, tmp_doubly_indirect_block, DISK_SECTOR_SIZE, 0);

      size_t tmp_delta = logical_block_no - DIRECT_BLOCK_CNT - 128;
      size_t doubly_indirect_block_offset = tmp_delta/128;

      disk_sector_t tmp_intermediate_block[128];
      buffer_cache_read_at(tmp_doubly_indirect_block[doubly_indirect_block_offset],tmp_intermediate_block, DISK_SECTOR_SIZE, 0); 

      size_t connection_block_offset = tmp_delta % 128;
      return tmp_intermediate_block[connection_block_offset];
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
  if (disk_inode != NULL)
  {
    /* pj4 */
    /*******/
    /* Get total sectors for this inode. */
    size_t total_sectors = bytes_to_sectors (length);
    disk_inode->length = length;
    disk_inode->magic = INODE_MAGIC;

    if(total_sectors == 0)
    {
      buffer_cache_write_at(sector, disk_inode, DISK_SECTOR_SIZE, 0);
      SAFE_FREE(disk_inode);
      return true;
    }
    else
    {
      size_t actuall_total_sectors;
      if(total_sectors <= DIRECT_BLOCK_CNT)
      {
        actuall_total_sectors = total_sectors;
      }
      else if(total_sectors <= DIRECT_BLOCK_CNT + 128)
      {
        actuall_total_sectors = total_sectors + 1;
      }
      else
      {
        size_t total_intermediate_block_cnt = ((total_sectors -DIRECT_BLOCK_CNT - 128) + 127) /128;
        actuall_total_sectors = total_sectors + 1 + 1 + total_intermediate_block_cnt;
      }
      bool is_possible = free_map_possible(actuall_total_sectors);
      if(is_possible == false)
      {
        SAFE_FREE(disk_inode);
        return false;
      }


      /* zero-data block. */
      static char zeros[DISK_SECTOR_SIZE];


      //(i) direct blocks initialization.
      size_t total_direct_blocks = total_sectors <= DIRECT_BLOCK_CNT? total_sectors : DIRECT_BLOCK_CNT;
      size_t dbi; //direct block index.
      for(dbi =0; dbi <total_direct_blocks;dbi++)
      {
        free_map_allocate(1, &disk_inode->direct_blocks[dbi]);
      }

      //(ii) indirect block initialization.
      /* Required data structure. */
      disk_sector_t tmp_indirect_block[128];
      

      if(total_sectors > DIRECT_BLOCK_CNT)
      { 
        size_t total_indirect_blocks = total_sectors <=(DIRECT_BLOCK_CNT + 128)? (total_sectors - DIRECT_BLOCK_CNT) : 128;
        
        /* Allocate indirect block itself in the filesys_disk. */
        free_map_allocate(1, &disk_inode->indirect_block);


        /* Allocate indirect block's data blocks. */ 
        size_t idbi;
        for(idbi = 0; idbi < total_indirect_blocks; idbi++)
        {
          free_map_allocate(1, &tmp_indirect_block[idbi]);
        }
      }

      //(iii) doubly indirect block
      // doubly indirect block ------> intermediate blocks(MAX: 128) --------> data blocks(MAX: 128)
      // 1 doubly indirect block can cover 128 * 128 blocks.
      disk_sector_t tmp_doubly_indirect_block[128];
      disk_sector_t tmp_intermediate_block[128];
      disk_sector_t dirty_data_block_index;
      if(total_sectors > DIRECT_BLOCK_CNT+128)
      {

        /* Allocate doubly indirect block. */
        free_map_allocate(1, &disk_inode->doubly_indirect_block);

        size_t extra_sectors = (total_sectors - DIRECT_BLOCK_CNT - 128);
        size_t total_intermediate_block_cnt = (extra_sectors + 127) /128;
        
        size_t intermediate_bi; //intermediate block index
        
        /* Allocate intermediate blocks which have 128 data blocks and their data blocks. */
        /* Also write data blocks and intermediate blocks into buffer cache. */ 
        size_t doubly_idbi; //doubly indirect block index(tmp_interemediate_block[doubly_idbi] is data sector number).
        for(intermediate_bi = 0; intermediate_bi < total_intermediate_block_cnt - 1; intermediate_bi++)
        {
          free_map_allocate(1, &tmp_doubly_indirect_block[intermediate_bi]);
          for(doubly_idbi =0; doubly_idbi < 128; doubly_idbi++)
          {
            disk_sector_t data_sec_no;
            free_map_allocate(1, &data_sec_no);
            //printf("DEBUG: tmp_intermediate_block[%d]: %d\n", doubly_idbi, data_sec_no);
            tmp_intermediate_block[doubly_idbi] = data_sec_no;
            buffer_cache_write_at(data_sec_no, zeros, DISK_SECTOR_SIZE, 0);
          }
          buffer_cache_write_at(tmp_doubly_indirect_block[intermediate_bi],tmp_intermediate_block , DISK_SECTOR_SIZE, 0);

        }

        /* Allocate the last intermediate block and its data blocks. */
        /* Also write data blocks and intermediate blocks into buffer cache. */ 
        free_map_allocate(1, &tmp_doubly_indirect_block[total_intermediate_block_cnt-1]);
        dirty_data_block_index = (extra_sectors - 1)%128;
        for(doubly_idbi = 0; doubly_idbi <= dirty_data_block_index; doubly_idbi++)
        {
          disk_sector_t data_sec_no;
          free_map_allocate(1, &data_sec_no);
          tmp_intermediate_block[doubly_idbi] = data_sec_no;
          buffer_cache_write_at(data_sec_no, zeros, DISK_SECTOR_SIZE, 0);
        }
        buffer_cache_write_at(tmp_doubly_indirect_block[total_intermediate_block_cnt-1], tmp_intermediate_block, DISK_SECTOR_SIZE, 0);

        buffer_cache_write_at(disk_inode->doubly_indirect_block, tmp_doubly_indirect_block, DISK_SECTOR_SIZE, 0);

      }

      /* Write disk_inode and data blocks into the buffer cache. */
      
      //1. Write disk_inode itself. 
      buffer_cache_write_at(sector, disk_inode, DISK_SECTOR_SIZE, 0);

      //2. Write direct blocks.
      size_t i;
      for(i =0; i<total_direct_blocks; i++)
      {
        buffer_cache_write_at(disk_inode->direct_blocks[i], zeros, DISK_SECTOR_SIZE, 0); 
      }

      //3. Write indirect block and its data blocks.
      if(total_sectors > DIRECT_BLOCK_CNT)
      {
        size_t total_indirect_blocks = total_sectors <=(DIRECT_BLOCK_CNT + 128)? (total_sectors - DIRECT_BLOCK_CNT) : 128;
        //3-1. Write indirect block.
        buffer_cache_write_at(disk_inode->indirect_block, tmp_indirect_block, DISK_SECTOR_SIZE, 0);

        //3-2. Write indirect block's data blocks.
        for(i=0; i<total_indirect_blocks; i++)
        {
          buffer_cache_write_at(tmp_indirect_block[i], zeros, DISK_SECTOR_SIZE, 0);
        }
      }

      //4. intermediate blocks and their data blocks are already written in the buffer cache. 
      SAFE_FREE (disk_inode);
      return true;
    }
  }

  return false;


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
      inode_release(inode);
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

    disk_sector_t next_sector_idx = byte_to_sector (inode, offset + DISK_SECTOR_SIZE);
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



static void inode_release(struct inode* inode)
{
  free_map_release (inode->sector, 1);
  //free_map_release (inode->data.start,bytes_to_sectors (inode->data.length)); 
  struct inode_disk* disk_inode = &inode->data;
  size_t length = disk_inode->length;

  if(length > 0)
  {
    size_t total_sectors = bytes_to_sectors(length);
    bool is_indirect = false;
    bool is_doulby_indirect = false;
    if(DIRECT_BLOCK_CNT < total_sectors)
    {
      is_indirect = true;
    }
    if(DIRECT_BLOCK_CNT+128 < total_sectors)
    {
      is_doulby_indirect = true;

    }
    //(i) direct blocks release.
    size_t total_direct_blocks = total_sectors <= DIRECT_BLOCK_CNT? total_sectors : DIRECT_BLOCK_CNT;
    size_t dbi; //direct block index
    for(dbi = 0; dbi <total_direct_blocks; dbi++)
    {
      free_map_release(disk_inode->direct_blocks[dbi], 1);
    }

    //(ii) indirect blocks release.
    if(is_indirect == true)
    {
      size_t total_indirect_blocks = total_sectors <= (DIRECT_BLOCK_CNT + 128)? (total_sectors - DIRECT_BLOCK_CNT) : 128;

      disk_sector_t tmp_indirect_block[128];
      buffer_cache_read_at(disk_inode->indirect_block, tmp_indirect_block, DISK_SECTOR_SIZE, 0);

      size_t idbi; //indirect block index
      for(idbi = 0; idbi < total_indirect_blocks; idbi++)
      {
        free_map_release(tmp_indirect_block[idbi], 1);
      }
      free_map_release(disk_inode->indirect_block, 1);
    }

    //(iii) doubly indirect blocks release.
    if(is_doulby_indirect == true)
    {

      disk_sector_t tmp_doubly_indirect_block[128];
      buffer_cache_read_at(disk_inode->doubly_indirect_block, tmp_doubly_indirect_block, DISK_SECTOR_SIZE, 0);

      disk_sector_t tmp_intermediate_block[128];
      disk_sector_t dirty_data_block_index;

      size_t extra_sectors = (total_sectors - DIRECT_BLOCK_CNT - 128);
      size_t total_intermediate_block_cnt = (extra_sectors + 127) /128;

      size_t intermediate_bi; //intermediate block index
      size_t doubly_idbi;


      for(intermediate_bi = 0; intermediate_bi < total_intermediate_block_cnt - 1; intermediate_bi++)
      {
        buffer_cache_read_at(tmp_doubly_indirect_block[intermediate_bi], tmp_intermediate_block, DISK_SECTOR_SIZE, 0);

        for(doubly_idbi =0; doubly_idbi < 128; doubly_idbi++)
        {
          free_map_release(tmp_intermediate_block[doubly_idbi], 1);
        }
        free_map_release(tmp_doubly_indirect_block[intermediate_bi], 1);
      }

      buffer_cache_read_at(tmp_doubly_indirect_block[total_intermediate_block_cnt-1], tmp_intermediate_block, DISK_SECTOR_SIZE, 0);
      dirty_data_block_index = (extra_sectors-1)%128;
      for(doubly_idbi= 0; doubly_idbi <=dirty_data_block_index; doubly_idbi++)
      {
        free_map_release(tmp_intermediate_block[doubly_idbi], 1);
      }
      free_map_release(tmp_doubly_indirect_block[total_intermediate_block_cnt-1], 1);

      free_map_release(disk_inode->doubly_indirect_block, 1);

    }



  }

}


