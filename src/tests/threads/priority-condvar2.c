/* Tests that cond_signal() wakes up the highest-priority thread
   waiting in cond_wait(). */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"

static thread_func priority_condvar_thread;
static thread_func priority_condvar_thread2;
static thread_func priority_donate_thread;
static struct lock lock;
static struct condition condition;


static struct lock donation_lock;

void
test_priority_condvar2 (void) 
{
  int i;
  
  /* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);

  lock_init (&lock);
  cond_init (&condition);
  lock_init(&donation_lock);
  thread_set_priority (PRI_MIN);
  for (i = 0; i < 10; i++) 
    {
      int priority = PRI_DEFAULT - (i + 7) % 10 - 1;
      char name[16];
      snprintf (name, sizeof name, "priority %d", priority);
      if(priority == 21)
      {
        thread_create(name, priority, priority_condvar_thread2, NULL);
      }
      else
      {
        thread_create (name, priority, priority_condvar_thread, NULL);
      }
    }

  msg("after this statment, thread(priority:50) will be created.");
  thread_create("donation", 50, priority_donate_thread, NULL);
  msg("now signaling will be started.");


  for (i = 0; i < 10; i++) 
    {
      lock_acquire (&lock);
      msg ("Signaling...");
      cond_signal (&condition, &lock);
      lock_release (&lock);
    }
}

static void
priority_condvar_thread (void *aux UNUSED) 
{
  msg ("Thread %s starting.", thread_name ());
  lock_acquire (&lock);
  cond_wait (&condition, &lock);
  msg ("%s: Thread priority %d woke up.", thread_name(), thread_current()->priority);
  lock_release (&lock);
  msg("%s: done.", thread_name());

}

static void
priority_condvar_thread2 (void *aux UNUSED)
{
  msg ("Thread %s starting.", thread_name ());
  lock_acquire(&donation_lock);
  msg("Thread %s: got the donation_lock.", thread_name());  
  lock_acquire (&lock);
  cond_wait (&condition, &lock);
  msg ("%s: Thread priority %d woke up.",thread_name(), thread_current()->priority);
  lock_release (&lock);
  lock_release(&donation_lock);
  msg("%s: done.", thread_name());
}



static void
priority_donate_thread(void *aux UNUSED)
{
  msg("donation: hello from donation. after this, donation will be blocked.");
  lock_acquire(&donation_lock);
  msg("donation: critical section!");
  lock_release(&donation_lock);
  msg("donation: done");

}