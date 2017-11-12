#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include <debug.h>
#include <list.h>
#include <stdint.h>

/* pj2 */
/*******/
#ifdef USERPROG
#include "threads/synch.h"
#include "userprog/process.h"
#endif
/*******/

/* pj3 */
/*******/
#ifdef VM
#include <hash.h>
#endif
/*******/

/* States in a thread's life cycle. */
enum thread_status
  {
    THREAD_RUNNING,     /* Running thread. */
    THREAD_READY,       /* Not running but ready to run. */
    THREAD_BLOCKED,     /* Waiting for an event to trigger. */
    THREAD_DYING        /* About to be destroyed. */
  };

/* Thread identifier type.
   You can redefine this to whatever type you like. */
typedef int tid_t;
#define TID_ERROR ((tid_t) -1)          /* Error value for tid_t. */

/* Thread priorities. */
#define PRI_MIN 0                       /* Lowest priority. */
#define PRI_DEFAULT 31                  /* Default priority. */
#define PRI_MAX 63                      /* Highest priority. */

/* A kernel thread or user process.

   Each thread structure is stored in its own 4 kB page.  The
   thread structure itself sits at the very bottom of the page
   (at offset 0).  The rest of the page is reserved for the
   thread's kernel stack, which grows downward from the top of
   the page (at offset 4 kB).  Here's an illustration:

        4 kB +---------------------------------+
             |          kernel stack           |
             |                |                |
             |                |                |
             |                V                |
             |         grows downward          |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             |                                 |
             +---------------------------------+
             |              magic              |
             |                :                |
             |                :                |
             |               name              |
             |              status             |
        0 kB +---------------------------------+

   The upshot of this is twofold:

      1. First, `struct thread' must not be allowed to grow too
         big.  If it does, then there will not be enough room for
         the kernel stack.  Our base `struct thread' is only a
         few bytes in size.  It probably should stay well under 1
         kB.

      2. Second, kernel stacks must not be allowed to grow too
         large.  If a stack overflows, it will corrupt the thread
         state.  Thus, kernel functions should not allocate large
         structures or arrays as non-static local variables.  Use
         dynamic allocation with malloc() or palloc_get_page()
         instead.

   The first symptom of either of these problems will probably be
   an assertion failure in thread_current(), which checks that
   the `magic' member of the running thread's `struct thread' is
   set to THREAD_MAGIC.  Stack overflow will normally change this
   value, triggering the assertion. */
/* The `elem' member has a dual purpose.  It can be an element in
   the run queue (thread.c), or it can be an element in a
   semaphore wait list (synch.c).  It can be used these two ways
   only because they are mutually exclusive: only a thread in the
   ready state is on the run queue, whereas only a thread in the
   blocked state is on a semaphore wait list. */
struct thread
  {
    /* Owned by thread.c. */
    tid_t tid;                          /* Thread identifier. */
    enum thread_status status;          /* Thread state. */
    char name[16];                      /* Name (for debugging purposes). */
    uint8_t *stack;                     /* Saved stack pointer. */
    int priority;                       /* Priority. */

    /* Shared between thread.c and synch.c. */
    struct list_elem elem;              /* List element. */
    
    /* pj1 */
    /*******/
    int old_priority;                   /* Old priority */
    struct lock* lock;                  /* When thread calls lock_acquire(), if the target lock's holder is not NULL, this lock is set to the lock */
    struct list lock_list;              /* When thread calls for lock_acquire(), if the target lock's holder is NULL, push the lock into the lock_list */
    struct semaphore* sema;             /* Point the semaphore in which this thread is waiting */ 
    int donation_level;                 /* Donation level for priority donation */
    struct condition* cond;             /* Condition variable in which this thread is waiting encapsulated with semaphore_elem. */
    /*******/
#ifdef USERPROG
    /* Owned by userprog/process.c. */
    uint32_t *pagedir;                  /* Page directory. */
    /* pj2 */
    /*******/
    bool load;                          /* If load succeeded in child process, true. Else false. */
    struct semaphore sema_exec;         /* Semaphore for system call exec. (process_execute, start_process) */
    struct process* process;            /* Information of this process. */
    struct list child_list;             /* Child process list. (list of struct process not struct thread) */ 
    int next_fd;                        /* Next fd to alloacte. */
    struct list fdt;                    /* File descriptor table. */
    struct file* executable;            /* Executable file for this process. */     
    /*******/
#endif


/* pj3 */
/*******/
#ifdef VM
    struct hash spt;                    /* Supplemental page table. */
    uint8_t* esp;                       /* User program's stack pointer. */
#endif    
/*******/

    /* Owned by thread.c. */
    unsigned magic;                     /* Detects stack overflow. */
  };

/* If false (default), use round-robin scheduler.
   If true, use multi-level feedback queue scheduler.
   Controlled by kernel command-line option "-o mlfqs". */
extern bool thread_mlfqs;

void thread_init (void);
void thread_start (void);

void thread_tick (void);
void thread_print_stats (void);

typedef void thread_func (void *aux);
tid_t thread_create (const char *name, int priority, thread_func *, void *);

void thread_block (void);
void thread_unblock (struct thread *);

struct thread *thread_current (void);
tid_t thread_tid (void);
const char *thread_name (void);

void thread_exit (void) NO_RETURN;
void thread_yield (void);

int thread_get_priority (void);
void thread_set_priority (int);

int thread_get_nice (void);
void thread_set_nice (int);
int thread_get_recent_cpu (void);
int thread_get_load_avg (void);

/* pj1 */
/*******/

/* New function declarations */
/**********************************************************************************************************************************/
void thread_sleep(int64_t wakeup_ticks);
void thread_awake(int64_t current_ticks);
void thread_check_and_yield(void);
void thread_check_and_yield_on_return(void);
int thread_get_max_priority(struct list* list);
void thread_donate_priority(struct thread* src);
void thread_restore_priority(void);
void thread_remove_and_insert_ordered(struct list* list, struct thread* t);
void thread_dec_donation_level(struct thread* root, int dec);
bool thread_priority_g(const struct list_elem* a, const struct list_elem* b, void* aux);
/**********************************************************************************************************************************/
/*******/

#endif /* threads/thread.h */