#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"


/* pj2 */
/*******/
#include "threads/vaddr.h"
#include "userprog/process.h"
#include "filesys/filesys.h"
#include "threads/malloc.h"
#include "filesys/file.h"
#include "devices/input.h"
#include "threads/init.h"

/* New static function declarations. */
/*****************************************************************************************************************************/
static int get_user(const uint8_t* uaddr); 
static bool put_user(uint8_t* udst, uint8_t byte);
static void* check_uaddr(uint8_t* uaddr, uint32_t bytes);
static void* check_ustr(const char* ustr);
static struct file_descriptor_table_entry* find_fdte(int fd);
/*****************************************************************************************************************************/
/* New staitc function definitions. */
/*****************************************************************************************************************************/
/* Reads a byte at user virtual address UADDR.
   UADDR must be below PHYS_BASE.
   Returns the byte value if successful, -1 if a segfault
   occurred. */
static int
get_user (const uint8_t *uaddr)
{
  if((uint32_t)uaddr >= (uint32_t)PHYS_BASE)
  {
    return -1;
  }
  int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
   : "=&a" (result) : "m" (*uaddr));
  return result;
}

/* Writes BYTE to user address UDST.
   UDST must be below PHYS_BASE.
   Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
  if((uint32_t)udst >= (uint32_t)PHYS_BASE)
  {
    return false;
  }
  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
   : "=&a" (error_code), "=m" (*udst) : "q" (byte));
  return error_code != -1;
}

/* Check user stack address is valid or not. If not valid, call syscall_exit(-1). Else return uaddr.  */
static void* check_uaddr(uint8_t* uaddr, uint32_t bytes)
{
  unsigned i;
  for(i= 0; i<bytes; i++)
  {
    /* If get_user() returns -1, call syscall_exit(-1) to terminate the process. */
    if(get_user(uaddr+i) == -1)
    {
      syscall_exit(-1);
    }
  }
  return (void*)uaddr;

}


/* Check user string address is valid or not. If not valid, call syscall_exit(-1). Else return ustr. */
static void* check_ustr(const char* ustr)
{
  char ch;
  unsigned i=0;
  while( (ch = get_user((uint8_t*)(ustr+i))) != '\0')
  {
    /* If get_user() returns -1, call syscall_exit(-1) to terminate the process. */
    if(ch == -1)
    {
      syscall_exit(-1);
    }
    i++;
  }
  return (void*)ustr;
}


/* Find fdte in fdt. If not found, return NULL. */
static struct file_descriptor_table_entry* find_fdte(int fd)
{
  struct thread* curr = thread_current();
  struct list_elem* iter;
  bool found_fdte = false;
  struct file_descriptor_table_entry* fdte;
  for(iter = list_begin(&curr->fdt); iter != list_end(&curr->fdt); iter = list_next(iter))
  {
    fdte = list_entry(iter, struct file_descriptor_table_entry, elem);
    if(fdte->fd == fd)
    {
      found_fdte = true;
      break;
    }
  }

  if(found_fdte == true)
  {
    return fdte;
  }
  else
  {
    return NULL;
  }

}
/*****************************************************************************************************************************/
/* System call definitions. */
/*****************************************************************************************************************************/
void syscall_halt(void)
{
  power_off();
}

void syscall_exit(int status)
{
  struct thread* curr = thread_current();
  struct process* ps = curr->process;
  if(ps != NULL)
  {
    /* Save exit status. */
    ps->exit_status = status;
    /* Print termination msg. */
    printf("%s: exit(%d)\n", curr->name, ps->exit_status);
  }
  thread_exit();
}

pid_t syscall_exec(const char* cmd_line)
{  
  pid_t ret = process_execute(cmd_line);
  return ret;
}

int syscall_wait(pid_t pid)
{
  return process_wait(pid);
}

bool syscall_create(const char* file, unsigned initial_size)
{
  lock_acquire(&filesys_lock);
  bool ret = filesys_create(file, initial_size);
  lock_release(&filesys_lock);
  return ret;
}

bool syscall_remove (const char *file)
{
  lock_acquire(&filesys_lock);
  bool ret = filesys_remove(file);
  lock_release(&filesys_lock);
  return ret;
}

