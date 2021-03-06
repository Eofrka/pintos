#include "userprog/process.h"
#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userprog/gdt.h"
#include "userprog/pagedir.h"
#include "userprog/tss.h"
#include "filesys/directory.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/flags.h"
#include "threads/init.h"
#include "threads/interrupt.h"
#include "threads/palloc.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

/* pj2 */
/*******/
#include "threads/malloc.h"
#include "userprog/syscall.h"
/*******/

/* pj3 */
/*******/
#ifdef VM
#include "vm/page.h"
#include "vm/frame.h"
#endif
/*******/

/* pj2 */
/*******/
/* User program arguments. */
struct arguments
{
  int argc;
  char* argv[64];
};
/*******/




static thread_func start_process NO_RETURN;
static bool load (char *cmdline, void (**eip) (void), void **esp);

/* New static function declarations. */
/*************************************************************************************************************************/
static void parse_arguments(char* str, struct arguments* args);
static void push_arguments(struct arguments* args, void** esp);
/*************************************************************************************************************************/


/* Starts a new thread running a user program loaded from
   FILENAME.  The new thread may be scheduled (and may even exit)
   before process_execute() returns.  Returns the new process's
   thread id, or TID_ERROR if the thread cannot be created. */
pid_t
process_execute (const char *cmdline) 
{
  /* pj2 */
  /*******/
  //lock_acquire(&filesys_lock);
  /*******/
  char *fn_copy;
  tid_t tid;

  /* Make a copy of FILE_NAME.
     Otherwise there's a race between the caller and load(). */
  fn_copy = palloc_get_page (0);
  strlcpy (fn_copy, cmdline, PGSIZE);

  /* pj2 */
  /*******/
  /* Not deal with file name > 16 */
  char* next;
  char* token = strtok_r(fn_copy, " ", &next);
    
  char file_name[16]={0,};
  size_t buffer_size = strlen(token) + 1;
  char* path = (char*)calloc(buffer_size, sizeof(char));
  remove_redundancy(path, token, buffer_size);
  
  int start_ofs = 0;
  if(path[0] == '/')
  {
    start_ofs=0;
  }
  
  char* ret_ptr;
  char* next_ptr;
  ret_ptr= strtok_r(path + start_ofs, "/", &next_ptr);

  while(ret_ptr)
  {
     if(next_ptr[0] != '\0')
     {
        ret_ptr = strtok_r(NULL, "/", &next_ptr);
     }
     else
     {
        strlcpy(file_name, ret_ptr, 16); 
        break;
     }
  }
  
  strlcpy (fn_copy, cmdline, PGSIZE);
  /*******/

   /* pj2 */
  /*******/
  /* Set current thread's load flag to false. */
  struct thread* curr = thread_current();
  curr->load = false;
  ASSERT(curr->load == false);
  /*******/


  /* Create a new thread to execute FILE_NAME. */
  tid = thread_create (file_name, PRI_DEFAULT, start_process, fn_copy);
  SAFE_FREE(path);
  /* pj2 */
  /*******/
  /* Wait loading in start_process(). */
  sema_down(&curr->sema_exec);
  /*******/
  if (tid == TID_ERROR)
    palloc_free_page (fn_copy); 

  /* pj2 */
  /*******/
  /* If load succeeded, return child thread's tid(pid), Else return PID_ERROR.
  Before return, release filesys_lock. */
  if(curr->load == true)
  {
    //lock_release(&filesys_lock);
    return tid;
  }
  else
  {
    //lock_release(&filesys_lock);
    return PID_ERROR;
  }
  /*******/
}

/* A thread function that loads a user process and makes it start
   running. */
