#ifndef FILESYS_DIRECTORY_H
#define FILESYS_DIRECTORY_H

#include <stdbool.h>
#include <stddef.h>
#include "devices/disk.h"

/* pj4 */
/*******/
#include "filesys/file.h"
#include "filesys/inode.h"
#include "filesys/off_t.h"
/*******/
/* Maximum length of a file name component.
   This is the traditional UNIX maximum length.
   After directories are implemented, this maximum length may be
   retained, but much longer full path names must be allowed. */
#define NAME_MAX 14


/* pj4 */
/*******/
/* A directory. */
struct dir 
{
  struct inode *inode;                /* Backing store. */
  off_t pos;                          /* Current position. */
};

/*******/


/* Opening and closing directories. */
bool dir_create (disk_sector_t sector, disk_sector_t parent_sector, size_t entry_cnt);
struct dir *dir_open (struct inode *);
struct dir *dir_open_root (void);
struct dir *dir_reopen (struct dir *);
void dir_close (struct dir *);
struct inode *dir_get_inode (struct dir *);

/* Reading and writing. */
bool dir_lookup (const struct dir *, const char *name, struct inode **);
bool dir_add (struct dir *, const char *name, disk_sector_t);
bool dir_remove (struct dir *, const char *name);
bool dir_readdir (struct dir *, char name[NAME_MAX + 1]);

/* pj4 */
/*******/
void remove_redundancy(char* dst, const char* src, size_t buffer_size);
bool change_dir(char* path);
bool make_dir(char* path);
int open_file_or_dir(char* path, struct file** open_file, struct dir** open_dir);
struct file* open_exec_file(char* path, struct dir* dir, int start_ofs);
bool create_file(char* path,unsigned initial_size);
bool remove_file_or_dir(char* path);

off_t dir_length(struct dir* dir);
void dir_seek (struct dir *dir , off_t new_pos);
off_t dir_tell (struct dir * dir);
/*******/
#endif /* filesys/directory.h */
