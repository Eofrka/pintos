#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/synch.h"
#include "threads/thread.h"

static void acquire1_thread_func (void* aux);
static void acquire2_thread_func (void* aux);
static void acquire3_thread_func (void* aux);

static struct lock lock1;
static struct lock lock2;


void
test_lock_reaping (void) 
{
  /* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);

  /* Make sure our priority is the default. */
  ASSERT (thread_get_priority () == PRI_DEFAULT);

  lock_init(&lock1);
  lock_init(&lock2);
  //msg("main: after this statment priority 31 thread(acquire1) will be created.");
  thread_create("acquire1", PRI_DEFAULT, acquire1_thread_func, NULL);
  //msg("main: after this statement priority 31 thread(acquire2) will be created.");
  thread_create("acquire2", PRI_DEFAULT, acquire2_thread_func, NULL);
  //msg("main: after this statement priority 31 thread(acquire3) will be created.");
  thread_create("acquire3", PRI_DEFAULT, acquire3_thread_func, NULL);
  thread_set_priority(20);
  msg("main: done.");
  msg ("This should be the last line before finishing this test.");
}

static void
acquire1_thread_func (void* aux UNUSED) 
{
  msg("acquire1: hello from acquire1."); 
  lock_acquire(&lock1);
  msg("acquire1: got the lock1.");
  lock_acquire(&lock2);
  msg("acquire1: critical section.");
  int i;
  for( i=0; i<100; i++)
  {
    msg("acuqire1: i: %d",i);
  }
  msg("acquire1: after this statement, thread_exit() will be called.");
  thread_exit();
  msg("acquire1: after this statement, lock_release(&lock1) will be called.");
  lock_release(&lock2);
  msg("acquire1: after this statement, lock_release(&lock1) will be called.");
  lock_release(&lock1);
  msg ("acquire1: done.");
}



static void
acquire2_thread_func (void* aux UNUSED)
{
  msg("acquire2: hello from acquire2.");
  lock_acquire(&lock1);

  msg("acquire2: got the lock1.");
  msg("acquire2: critical section.");
  msg("acqurie2: after this statement, lock_release(&lock1) will be called.");
  lock_release(&lock1);
  msg("acquire2: done.");
}


static void acquire3_thread_func (void* aux UNUSED)
{
  msg("acquire3: hello from acquire3.");
  lock_acquire(&lock2);
  msg("acquire3: got the lock2.");
  msg("acquire3: critical section.");
  msg("acquire3: after this statement, lock_release(&lock2) will be called.");
  lock_release(&lock2);
  msg("acquire3: done.");

}