static void
start_process (void *cmdline_)
{
  char *cmdline = cmdline_;
  struct intr_frame if_;
  bool success;

  /* Initialize interrupt frame and load executable. */
  memset (&if_, 0, sizeof if_);
  if_.gs = if_.fs = if_.es = if_.ds = if_.ss = SEL_UDSEG;
  if_.cs = SEL_UCSEG;
  if_.eflags = FLAG_IF | FLAG_MBS;
  success = load (cmdline, &if_.eip, &if_.esp);
  palloc_free_page (cmdline);
  /* pj2 */
  /*******/
  struct thread* curr = thread_current();
  struct process* ps = curr->process;
  /* If loading is not succeeded. */
  if (!success)
  { 
    /* Set pid to PID_ERROR. */
    ps->pid = PID_ERROR;
    /* Set exit_status to some value. */
    ps->exit_status = -1;
    /* Set parent thread's load flag to false. */
    ps->parent->load = false;

    /* Wake up parent's sema_exec. */
    sema_up(&ps->parent->sema_exec);
    /* Call thread_exit(). (3.3.2 The process termination message is optional when a process fails to load. */
    thread_exit ();
  }
  /* Loading is succeeded. */
  else
  {
    /* Set pid to current thread's tid. */
    ps->pid = curr->tid;
    /* Push this process information into parent thread's child list. */
    list_push_back(&ps->parent->child_list, &ps->elem);
    /* Set parent thread's load flag to true. */
    ps->parent->load = true;
    /* Awake parent thread's sema_exec. */
    /* pj4 */
    /*******/
    
    if(ps->parent != NULL && ps->parent->cwd == NULL)
    {
      lock_acquire(&filesys_lock);
      curr->cwd = dir_open_root();
      lock_release(&filesys_lock);
    }
    else
    {
      lock_acquire(&filesys_lock);
      curr->cwd = dir_reopen(ps->parent->cwd);
      lock_release(&filesys_lock);

    }
    /********/
    sema_up(&ps->parent->sema_exec);
  }
  /*******/

  /* Start the user process by simulating a return from an
     interrupt, implemented by intr_exit (in
     threads/intr-stubs.S).  Because intr_exit takes all of its
     arguments on the stack in the form of a `struct intr_frame',
     we just point the stack pointer (%esp) to our stack frame
     and jump to it. */
  asm volatile ("movl %0, %%esp; jmp intr_exit" : : "g" (&if_) : "memory");
  NOT_REACHED ();
}

/* Waits for thread TID to die and returns its exit status.  If
   it was terminated by the kernel (i.e. killed due to an
   exception), returns -1.  If TID is invalid or if it was not a
   child of the calling process, or if process_wait() has already
   been successfully called for the given TID, returns -1
   immediately, without waiting.

   This function will be implemented in problem 2-2.  For now, it
   does nothing. */
int
process_wait (pid_t child_pid) 
{
/* pj2 */
  /*******/
  struct thread* curr = thread_current();
  struct process* ps = curr->process;

  /* The process which call process_wait should not be NULL. */
  ASSERT(ps != NULL);

  /* Set ps->waiting to true if is is false. Else return -1 immediately. */
  if(ps->waiting == false)
  {
    ps->waiting = true;
  }
  else
  {
    /* Already has called process_wait(). Return -1 immediately. */
    return -1;
  }


  /* Searching child_pid which pid is equal to child_pid. */
  struct list_elem* iter;
  struct process* cps;
  bool found_child = false;
  for(iter = list_begin(&curr->child_list); iter != list_end(&curr->child_list); iter = list_next(iter))
  {
    cps = list_entry(iter, struct process, elem);
    if(cps->pid == child_pid)
    {
      found_child = true;
      break;
    }
  }



  /* If found target (child) process, Check its zombie flag whether it is exited or not. */
  if(found_child)
  {

    sema_down(&cps->sema_wait);
      //Q? How to deal with kill by kernel case? I modified exception.c@kill() function.

      /* My modification: Call syscall_exit(-1) instead thread_exit().
      Does it(My modification of kill() function) match well to document's wait() system call specification?? 
      => "If pid did not call exit(), but was terminated by the kernel(e.g. killed due to an exception),
       wait(pid) must return -1." */


       /* I think it matches to document spec because when kill() is called, syscall_exit(-1) is called. Then print termination message and set process's exit_status to -1. Then call thread_exit() and inside thread_exit(), process_exit() is called. In process_exit(), it changes process's zombie flag to true. Therefore, the process which called process_wait() does not sema_down(&cps->sema_wait) and jumps to below.
       Below code removes child process from child_list, frees the child process, changes waiting flag of process which called process_wait to false, and returns the exit status of it which is -1 when kill() is called. */

    
    ASSERT(cps->zombie == true);

    /* Remove child process's information from the current thread's child_list. */
    list_remove(iter);
    /* Save child process's exit_status. */
    int exit_status = cps->exit_status;
    /* Resource dealloaction. */
    SAFE_FREE(cps);
    /* Restore current thread's waiting flag. */
    ps->waiting = false;
    return exit_status;
  }
  /* If target (child) process is not found, restore the waiting flag and return -1. */
  else
  {
    ps->waiting = false;
    return -1;
  }
  /*******/
}

