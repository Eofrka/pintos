#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

/* pj2 */
/*******/
typedef int pid_t;
#define PID_ERROR ((pid_t)-1)
#define PID_DEFAULT ((pid_t)0)
#define DEFAULT_EXIT_STATUS ((int)-1)

/* New data structures. */
/*************************************************************************************************************************/
/* Process. */
struct process
{
  pid_t pid;                          /* Process identifier. */
  int exit_status;                    /* Exit status. */
  bool zombie;                        /* If this process is zombie stauts, true. Else false. */
  struct thread* parent;              /* Point parent thread. */
  bool waiting;                       /* If this process is waiting a child process, true, Else false. */
  struct semaphore sema_wait;         /* Semaphore for wait system call. */
  struct list_elem elem;              /* List element for child_list. */
};

/* File descriptor entry. */
struct file_descriptor_entry
{
  int fd;                             /* File descriptor. */
  struct file* file;                  /* File table entry. */
  struct list_elem elem;              /* List element for file descriptor table. */
};



/*************************************************************************************************************************/
/*******/
pid_t process_execute (const char *file_name);
int process_wait (pid_t);
void process_exit (void);
void process_activate (void);

#endif /* userprog/process.h */
