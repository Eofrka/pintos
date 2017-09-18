#include "threads/synch.h"
#include "threads/thread.h"

static void acquire_thread_func(void* aux);
static void tmp_thread_func(void* aux);


static struct lock lock;

void
test_priority_change_case1 (void) 
{

  /* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);

  /* Make sure our priority is the default. */
  ASSERT (thread_get_priority () == PRI_DEFAULT);

  lock_init (&lock);
  lock_acquire (&lock);
  msg("main: got the lock.");
  thread_create ("tmp", PRI_DEFAULT, tmp_thread_func, NULL);
  msg ("main: thread should have priority %d.  Actual priority: %d.",
       PRI_DEFAULT, thread_get_priority ());


  //Q? how to synch?
  int i;
  for(i = 0; i<7; i++)
  {
    if(i == 6)
    {
      thread_set_priority (40);
    }
    msg("main: %d", i);
  }
  

  //msg ("main: after this statement, thread_set_priority(40) will be called.");
  
  msg ("main: thread should have priority %d.  Actual priority: %d.", 40, thread_get_priority ());
  msg("main: after this statment, lock_release(&lock) will be called.");
  lock_release(&lock);
  msg ("Main thread should have priority %d.  Actual priority: %d.", 40, thread_get_priority ());
}


static void tmp_thread_func(void* aux UNUSED)
{
  msg("tmp: hello from tmp.");
  msg("tmp: after this statment, acuqire will be created.");
  thread_create("acquire", 50, acquire_thread_func, NULL);
  msg("tmp: done.");
}

static void acquire_thread_func (void* aux UNUSED) 
{
  msg("acquire: hello from acquire.");
  lock_acquire (&lock);
  msg ("acquire: got the lock");
  msg("acquire: critical section");
  msg("acquire: after this statement, lock_release(&lock) will be called.");
  lock_release (&lock);
  msg ("acquire: done");
}