/* Free the current process's resources. */
void
process_exit (void)
{
  /* pj2 */
  /*******/
  struct thread *curr = thread_current ();
  struct process* ps = curr->process;
  ASSERT(ps != NULL); 
  /* Set process's status into zombie. The 'struct process' will be reaped in process_wait(). */
  ps->zombie = true;
  
  /* Check current thread's child_list, and pop the all child processes. 
  If child process's status is zombie, FREE it. Else make it orphan. */
  struct list_elem* iter;
  struct process* cps;
  for(iter = list_begin(&curr->child_list); iter != list_end(&curr->child_list); iter = list_next(iter))
  {
    cps = list_entry(list_pop_front(&curr->child_list), struct process, elem);
    if(cps->zombie == true)
    {
      /* Frees already exited child process. */
      SAFE_FREE(cps);
    }
    else
    {
      /* Make child to orphan. */
      cps->parent = NULL;
    }
  }

/* pj3 */
/*******/
#ifdef VM

  /* Unmap entire mmap table entries implicitly when process is terminated. */
  while(!list_empty(&curr->mmap_table))
  {
    syscall_munmap(curr->next_mapid-1);
  }

/*******/  
#endif


  /* Close entire file descriptor entries implicitly when process is terminated. */
  while(!list_empty(&curr->fdt))
  {
    syscall_close(curr->next_fd-1);
  }

  dir_close(curr->cwd);

  /* Call file_close() to call file_allow_write(). This intervene filesystem, 
  so lock_acquire() and lock_release() needed! */
  if(curr->executable != NULL)
  {
    lock_acquire(&filesys_lock);
    file_close(curr->executable);
    lock_release(&filesys_lock);
  }
  uint32_t *pd;

/* pj3 */
/*******/
#ifdef VM
  /* Destroy supplemental page table. */
  spt_destroy(&curr->spt);

/*******/  
#endif

  /* Destroy the current process's page directory and switch back
     to the kernel-only page directory. */
  pd = curr->pagedir;
  if (pd != NULL) 
  {
      /* Correct ordering here is crucial.  We must set
         curr->pagedir to NULL before switching page directories,
         so that a timer interrupt can't switch back to the
         process page directory.  We must activate the base page
         directory before destroying the process's page
         directory, or our active page directory will be one
         that's been freed (and cleared). */
    curr->pagedir = NULL;
    pagedir_activate (NULL);
    pagedir_destroy (pd);
  }

  /* If current process is an orphan process, free its 'struct process'. There are no parent process waiting it to reap... */
  if(ps->parent == NULL)
  {
    SAFE_FREE(ps);
  }
  /* Else, call sema_up(&ps->sema_wait) for blocking system call wait(). */
  else
  {
    sema_up(&ps->sema_wait);
  }
  /*******/
}