int syscall_open(const char* file)
{
  int ret;
  struct file* open_file;
  struct file_descriptor_table_entry* fdte;
  lock_acquire(&filesys_lock);
  open_file = filesys_open(file);

  /* If cannot open file name 'file', return -1. */
  if(open_file == NULL)
  {
    lock_release(&filesys_lock);
    return -1;
  }

  struct thread* curr = thread_current();

  /* Allocate file_descriptor_table_entry. */
  fdte = (struct file_descriptor_table_entry*)malloc(sizeof(struct file_descriptor_table_entry));
  if(fdte == NULL)
  {
    PANIC("not enough memory to allocate struct file_descriptor_table_entry");
  }
  fdte->fd = curr->next_fd;
  curr->next_fd++;
  fdte->file = open_file;
  fdte->elem.prev=  NULL;
  fdte->elem.next = NULL;

  /* Push fdte into current thread's file descriptor table(fdt). */
  list_push_back(&curr->fdt, &fdte->elem);
  ret = fdte->fd;

  /* return value should not be 0 or 1. These two values are for STDIN(0), STDOUT(1). */
  ASSERT(ret != 0);
  ASSERT(ret != 1);
  lock_release(&filesys_lock);
  return ret;
}

int syscall_filesize(int fd)
{
  lock_acquire(&filesys_lock);
  struct file_descriptor_table_entry* fdte = find_fdte(fd);
  int ret;
  if(fdte != NULL)
  {
    ret = file_length(fdte->file);
  } 
  else
  {
    /* Q? If fdte == NULL, How to deal with it? */
    ret = -1;
  }
  lock_release(&filesys_lock);
  return ret;
}

int syscall_read(int fd, void* buffer, unsigned size)
{
  int ret= -1;
  /* If fd == 0, read from STDIN(0). Use input_getc() to read from keyboard. */
  if(fd == 0)
  {
    lock_acquire(&filesys_lock);
    unsigned i =0;
    while(i < size)
    {    
      uint8_t key = input_getc();
      /* If buffer's address is not allowed to write, call syscall_exit(-1). */
      if(put_user((uint8_t*)(buffer+i), key) == false)
      {
        lock_release(&filesys_lock);      
        syscall_exit(-1);
      }
      i++;
    }
    ret = (int)i;
    lock_release(&filesys_lock);
    return ret;

  }
  /* Read from STDOUT(1) is not allowed. Just return -1. */
  else if(fd == 1)
  {
    //syscall_exit(-1);
    return -1;
  }
  /* Read from file. */
  else
  {
    lock_acquire(&filesys_lock);
    struct file_descriptor_table_entry* fdte= find_fdte(fd);
    if(fdte != NULL)
    {

      unsigned i =0;
      while(i < size)
      {     
        /* If buffer's address is not allowed to write, call syscall_exit(-1). */
        uint8_t key = get_user((uint8_t*)buffer+i); 
        if(put_user((uint8_t*)(buffer+i), key) == false)
        {
          lock_release(&filesys_lock);      
          syscall_exit(-1);
        }
        i++;
      }
      ret = file_read(fdte->file, buffer, size);
    }
    else
    {
      ret = -1;
    }
    lock_release(&filesys_lock);
    return ret;
  }
}
int syscall_write(int fd, const void* buffer, unsigned size)
{
  int ret = -1;
  /* If fd == 1, write to STDOUT(1). Use putbuf() to write to the console. */
  if(fd == 1)
  {
    lock_acquire(&filesys_lock);
    unsigned i =0;
    while(i < size)
    {
      putbuf(buffer+i, 1);
      i++;
    }
    ret = (int)i;
    lock_release(&filesys_lock);
    return ret;
  }
  /* It is not allowed to write to the STDIN(1). Just return -1. */
  else if( fd == 0)
  {
    //syscall_exit(-1);
    return -1;
  }
  /* Write to the file. */
  else
  {
    lock_acquire(&filesys_lock);
    struct file_descriptor_table_entry* fdte = find_fdte(fd);
    if(fdte != NULL)
    {
      ret = file_write(fdte->file, buffer, size);
    }
    else
    {
      ret = -1;
    }
    lock_release(&filesys_lock);
    return ret;
  }
}

void syscall_seek(int fd, unsigned position)
{
  lock_acquire(&filesys_lock);
  struct file_descriptor_table_entry* fdte = find_fdte(fd);
  if(fdte != NULL)
  {
    file_seek(fdte->file, position); 
  }
  lock_release(&filesys_lock);
  /* Q? If fdte == NULL, How to deal with it? */
}

unsigned syscall_tell(int fd)
{
  lock_acquire(&filesys_lock);
  struct file_descriptor_table_entry* fdte = find_fdte(fd);
  unsigned ret;
  if(fdte != NULL)
  {
    ret = file_tell(fdte->file);

  }
  else
  {
    /* Q? If fdte == NULL, How to deal with it? */
    ret = 0xffffffff;
  }
  lock_release(&filesys_lock);
  return ret;
}

