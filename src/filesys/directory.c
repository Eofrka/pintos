#include "filesys/directory.h"
#include <stdio.h>
#include <string.h>
#include <list.h>
#include "filesys/filesys.h"
#include "threads/malloc.h"

/* pj4 */
/*******/
#include "threads/thread.h"
#include "filesys/free-map.h"
#include "filesys/file.h"
/*******/


/* A single directory entry. */
struct dir_entry 
  {
    disk_sector_t inode_sector;         /* Sector number of header. */
    char name[NAME_MAX + 1];            /* Null terminated file name. */
    bool in_use;                        /* In use or free? */
  };

/* Creates a directory with space for ENTRY_CNT entries in the
   given SECTOR.  Returns true if successful, false on failure. */
bool
dir_create (disk_sector_t sector,disk_sector_t parent_sector, size_t entry_cnt) 
{
  if(inode_create (sector, (entry_cnt+2) * sizeof (struct dir_entry), true) == false)
  {
    return false;
  }

  struct inode* inode = inode_open(sector);
  struct dir* dir;
  if(inode != NULL)
  {
    dir = dir_open(inode);
    if(dir_add(dir, ".", sector) == false || dir_add(dir, "..", parent_sector) == false)
    {
      dir_close(dir);
      return false;
    }
    else
    {
      dir_close(dir);
      return true;
    }
  }
  else
  {
    return false;
  }
}

/* Opens and returns the directory for the given INODE, of which
   it takes ownership.  Returns a null pointer on failure. */
struct dir *
dir_open (struct inode *inode) 
{
  struct dir *dir = calloc (1, sizeof *dir);
  if (inode != NULL && dir != NULL)
    {
      dir->inode = inode;
      dir->pos = 0;
      return dir;
    }
  else
    {
      inode_close (inode);
      free (dir);
      return NULL; 
    }
}

/* Opens the root directory and returns a directory for it.
   Return true if successful, false on failure. */
struct dir *
dir_open_root (void)
{
  return dir_open (inode_open (ROOT_DIR_SECTOR));
}

/* Opens and returns a new directory for the same inode as DIR.
   Returns a null pointer on failure. */
struct dir *
dir_reopen (struct dir *dir) 
{
  return dir_open (inode_reopen (dir->inode));
}

/* Destroys DIR and frees associated resources. */
void
dir_close (struct dir *dir) 
{
  if (dir != NULL)
    {
      inode_close (dir->inode);
      free (dir);
    }
}

/* Returns the inode encapsulated by DIR. */
struct inode *
dir_get_inode (struct dir *dir) 
{
  return dir->inode;
}

/* Searches DIR for a file with the given NAME.
   If successful, returns true, sets *EP to the directory entry
   if EP is non-null, and sets *OFSP to the byte offset of the
   directory entry if OFSP is non-null.
   otherwise, returns false and ignores EP and OFSP. */
static bool
lookup (const struct dir *dir, const char *name,
        struct dir_entry *ep, off_t *ofsp) 
{
  struct dir_entry e;
  size_t ofs;
  
  ASSERT (dir != NULL);
  ASSERT (name != NULL);

  for (ofs = 0; inode_read_at (dir->inode, &e, sizeof e, ofs) == sizeof e;
       ofs += sizeof e) 
    if (e.in_use && !strcmp (name, e.name)) 
      {
        if (ep != NULL)
          *ep = e;
        if (ofsp != NULL)
          *ofsp = ofs;
        return true;
      }
  return false;
}

/* Searches DIR for a file with the given NAME
   and returns true if one exists, false otherwise.
   On success, sets *INODE to an inode for the file, otherwise to
   a null pointer.  The caller must close *INODE. */
bool
dir_lookup (const struct dir *dir, const char *name,
            struct inode **inode) 
{
  struct dir_entry e;

  ASSERT (dir != NULL);
  ASSERT (name != NULL);

  if (lookup (dir, name, &e, NULL))
    *inode = inode_open (e.inode_sector);
  else
    *inode = NULL;

  return *inode != NULL;
}

/* Adds a file named NAME to DIR, which must not already contain a
   file by that name.  The file's inode is in sector
   INODE_SECTOR.
   Returns true if successful, false on failure.
   Fails if NAME is invalid (i.e. too long) or a disk or memory
   error occurs. */