/* Sets up the CPU for running user code in the current
   thread.
   This function is called on every context switch. */
void
process_activate (void)
{
  struct thread *t = thread_current ();

  /* Activate thread's page tables. */
  pagedir_activate (t->pagedir);

  /* Set thread's kernel stack for use in processing
     interrupts. */
  tss_update ();
}

/* We load ELF binaries.  The following definitions are taken
   from the ELF specification, [ELF1], more-or-less verbatim.  */

/* ELF types.  See [ELF1] 1-2. */
typedef uint32_t Elf32_Word, Elf32_Addr, Elf32_Off;
typedef uint16_t Elf32_Half;

/* For use with ELF types in printf(). */
#define PE32Wx PRIx32   /* Print Elf32_Word in hexadecimal. */
#define PE32Ax PRIx32   /* Print Elf32_Addr in hexadecimal. */
#define PE32Ox PRIx32   /* Print Elf32_Off in hexadecimal. */
#define PE32Hx PRIx16   /* Print Elf32_Half in hexadecimal. */

/* Executable header.  See [ELF1] 1-4 to 1-8.
   This appears at the very beginning of an ELF binary. */
struct Elf32_Ehdr
{
  unsigned char e_ident[16];
  Elf32_Half    e_type;
  Elf32_Half    e_machine;
  Elf32_Word    e_version;
  Elf32_Addr    e_entry;
  Elf32_Off     e_phoff;
  Elf32_Off     e_shoff;
  Elf32_Word    e_flags;
  Elf32_Half    e_ehsize;
  Elf32_Half    e_phentsize;
  Elf32_Half    e_phnum;
  Elf32_Half    e_shentsize;
  Elf32_Half    e_shnum;
  Elf32_Half    e_shstrndx;
};

/* Program header.  See [ELF1] 2-2 to 2-4.
   There are e_phnum of these, starting at file offset e_phoff
   (see [ELF1] 1-6). */
struct Elf32_Phdr
{
  Elf32_Word p_type;
  Elf32_Off  p_offset;
  Elf32_Addr p_vaddr;
  Elf32_Addr p_paddr;
  Elf32_Word p_filesz;
  Elf32_Word p_memsz;
  Elf32_Word p_flags;
  Elf32_Word p_align;
};

/* Values for p_type.  See [ELF1] 2-3. */
#define PT_NULL    0            /* Ignore. */
#define PT_LOAD    1            /* Loadable segment. */
#define PT_DYNAMIC 2            /* Dynamic linking info. */
#define PT_INTERP  3            /* Name of dynamic loader. */
#define PT_NOTE    4            /* Auxiliary info. */
#define PT_SHLIB   5            /* Reserved. */
#define PT_PHDR    6            /* Program header table. */
#define PT_STACK   0x6474e551   /* Stack segment. */

/* Flags for p_flags.  See [ELF3] 2-3 and 2-4. */
#define PF_X 1          /* Executable. */
#define PF_W 2          /* Writable. */
#define PF_R 4          /* Readable. */

static bool setup_stack (void **esp, struct arguments* args);
static bool validate_segment (const struct Elf32_Phdr *, struct file *);
static bool load_segment (struct file *file, off_t ofs, uint8_t *upage,
  uint32_t read_bytes, uint32_t zero_bytes,
  bool writable);

/* Loads an ELF executable from FILE_NAME into the current thread.
   Stores the executable's entry point into *EIP
   and its initial stack pointer into *ESP.
   Returns true if successful, false otherwise. */