void syscall_close(int fd)
{
  struct thread* curr = thread_current();
  struct list_elem* iter;
  struct file_descriptor_table_entry* fdte;
  bool found_fdte = false;

  /* Find file descriptor table entry which fd is equal to parameter 'fd' in current thread's file descriptor table. If found fdte, remove fdte from the fdt, decrease thread's next_fd one, and
  close the file. Finally free the fdte which allocated at syscall_open(). Else, Do nothing. */
  lock_acquire(&filesys_lock);
  for(iter = list_begin(&curr->fdt); iter != list_end(&curr->fdt); iter= list_next(iter))
  {
    fdte = list_entry(iter, struct file_descriptor_table_entry, elem);
    if(fdte->fd == fd)
    {
      found_fdte = true;
      break;
    }
  }
  if(found_fdte == true)
  {
    list_remove(iter);
    curr->next_fd--;    
    file_close(fdte->file);
    SAFE_FREE(fdte);

  }
  lock_release(&filesys_lock);
}

/****************************************************************************************************************************/
/*******/



static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&filesys_lock);
}

static void
syscall_handler (struct intr_frame *f) 
{
  
  /* pj2 */
  /*******/
  //printf ("system call!\n");
  uint8_t* esp = (uint8_t*)f->esp;
  int syscall_num = *(int*)check_uaddr(esp, 4);
/* pj3 */
/*******/
#ifdef VM
  /* Save esp for kernel mode page fault. */
  thread_current()->esp = esp;
#endif
/*******/
  void* arg1 = NULL;
  void* arg2 = NULL;
  void* arg3 = NULL;
  //printf("syscall_num: %d\n", syscall_num);
  switch(syscall_num)
  {
    /* PJ2 */
    /********/
    case SYS_HALT:
    syscall_halt();
    break;
    case SYS_EXIT:
    arg1 = check_uaddr(esp+4, 4);
    int status = *(int*)arg1;
    syscall_exit(status);
    break;
    case SYS_EXEC:
    arg1 = check_uaddr(esp+4, 4);
    const char* cmd_line = (char*)*(uint32_t*)arg1;
    check_ustr(cmd_line);
    f->eax = (uint32_t)syscall_exec(cmd_line);
    break;
    case SYS_WAIT:
    arg1 = check_uaddr(esp+4, 4);
    pid_t pid = *(pid_t*)arg1;
    f->eax = (uint32_t)syscall_wait(pid);
    break;
    case SYS_CREATE:
    arg1 = check_uaddr(esp+4, 4);
    const char* file =(char*)*(uint32_t*)arg1;
    check_ustr(file);
    arg2 = check_uaddr(esp+8, 4);
    unsigned int size = *(unsigned int*)arg2;
    f->eax = (uint32_t)syscall_create(file, size);
    break;
    case SYS_REMOVE:
    arg1 = check_uaddr(esp+4, 4);
    file = (char*)*(uint32_t*)arg1;
    check_ustr(file);
    f->eax = (uint32_t)syscall_remove(file);
    break;
    case SYS_OPEN:
    arg1 = check_uaddr(esp+4, 4);
    file = (char*)*(uint32_t*)arg1;
    check_ustr(file);
    f->eax = (uint32_t)syscall_open(file);
    break;
    case SYS_FILESIZE:
    arg1 = check_uaddr(esp+4, 4);
    int fd = *(int*)arg1;
    f->eax = (uint32_t)syscall_filesize(fd);
    break;
    case SYS_READ:
    arg1 = check_uaddr(esp+4, 4);
    fd = *(int*)arg1;
    arg2 = check_uaddr(esp+8, 4);
    void* buf = (void*)*(uint32_t*)arg2;
    arg3 = check_uaddr(esp+12, 4);
    size = *(unsigned*)arg3;
    check_uaddr((uint8_t*)buf, size);
    f->eax = (uint32_t)syscall_read(fd, buf, size);
    break;
    case SYS_WRITE:
    arg1 =check_uaddr(esp+4, 4);
    fd = *(int*)arg1;
    arg2 = check_uaddr(esp+8, 4);
    const void* buffer = (void*)*(uint32_t*)arg2;
    arg3 = check_uaddr(esp+12, 4);
    size = *(unsigned*)arg3;
    check_uaddr((uint8_t*)buffer, size);
    f->eax = (uint32_t)syscall_write(fd, buffer, size);
    break;
    case SYS_SEEK:
    arg1 = check_uaddr(esp+4, 4);
    fd = *(int*)arg1;
    arg2 = check_uaddr(esp+8, 4);
    unsigned position = *(unsigned*)arg2;
    syscall_seek(fd, position);
    break;
    case SYS_TELL:
    arg1 = check_uaddr(esp+4, 4);
    fd = *(int*)arg1;
    f->eax =(uint32_t)syscall_tell(fd);
    break;
    case SYS_CLOSE:
    arg1 = check_uaddr(esp+4, 4);
    fd = *(int*)arg1;
    syscall_close(fd);
    break;
    default:
    syscall_exit(-1);
    /********/


  }


  //thread_exit();
  /*******/
}
