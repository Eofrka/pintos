#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/synch.h"
#include "threads/thread.h"

static void
acquire1_thread_func (void);

static struct lock lock1;
static struct lock lock2;




static void
down1_thread_func(void); 

static struct semaphore sema1;
static struct semaphore sema2;


void
test_deadlock_lock (void) 
{
  /* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);

  /* Make sure our priority is the default. */
  ASSERT (thread_get_priority () == PRI_DEFAULT);

  lock_init (&lock1);
  lock_init(&lock2);
  lock_acquire (&lock1);
  msg("main: got the lock1.");
  thread_create ("acquire1", PRI_DEFAULT+1, acquire1_thread_func, NULL);


  msg("main: acquire1 has been blocked(because of lock_acquire(&lock1))...");
  msg("main: after this statement, lock_acquire(&lock2) will be called.");
  lock_acquire(&lock2);
  /* Deadlock! */

  msg("main: critical section!");
  msg("main: unreachable.");
  lock_release (&lock1);
  msg("main: done.");
  msg ("This should be the last line before finishing this test.");
}

static void
acquire1_thread_func (void) 
{
  
  lock_acquire (&lock2);
  msg ("acquire1: got the lock2.");
  msg("acquire1: after this statement, lock_acquire(&lock1) will be called.");
  lock_acquire(&lock1);
  /* Deadlock. */
  msg("acquire1: critical section!");
  msg("acquire1: unreachable.");
  lock_release (&lock2);
  msg ("acquire1: done.");
}



void test_deadlock_sema(void)
{

/* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);
  /* Make sure our priority is the default. */
  ASSERT (thread_get_priority () == PRI_DEFAULT);

  sema_init(&sema1, 1);
  sema_init(&sema2, 1);



  sema_down(&sema1);
  msg("main: got the sema1.");
  thread_create("down1", PRI_DEFAULT+1, down1_thread_func, NULL);
  msg("main: down1 has been blocked(because of sema_down(&sema1))...");
  msg("main: after this statement, sema_down(&sema2) will be called.");
  sema_down(&sema2);
  /* Deadlock! */
  msg("main: critical section");
  msg("main: unreachable.");
  sema_up(&sema1);
  msg("main: done.");
  msg ("This should be the last line before finishing this test.");


}


static void down1_thread_func(void)
{
  sema_down(&sema2);
  msg("down1: got the sema2.");
  msg("down1: after this statement, sema_down(&sema1) will be called.");
  sema_down(&sema1);
  /* Deadlock! */
  msg("down1: critical section.");
  msg("down1: unreachable.");
  sema_up(&sema2);
  msg("down1: done.");


}