bool
load (char *cmdline, void (**eip) (void), void **esp) 
{
  struct thread *t = thread_current ();
  struct Elf32_Ehdr ehdr;
  struct file *file = NULL;
  off_t file_ofs;
  bool success = false;
  int i;
  int start_ofs = 0;
  size_t buffer_size = 0;
  char* path=NULL;
  struct dir* dir = NULL;

/* pj2 */
  /*******/
  /* Basic setting for argument pushing on the stack. */
  struct arguments* args = (struct arguments*)malloc(sizeof(struct arguments));
  if(args == NULL)
  {
    goto done;
  }
  /* Actual parsing. */
  parse_arguments(cmdline, args);
  /* Set file_name. */
  const char* file_name = args->argv[0];
  /*******/

  /* Allocate and activate page directory. */
  t->pagedir = pagedir_create ();
  if (t->pagedir == NULL) 
    goto done;
  process_activate ();

/* pj3 */
/*******/  
#ifdef VM
  /* Initialize supplemental page table. */
  spt_init(&t->spt);
#endif  
/*******/  

  /* Open executable file. */
  lock_acquire(&filesys_lock);
  buffer_size = strlen(file_name) + 1; 
  path = (char*)malloc(sizeof(char) * buffer_size);
  strlcpy(path, file_name, buffer_size);
  
  if(file_name[0] == '/')
  {
    dir = dir_open_root();
    start_ofs = 1;
  }
  else
  {
    if(t->process->parent->tid == 1)
    {
      dir = dir_open_root();
    }
    else
    {
      dir= dir_reopen(t->process->parent->cwd);
    }
  }

  if(dir == NULL)
  {
    printf("load: directory open failed\n");
    goto done;
  }

  //t->cwd = dir;

  //open_file_or_dir(file_name, &file, NULL);
  file = open_exec_file(path, dir, start_ofs);
  //file = filesys_open(file_name);
  if (file == NULL) 
  {
    printf ("load: %s: open failed\n", file_name);
    goto done; 
  }

  /* Read and verify executable header. */
  if (file_read (file, &ehdr, sizeof ehdr) != sizeof ehdr
    || memcmp (ehdr.e_ident, "\177ELF\1\1\1", 7)
    || ehdr.e_type != 2
    || ehdr.e_machine != 3
    || ehdr.e_version != 1
    || ehdr.e_phentsize != sizeof (struct Elf32_Phdr)
    || ehdr.e_phnum > 1024) 
  {
    printf ("load: %s: error loading executable\n", file_name);
    goto done; 
  }

  /* Read program headers. */
  file_ofs = ehdr.e_phoff;
  for (i = 0; i < ehdr.e_phnum; i++) 
  {
    struct Elf32_Phdr phdr;

    if (file_ofs < 0 || file_ofs > file_length (file))
      goto done;
    file_seek (file, file_ofs);

    if (file_read (file, &phdr, sizeof phdr) != sizeof phdr)
      goto done;
    file_ofs += sizeof phdr;
    switch (phdr.p_type) 
    {
      case PT_NULL:
      case PT_NOTE:
      case PT_PHDR:
      case PT_STACK:
      default:
          /* Ignore this segment. */
      break;
      case PT_DYNAMIC:
      case PT_INTERP:
      case PT_SHLIB:
      goto done;
      case PT_LOAD:
      if (validate_segment (&phdr, file)) 
      {
        bool writable = (phdr.p_flags & PF_W) != 0;
        uint32_t file_page = phdr.p_offset & ~PGMASK;
        uint32_t mem_page = phdr.p_vaddr & ~PGMASK;
        uint32_t page_offset = phdr.p_vaddr & PGMASK;
        uint32_t read_bytes, zero_bytes;
        if (phdr.p_filesz > 0)
        {
                  /* Normal segment.
                     Read initial part from disk and zero the rest. */
          read_bytes = page_offset + phdr.p_filesz;
          zero_bytes = (ROUND_UP (page_offset + phdr.p_memsz, PGSIZE)
            - read_bytes);
        }
        else 
        {
                  /* Entirely zero.
                     Don't read anything from disk. */
          read_bytes = 0;
          zero_bytes = ROUND_UP (page_offset + phdr.p_memsz, PGSIZE);
        }
        if (!load_segment (file, file_page, (void *) mem_page,
         read_bytes, zero_bytes, writable))
          goto done;
      }
      else
        goto done;
      break;
    }
  }

  /* Set up stack. */
  if (!setup_stack (esp,args))
    goto done;

  /* Start address. */
  *eip = (void (*) (void)) ehdr.e_entry;

  success = true;

  /* pj2 */
  /*******/
  /* 3.3.5 Denying writes to executables.
  In process_execute, I wrapped it with lock_acqurie(&filesys_lock) and lock_release(&filesys_lock).
  Therefore accessing file system is safe. */
  struct thread* curr = thread_current();
  curr->executable = file;
  file_deny_write(curr->executable);
  /*******/
  done:
  /* We arrive here whether the load is successful or not. */

  /* pj2 */
  /*******/

  /* Free args */
  SAFE_FREE(args);
  SAFE_FREE(path);
  /* If failed, call file_close(file). */
  if(success != true )
  {
    file_close(file);
  }
  /*******/
  lock_release(&filesys_lock);
  return success;
}

