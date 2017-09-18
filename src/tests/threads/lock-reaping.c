#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/synch.h"
#include "threads/thread.h"

static void acquire1_thread_func (void* aux);
static void acquire2_thread_func (void* aux);
static void acquire3_thread_func (void* aux);
static void acquire4_thread_func (void* aux);
static struct lock lock1;
static struct lock lock2;
static struct lock lock3;


static struct lock a;
static struct lock b;
static void aa_thread_func (void* aux);
static void bb_thread_func (void* aux);
void test_lock_reaping2 (void)
{
  /* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);

  /* Make sure our priority is the default. */
  ASSERT (thread_get_priority () == PRI_DEFAULT);
  lock_init(&a);
  lock_init(&b);
  lock_acquire(&a);
  msg("main: got the lock a.");
  lock_acquire(&b);
  msg("main: got the lock b.");
  msg("main: create aa and bb");
  thread_create("aa", 32, aa_thread_func, NULL);
  thread_create("bb", 33, bb_thread_func,NULL);
  msg("main: now aa and bb are blocked.");
  msg("main: I will call thread_exit().");
  thread_exit();
  lock_release(&b);
  lock_release(&a);
  msg("main: done.");
}

static void aa_thread_func (void* aux UNUSED)
{
  msg("aa: hello from aa.");
  lock_acquire(&a);
  msg("aa:critical section.");
  lock_release(&a);
  msg("aa: done.");

}
static void bb_thread_func (void* aux UNUSED)
{

  msg("bb: hello from aa.");
  lock_acquire(&b);
  msg("bb:critical section.");
  lock_release(&b);
  msg("bb: done.");


}

void
test_lock_reaping (void) 
{
  /* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);

  /* Make sure our priority is the default. */
  ASSERT (thread_get_priority () == PRI_DEFAULT);

  lock_init(&lock1);
  lock_init(&lock2);
  lock_init(&lock3);
  thread_create("acquire1", 31, acquire1_thread_func, NULL);
  thread_create("acquire2", 31, acquire2_thread_func, NULL);
  thread_create("acquire3", 31, acquire3_thread_func, NULL);
  thread_set_priority(20);
  msg("main: done.");
  msg ("This should be the last line before finishing this test.");
}

static void
acquire1_thread_func (void* aux UNUSED) 
{
  printf("@hello from acquire1.\n"); 
  lock_acquire(&lock1);
  msg("acquire1: got the lock1.");
  lock_acquire(&lock2);
  msg("acquire1: critical section.");
  int i;
  for( i=0; i<100; i++)
  {
    if(i==31)
    {
      thread_create("acquire4", 50, acquire4_thread_func, NULL);
    }
    msg("acuqire1: i: %d",i);
  }
  msg("acquire1: effective priority: %d", thread_get_priority());
  msg("acquire1: after this statement, thread_exit() will be called.");
  thread_exit();
  /*
  msg("acquire1: after this statement, lock_release(&lock2) will be called.");
  lock_release(&lock2);
  msg("acquire1: after this statement, lock_release(&lock1) will be called.");
  lock_release(&lock1);
  */
  msg ("acquire1: done.");
}



static void
acquire2_thread_func (void* aux UNUSED)
{
  printf("#hello from acquire2.\n");

  lock_acquire(&lock1);

  msg("acquire2: got the lock1.");
  msg("acquire2: critical section.");
  msg("acqurie2: after this statement, lock_release(&lock1) will be called.");
  lock_release(&lock1);

  msg("acquire2: done.");
}


static void acquire3_thread_func (void* aux UNUSED)
{
  printf("$hello from acquire3.\n");
  lock_acquire(&lock3);
  msg("acquire3: got the lock3.");
  lock_acquire(&lock2);
  msg("acquire3: got the lock2.");
  msg("acquire3: critical section.");
  msg("acquire3: after this statement, lock_release(&lock2) will be called.");
  lock_release(&lock2);
  lock_release(&lock3);
  msg("acquire3: done.");

}

static void acquire4_thread_func (void* aux UNUSED)
{
  printf("$hello from acquire4.\n");
  lock_acquire(&lock3);
  msg("acquire4: got the lock3.");
  msg("acquire4: critical section.");
  msg("acquire4: after this statement, lock_release(&lock3) will be called.");
  lock_release(&lock3);
  msg("acquire4: done.");
}