bool
dir_add (struct dir *dir, const char *name, disk_sector_t inode_sector) 
{
  struct dir_entry e;
  off_t ofs;
  bool success = false;
  
  ASSERT (dir != NULL);
  ASSERT (name != NULL);

  /* Check NAME for validity. */
  if (*name == '\0' || strlen (name) > NAME_MAX)
    return false;

  /* Check that NAME is not in use. */
  if (lookup (dir, name, NULL, NULL))
    goto done;

  /* Set OFS to offset of free slot.
     If there are no free slots, then it will be set to the
     current end-of-file.
     
     inode_read_at() will only return a short read at end of file.
     Otherwise, we'd need to verify that we didn't get a short
     read due to something intermittent such as low memory. */
  for (ofs = 0; inode_read_at (dir->inode, &e, sizeof e, ofs) == sizeof e;
       ofs += sizeof e) 
    if (!e.in_use)
      break;

  /* Write slot. */
  e.in_use = true;
  strlcpy (e.name, name, sizeof e.name);
  e.inode_sector = inode_sector;
  success = inode_write_at (dir->inode, &e, sizeof e, ofs) == sizeof e;

 done:
  return success;
}

/* Removes any entry for NAME in DIR.
   Returns true if successful, false on failure,
   which occurs only if there is no file with the given NAME. */
bool
dir_remove (struct dir *dir, const char *name) 
{
  struct dir_entry e;
  struct inode *inode = NULL;
  bool success = false;
  off_t ofs;

  ASSERT (dir != NULL);
  ASSERT (name != NULL);

  /* Find directory entry. */
  if (!lookup (dir, name, &e, &ofs))
    goto done;

  /* Open inode. */
  inode = inode_open (e.inode_sector);
  if (inode == NULL)
    goto done;

  /* Erase directory entry. */
  e.in_use = false;
  if (inode_write_at (dir->inode, &e, sizeof e, ofs) != sizeof e) 
    goto done;

  /* Remove inode. */
  inode_remove (inode);
  success = true;

 done:
  inode_close (inode);
  return success;
}

/* Reads the next directory entry in DIR and stores the name in
   NAME.  Returns true if successful, false if the directory
   contains no more entries. */
bool
dir_readdir (struct dir *dir, char name[NAME_MAX + 1])
{
  struct dir_entry e;

  while (inode_read_at (dir->inode, &e, sizeof e, dir->pos) == sizeof e) 
    {
      dir->pos += sizeof e;
      if (e.in_use)
        {
          strlcpy (name, e.name, NAME_MAX + 1);
          return true;
        } 
    }
  return false;
}

/* pj4 */
/*******/
void remove_redundancy(char* dst, const char* src, size_t buffer_size)
{
  size_t i;
  size_t j=0;

  bool flag=false;
  for(i=0; i<buffer_size; i++)
  {

    if(src[i] == '/' && flag == true)
    {
      continue;
    }
    
    if(src[i] == '/')
    {
      flag=true;
    }
    else
    {
      flag=false;
    }
    dst[j] = src[i];
    j++;
  }


}



bool change_dir(char* path)
{

  struct dir* dir;
  struct inode* inode;

  int start_ofs=0;
  if(path[0] == '/')
  {
    dir = dir_open_root();
    start_ofs = 1;
  }
  else
  {
    dir = dir_reopen(thread_current()->cwd);
  }


  if(dir == NULL)
  {
    return false;
  }


  char* ret_ptr;
  char* next_ptr;
  ret_ptr= strtok_r(path + start_ofs, "/", &next_ptr);

  bool is_exist = false;
  bool is_dir = false;

  while(ret_ptr)
  {
    is_exist = dir_lookup(dir, ret_ptr, &inode);
    dir_close(dir);
    if(is_exist == false)
    {
      return false;
    }
    
    is_dir = inode_is_dir(inode);
    if(is_dir == false)
    {
      return false;
    }

    dir = dir_open(inode);
    if(dir == NULL)
    {
      return false;
    }
    ret_ptr = strtok_r(NULL, "/", &next_ptr);
  }


  dir_close(thread_current()->cwd);
  thread_current()->cwd = dir;
  return true;
}

bool make_dir(char* path)
{
  struct dir* dir;
  struct inode* inode;

  int start_ofs=0;
  if(path[0] == '/')
  {
    dir = dir_open_root();
    start_ofs = 1;
  }
  else
  { 
    dir = dir_reopen(thread_current()->cwd);
  }
  if(dir ==  NULL)
  {
    return false;
  }
  char* ret_ptr;
  char* next_ptr;
  ret_ptr= strtok_r(path + start_ofs, "/", &next_ptr);

  bool is_exist = false;
  bool is_dir = false;

  if(ret_ptr == NULL)
  {
    //can not make root dir
    return false;
  }




  while(ret_ptr)
  {
    if(next_ptr[0] != '\0')
    {
      is_exist = dir_lookup(dir, ret_ptr, &inode);
      dir_close(dir);
      if(is_exist == false)
      {
        return false;
      }

      is_dir = inode_is_dir(inode);
      if(is_dir == false)
      {
        return false;
      }

      dir = dir_open(inode);
      if(dir == NULL)
      {
        return false;
      }
      ret_ptr = strtok_r(NULL, "/", &next_ptr);
    }
    else
    {
      is_exist = dir_lookup(dir, ret_ptr, &inode);
      if(is_exist == true)
      {
        dir_close(dir);
        return false;
      }
      disk_sector_t inode_sector;
      disk_sector_t parent_sector = inode_get_sector(dir->inode);
      bool success = (dir != NULL
        && free_map_allocate (1, &inode_sector)
        && dir_create(inode_sector, parent_sector, 16)
        && dir_add (dir, ret_ptr, inode_sector));
      if (!success && inode_sector != 0) 
        free_map_release (inode_sector, 1);
      dir_close (dir);
      return success;
    }
  }

  PANIC("If reached here, it may have bug on my code.");
  return false;

}