/* load() helpers. */

static bool install_page (void *upage, void *kpage, bool writable);

/* Checks whether PHDR describes a valid, loadable segment in
   FILE and returns true if so, false otherwise. */
static bool
validate_segment (const struct Elf32_Phdr *phdr, struct file *file) 
{
  /* p_offset and p_vaddr must have the same page offset. */
  if ((phdr->p_offset & PGMASK) != (phdr->p_vaddr & PGMASK)) 
    return false; 

  /* p_offset must point within FILE. */
  if (phdr->p_offset > (Elf32_Off) file_length (file)) 
    return false;

  /* p_memsz must be at least as big as p_filesz. */
  if (phdr->p_memsz < phdr->p_filesz) 
    return false; 

  /* The segment must not be empty. */
  if (phdr->p_memsz == 0)
    return false;
  
  /* The virtual memory region must both start and end within the
     user address space range. */
  if (!is_user_vaddr ((void *) phdr->p_vaddr))
    return false;
  if (!is_user_vaddr ((void *) (phdr->p_vaddr + phdr->p_memsz)))
    return false;

  /* The region cannot "wrap around" across the kernel virtual
     address space. */
  if (phdr->p_vaddr + phdr->p_memsz < phdr->p_vaddr)
    return false;

  /* Disallow mapping page 0.
     Not only is it a bad idea to map page 0, but if we allowed
     it then user code that passed a null pointer to system calls
     could quite likely panic the kernel by way of null pointer
     assertions in memcpy(), etc. */
  if (phdr->p_vaddr < PGSIZE)
    return false;

  /* It's okay. */
  return true;
}

/* Loads a segment starting at offset OFS in FILE at address
   UPAGE.  In total, READ_BYTES + ZERO_BYTES bytes of virtual
   memory are initialized, as follows:

        - READ_BYTES bytes at UPAGE must be read from FILE
          starting at offset OFS.

        - ZERO_BYTES bytes at UPAGE + READ_BYTES must be zeroed.

   The pages initialized by this function must be writable by the
   user process if WRITABLE is true, read-only otherwise.

   Return true if successful, false if a memory allocation error
   or disk read error occurs. */
static bool
load_segment (struct file *file, off_t ofs, uint8_t *upage,
  uint32_t read_bytes, uint32_t zero_bytes, bool writable) 
{
  ASSERT ((read_bytes + zero_bytes) % PGSIZE == 0);
  ASSERT (pg_ofs (upage) == 0);
  ASSERT (ofs % PGSIZE == 0);

  file_seek (file, ofs);

/* pj3 */
/*******/
#ifdef VM
  struct thread* curr = thread_current();
  uint32_t* pagedir = curr->pagedir;
  while (read_bytes > 0 || zero_bytes > 0) 
  {
    /* Calculate how to fill this page.
       We will read PAGE_READ_BYTES bytes from FILE
       and zero the final PAGE_ZERO_BYTES bytes. */
    size_t page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
    size_t page_zero_bytes = PGSIZE - page_read_bytes;

    /* 1. Allocate spte. */
    struct supplemental_page_table_entry* spte = spte_create();
    if(spte == NULL)
    {
      return false;
    }
    /* 2. Initilize spte. */
    spte->upage = upage;
    spte->state = SPTE_FILE;
    spte->pagedir = pagedir;
    spte->writable = writable;
    spte->file = file;
    spte->ofs = ofs;
    spte->page_read_bytes = page_read_bytes;
    spte->page_zero_bytes = page_zero_bytes;
    
    /* 3. Insert spte into spt. */
    if(!spte_insert(&curr->spt, spte))
    {
      SAFE_FREE(spte);
      return false;
    }    

    /* Advance. */
    read_bytes -= page_read_bytes;
    zero_bytes -= page_zero_bytes;
    ofs += page_read_bytes;
    upage += PGSIZE;

  }
  return true;
#endif   
/*******/
  while (read_bytes > 0 || zero_bytes > 0) 
  {
      /* Do calculate how to fill this page.
         We will read PAGE_READ_BYTES bytes from FILE
         and zero the final PAGE_ZERO_BYTES bytes. */
    size_t page_read_bytes = read_bytes < PGSIZE ? read_bytes : PGSIZE;
    size_t page_zero_bytes = PGSIZE - page_read_bytes;

      /* Get a page of memory. */
    uint8_t *kpage = palloc_get_page (PAL_USER);
    if (kpage == NULL)
      return false;

      /* Load this page. */
    if (file_read (file, kpage, page_read_bytes) != (int) page_read_bytes)
    {
      palloc_free_page (kpage);
      return false; 
    }
    memset (kpage + page_read_bytes, 0, page_zero_bytes);

      /* Add the page to the process's address space. */
    if (!install_page (upage, kpage, writable)) 
    {
      palloc_free_page (kpage);
      return false; 
    }

      /* Advance. */
    read_bytes -= page_read_bytes;
    zero_bytes -= page_zero_bytes;
    upage += PGSIZE;
  }
  return true;
}

/* Create a minimal stack by mapping a zeroed page at the top of
   user virtual memory. */
static bool
setup_stack (void **esp, struct arguments* args) 
{

/* pj3 */
/*******/
#ifdef VM
  /* 1. Create a stack spte. */
  struct supplemental_page_table_entry* spte = spte_create();
  if(spte == NULL)
  {
    return false;
  }

  /* 2. Initialize the spte. */
  struct thread* curr  = thread_current();
  spte->upage = (void*)((uint32_t)PHYS_BASE - (uint32_t)PGSIZE);
  spte->state = SPTE_ZERO;
  spte->pagedir = curr->pagedir;
  spte->writable = true;
  spte->page_zero_bytes = (size_t)PGSIZE;
  spte->is_stack_page = true;

  /* 3. Insert the spte into spt. */
  if(!spte_insert(&curr->spt, spte))
  {
    SAFE_FREE(spte);
    return false;
  }

  /* 4. Obtain frame. */
  lock_acquire(&frame_lock);
  struct frame_table_entry* fte = fte_obtain(spte,PAL_USER);
  if(fte == NULL)
  {
    hash_delete(&curr->spt, &spte->h_elem);
    SAFE_FREE(spte);
    lock_release(&frame_lock);
    return false;
  }

  /* 5. Fetch frame. */
  if(!fte_fetch(fte, spte))
  {
    /* May not occur. */
    list_remove(&fte->elem);
    palloc_free_page(fte->kpage);
    SAFE_FREE(fte);

    hash_delete(&curr->spt, &spte->h_elem);
    SAFE_FREE(spte);
    lock_release(&frame_lock);
    return false;
  }

  /* 6. Install the page into original pintos's page table.
        swapped_in page's dirty bit must be cleared because of pagedir_set_page(). */
  if(!fte_install(fte, spte))
  {
    list_remove(&fte->elem);
    palloc_free_page(fte->kpage);
    SAFE_FREE(fte);
    

    hash_delete(&curr->spt, &spte->h_elem);
    SAFE_FREE(spte);
    lock_release(&frame_lock);
    return false;
  }
  else
  {
    /* 7. Finally, update the spte's state into SPTE_FRAME. */
    spte->state = SPTE_FRAME;

    lock_release(&frame_lock);
    /* 8. Push arguments into user stack. */
    push_arguments(args, esp);

    return true;
  }
  
#endif
/******/
  uint8_t *kpage;
  bool success = false;  
  kpage = palloc_get_page (PAL_USER | PAL_ZERO);
  if (kpage != NULL) 
  {
    success = install_page (((uint8_t *) PHYS_BASE) - PGSIZE, kpage, true);
    if (success)
    {
      /* pj2 */
      /*******/
      push_arguments(args, esp);
      //hex_dump((uintptr_t)(*esp), *esp, PHYS_BASE - (*esp), true);
      /*******/
    }
    else
      palloc_free_page (kpage);
  }
  return success;
}