int open_file_or_dir(char* path, struct file** open_file, struct dir** open_dir)
{

  struct dir* dir;
  struct inode* inode;

  int start_ofs=0;
  if(path[0] == '/')
  {
    dir = dir_open_root();
    start_ofs = 1;
  }
  else
  {
    dir = dir_reopen(thread_current()->cwd);
  }

  if(dir == NULL)
  {
    return -1;
  }


  char* ret_ptr;
  char* next_ptr;
  ret_ptr= strtok_r(path + start_ofs, "/", &next_ptr);
  bool is_exist = false;
  bool is_dir = false;

  if(ret_ptr == NULL)
  {
    //open root dir.
    *open_dir = dir;
    return 1;
  }

  while(ret_ptr)
  {
    if(next_ptr[0] != '\0')
    {
      is_exist = dir_lookup(dir, ret_ptr, &inode);
      dir_close(dir);
      if(is_exist == false)
      {
        return -1;
      }

      is_dir = inode_is_dir(inode);
      if(is_dir == false)
      {
        return -1;
      }

      dir = dir_open(inode);
      if(dir == NULL)
      {
        return false;
      }
      ret_ptr = strtok_r(NULL, "/", &next_ptr);
    }
    else
    {
      is_exist =dir_lookup(dir, ret_ptr, &inode);
      dir_close(dir);
      if(is_exist == false)
      {
        return -1;
      }
      is_dir =inode_is_dir(inode);
      if(is_dir == false)
      {
        *open_file= file_open(inode);
        if(*open_file == NULL)
        {
          return -1;
        }
        else
        {
          return 0;
        }
      }
      else
      {
        *open_dir = dir_open(inode);
        if(*open_dir == NULL)
        {
          return -1;
        }
        else
        {
          return 1;
        }
      }
    }
  }

  printf("why???????????\n");
  PANIC("If reached here, it may have bug on my code.");
  return -1;


}


bool create_file(char* path, unsigned initial_size)
{
  struct dir* dir;
  struct inode* inode;

  int start_ofs=0;
  if(path[0] == '/')
  {
    dir = dir_open_root();
    start_ofs = 1;
  }
  else
  {
    dir = dir_reopen(thread_current()->cwd);
  }

  if(dir == NULL)
  {
    return false;
  }

  char* ret_ptr;
  char* next_ptr;
  ret_ptr= strtok_r(path + start_ofs, "/", &next_ptr);
  bool is_exist = false;
  bool is_dir = false;

  if(ret_ptr == NULL)
  {
    //can not create "/".
    return false;
  }

  while(ret_ptr)
  {
    if(next_ptr[0] != '\0')
    {
      is_exist = dir_lookup(dir, ret_ptr, &inode);
      dir_close(dir);
      if(is_exist == false)
      {
        return -1;
      }

      is_dir = inode_is_dir(inode);
      if(is_dir == false)
      {
        return -1;
      }

      dir = dir_open(inode);
      if(dir == NULL)
      {
        return false;
      }
      ret_ptr = strtok_r(NULL, "/", &next_ptr);
    }
    else
    {
      is_exist = dir_lookup(dir,ret_ptr,&inode);
      if(is_exist == true)
      {
        dir_close(dir);
        return false;
      }

      disk_sector_t inode_sector = 0;
      bool success = (dir != NULL
        && free_map_allocate (1, &inode_sector)
        && inode_create (inode_sector, initial_size, false)
        && dir_add (dir, ret_ptr, inode_sector));
      if (!success && inode_sector != 0) 
        free_map_release (inode_sector, 1);
      dir_close (dir);

      return success;
      
    }
  }

  PANIC("If reached here, it may have bug on my code.");
  return false;
}

off_t dir_length(struct dir* dir)
{
  ASSERT (dir!= NULL);
  return inode_length(dir->inode);
}

void dir_seek (struct dir *dir , off_t new_pos)
{
  ASSERT (dir != NULL);
  ASSERT (new_pos >= 0);
  dir->pos = new_pos;
}

off_t dir_tell (struct dir * dir)
{
  ASSERT (dir != NULL);
  return dir->pos;
}
/*******/