/* Adds a mapping from user virtual address UPAGE to kernel
   virtual address KPAGE to the page table.
   If WRITABLE is true, the user process may modify the page;
   otherwise, it is read-only.
   UPAGE must not already be mapped.
   KPAGE should probably be a page obtained from the user pool
   with palloc_get_page().
   Returns true on success, false if UPAGE is already mapped or
   if memory allocation fails. */
static bool
install_page (void *upage, void *kpage, bool writable)
{
  struct thread *t = thread_current ();

  /* Verify that there's not already a page at that virtual
     address, then map our page there. */
  return (pagedir_get_page (t->pagedir, upage) == NULL
    && pagedir_set_page (t->pagedir, upage, kpage, writable));
}


/* New static function definitions. */
/*************************************************************************************************************************/
/* Parses the str with deliminator " ". */
static void parse_arguments(char* str, struct arguments* args)
{
  char* next;
  int argc =0;
  const char* delim = " ";
  args->argv[argc] = strtok_r(str, delim , &next);
  while(argc < 64 && args->argv[argc])
  {
    //printf("argv[%d]: [%s]\n", argc, args->argv[argc]); 
    argc++;
    args->argv[argc] = strtok_r(NULL, delim, &next);
  }
  args->argc = argc;
}

/* Push arguments into the stack. */
static void push_arguments(struct arguments* args, void** esp)
{
  *esp = PHYS_BASE;
  uint8_t* cur =(uint8_t*)*esp;

  /* String pushing. */
  int i;
  int argc = args->argc;
  int len = 0;
  for(i=argc-1; i>=0; i--)
  {
    len = strlen(args->argv[i])+1;
    cur = cur-len;
    memcpy(cur, args->argv[i], len);
    args->argv[i] = (char*)cur; 
  }
  /* Word-align */
  uint32_t tmp_addr = (uint32_t)cur;
  uint8_t* aligned_cur = (uint8_t*)(tmp_addr & -4);
  uint32_t padding_size = (uint32_t)(cur - aligned_cur);
  cur = aligned_cur;
  memset(cur, 0, padding_size);

  
  /* argv[i] pushing. */
  for(i = argc; i >=0; i--)
  {
    cur = cur -4;
    *(uint32_t*)cur =(uint32_t)args->argv[i];
  }
  
  /* argv, argc, fake_ret_address pushing */
  uint32_t argv_addr =(uint32_t)cur;
  cur = cur-4;
  *(uint32_t*)cur = argv_addr;
  cur = cur-4;
  *(uint32_t*)cur = argc;
  cur = cur-4;
  *(uint32_t*)cur = 0;
  /* Set esp */
  *esp = (void*)cur;
}

/*************************************************************************************************************************/
/*******/
