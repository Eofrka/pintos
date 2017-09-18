/* This file is derived from source code for the Nachos
   instructional operating system.  The Nachos copyright notice
   is reproduced in full below. */

/* Copyright (c) 1992-1996 The Regents of the University of California.
   All rights reserved.

   Permission to use, copy, modify, and distribute this software
   and its documentation for any purpose, without fee, and
   without written agreement is hereby granted, provided that the
   above copyright notice and the following two paragraphs appear
   in all copies of this software.

   IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
   ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
   CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
   AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
   HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
   PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
   BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
   MODIFICATIONS.
*/

#include "threads/synch.h"
#include <stdio.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

/* Initializes semaphore SEMA to VALUE.  A semaphore is a
   nonnegative integer along with two atomic operators for
   manipulating it:

   - down or "P": wait for the value to become positive, then
     decrement it.

   - up or "V": increment the value (and wake up one waiting
     thread, if any). */
void
sema_init (struct semaphore *sema, unsigned value) 
{
  ASSERT (sema != NULL);

  sema->value = value;
  list_init (&sema->waiters);
}

/* Down or "P" operation on a semaphore.  Waits for SEMA's value
   to become positive and then atomically decrements it.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but if it sleeps then the next scheduled
   thread will probably turn interrupts back on. This is
   sema_down function. */
void
sema_down (struct semaphore *sema) 
{
  enum intr_level old_level;
  struct thread* curr;
  ASSERT (sema != NULL);
  ASSERT (!intr_context ());

  old_level = intr_disable ();
  curr = thread_current();
  while (sema->value == 0) 
  {
    /* pj1 */
    /*******/
    /* Insert current thread into semaphore's waiters in descending order of priority. */
    curr->sema = sema;
    list_insert_ordered(&sema->waiters,&curr->elem,(list_less_func*)thread_priority_g, NULL);
    /* Before thread_block(), Actual priority donation needed! */
    thread_donate_priority(curr);
    /*******/
    thread_block ();
  }
  /* When thread unblock from sema_up(), start from here. */
  sema->value--;
  intr_set_level (old_level);
}

/* Down or "P" operation on a semaphore, but only if the
   semaphore is not already 0.  Returns true if the semaphore is
   decremented, false otherwise.

   This function may be called from an interrupt handler. */
bool
sema_try_down (struct semaphore *sema) 
{
  enum intr_level old_level;
  bool success;

  ASSERT (sema != NULL);

  old_level = intr_disable ();
  if (sema->value > 0) 
    {
      sema->value--;
      success = true; 
    }
  else
    success = false;
  intr_set_level (old_level);

  return success;
}

/* Up or "V" operation on a semaphore.  Increments SEMA's value
   and wakes up one thread of those waiting for SEMA, if any.

   This function may be called from an interrupt handler. */
void
sema_up (struct semaphore *sema) 
{
  enum intr_level old_level;
  /* pj1 */
  /*******/
  struct thread* to_unblock_thread;
  /*******/
  ASSERT (sema != NULL);

  old_level = intr_disable ();
  if (!list_empty (&sema->waiters))
  { 
    /* pj1 */
    /*******/
    /* pop the highest priority thread from the waiters, and the thread is going to be unblocked. then get the priority of the thread */
    to_unblock_thread = list_entry (list_pop_front (&sema->waiters),struct thread, elem); 
    to_unblock_thread->lock = NULL;
    to_unblock_thread->sema = NULL;
    to_unblock_thread->cond = NULL;
    /* Update donation level */
    int dec = to_unblock_thread->donation_level;
    //for each lock in the to_unblock_thread->lock_list :
      // for each thread in the lock->samaphore->waiters :
        // thread->donation_level -= dec; recursively
    if(dec !=0)
    { 
      thread_dec_donation_level(to_unblock_thread, dec);
    }
    /*******/
    thread_unblock (to_unblock_thread);

  }
  sema->value++;
  /* pj1 */
  /*******/
  thread_preempt();
  /*******/
  intr_set_level (old_level);

}

static void sema_test_helper (void *sema_);

/* Self-test for semaphores that makes control "ping-pong"
   between a pair of threads.  Insert calls to printf() to see
   what's going on. */
void
sema_self_test (void) 
{
  struct semaphore sema[2];
  int i;

  printf ("Testing semaphores...");
  sema_init (&sema[0], 0);
  sema_init (&sema[1], 0);
  thread_create ("sema-test", PRI_DEFAULT, sema_test_helper, &sema);
  for (i = 0; i < 10; i++) 
    {
      sema_up (&sema[0]);
      sema_down (&sema[1]);
    }
  printf ("done.\n");
}

/* Thread function used by sema_self_test(). */
static void
sema_test_helper (void *sema_) 
{
  struct semaphore *sema = sema_;
  int i;

  for (i = 0; i < 10; i++) 
    {
      sema_down (&sema[0]);
      sema_up (&sema[1]);
    }
}

/* Initializes LOCK.  A lock can be held by at most a single
   thread at any given time.  Our locks are not "recursive", that
   is, it is an error for the thread currently holding a lock to
   try to acquire that lock.

   A lock is a specialization of a semaphore with an initial
   value of 1.  The difference between a lock and such a
   semaphore is twofold.  First, a semaphore can have a value
   greater than 1, but a lock can only be owned by a single
   thread at a time.  Second, a semaphore does not have an owner,
   meaning that one thread can "down" the semaphore and then
   another one "up" it, but with a lock the same thread must both
   acquire and release it.  When these restrictions prove
   onerous, it's a good sign that a semaphore should be used,
   instead of a lock. */
void
lock_init (struct lock *lock)
{
  ASSERT (lock != NULL);

  lock->holder = NULL;
  sema_init (&lock->semaphore, 1);
  /* pj1 */
  /*******/
  lock->elem.prev =NULL;
  lock->elem.next = NULL;
  /*******/
}

/* Acquires LOCK, sleeping until it becomes available if
   necessary.  The lock must not already be held by the current
   thread.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but interrupts will be turned back on if
   we need to sleep. */
void
lock_acquire (struct lock *lock)
{
  ASSERT (lock != NULL);
  ASSERT (!intr_context ());
  ASSERT (!lock_held_by_current_thread (lock));

  /* pj1 */
  /*******/
  /* Disable the interrupt */
  enum intr_level old_level;
  old_level = intr_disable();
  struct thread* curr = thread_current();
  if(lock->holder != NULL)
  {
    /* These two statements are needed for recursive call of thread_donate_priority() in sema_down() */
    /* Current thread's lock points the lock. */
    curr->lock = lock;
    /* Update the donation level. */
    curr->donation_level = lock->holder->donation_level+1;
  }
  /*******/  

  sema_down (&lock->semaphore);

  /* pj1 */
  /*******/
  /* After sema down, it means current thread is unblocked from sema_up(after return of sema_down()). */

  /* Insert the lock into current thread's lock_list. Order is not important because the lock has no waiting threads in the semaphore's waiters initially. */
  list_push_back(&curr->lock_list, &lock->elem);
  lock->holder = curr;
  /* Restore the interrupt */
  intr_set_level(old_level);
  /*******/

}

/* Tries to acquires LOCK and returns true if successful or false
   on failure.  The lock must not already be held by the current
   thread.

   This function will not sleep, so it may be called within an
   interrupt handler. */
bool
lock_try_acquire (struct lock *lock)
{
  bool success;

  ASSERT (lock != NULL);
  ASSERT (!lock_held_by_current_thread (lock));

  success = sema_try_down (&lock->semaphore);
  if (success)
    lock->holder = thread_current ();
  return success;
}

/* Releases LOCK, which must be owned by the current thread.
   This is lock_release function.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to release a lock within an interrupt
   handler. */
void
lock_release (struct lock *lock) 
{
  ASSERT (lock != NULL);
  ASSERT (lock_held_by_current_thread (lock));
  /* pj1 */
  /*******/
  /* Q: old_level = intr_disabel(), intr_set_level(old_level) needed? */
  /* Disable the interrupt */
  enum intr_level old_level;
  old_level = intr_disable();

  // 1. Remove lock from current thread's lock_list. 
  list_remove(&lock->elem);
  
  // 2. Priority restoration needed! 
  thread_restore_priority();
  
  lock->holder = NULL;
  sema_up (&lock->semaphore);
  /* Restore the interrupt */
  intr_set_level(old_level);
  /*******/
}

/* Returns true if the current thread holds LOCK, false
   otherwise.  (Note that testing whether some other thread holds
   a lock would be racy.) */
bool
lock_held_by_current_thread (const struct lock *lock) 
{
  ASSERT (lock != NULL);

  return lock->holder == thread_current ();
}

/* One semaphore in a list. */
struct semaphore_elem 
  {
    struct list_elem elem;              /* List element. */
    struct semaphore semaphore;         /* This semaphore. */
  };


/* Initializes condition variable COND.  A condition variable
   allows one piece of code to signal a condition and cooperating
   code to receive the signal and act upon it. */
void
cond_init (struct condition *cond)
{
  ASSERT (cond != NULL);

  list_init (&cond->waiters);
}

/* Atomically releases LOCK and waits for COND to be signaled by
   some other piece of code.  After COND is signaled, LOCK is
   reacquired before returning.  LOCK must be held before calling
   this function.

   The monitor implemented by this function is "Mesa" style, not
   "Hoare" style, that is, sending and receiving a signal are not
   an atomic operation.  Thus, typically the caller must recheck
   the condition after the wait completes and, if necessary, wait
   again.

   A given condition variable is associated with only a single
   lock, but one lock may be associated with any number of
   condition variables.  That is, there is a one-to-many mapping
   from locks to condition variables.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but interrupts will be turned back on if
   we need to sleep. */
void
cond_wait (struct condition *cond, struct lock *lock) 
{
  struct semaphore_elem waiter;

  ASSERT (cond != NULL);
  ASSERT (lock != NULL);
  ASSERT (!intr_context ());
  ASSERT (lock_held_by_current_thread (lock));
  
  sema_init (&waiter.semaphore, 0);
  /* pj1 */
  /*******/
  /* Insert the semaphore_elem "waiter" into cond->waiters(list) in descending order of thread's priority. */
  struct thread* curr = thread_current();
  struct list_elem* semaphore_elem_iter;
  struct semaphore_elem* tmp_capsule;
  for(semaphore_elem_iter = list_begin(&cond->waiters); semaphore_elem_iter != list_end(&cond->waiters); semaphore_elem_iter=list_next(semaphore_elem_iter))
  {
    tmp_capsule = list_entry(semaphore_elem_iter, struct semaphore_elem, elem);
    if(thread_get_max_priority(&tmp_capsule->semaphore.waiters) < curr->priority)
    {
      break;
    }
  }
  list_insert(semaphore_elem_iter, &waiter.elem);
  curr->cond = cond;
  /*******/
  lock_release (lock);
  sema_down (&waiter.semaphore);
  lock_acquire (lock);
}


/* If any threads are waiting on COND (protected by LOCK), then
   this function signals one of them to wake up from its wait.
   LOCK must be held before calling this function.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to signal a condition variable within an
   interrupt handler. */
void
cond_signal (struct condition *cond, struct lock *lock UNUSED) 
{
  ASSERT (cond != NULL);
  ASSERT (lock != NULL);
  ASSERT (!intr_context ());
  ASSERT (lock_held_by_current_thread (lock));

  if (!list_empty (&cond->waiters)) 
    sema_up (&list_entry (list_pop_front (&cond->waiters),
                          struct semaphore_elem, elem)->semaphore);
}

/* Wakes up all threads, if any, waiting on COND (protected by
   LOCK).  LOCK must be held before calling this function.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to signal a condition variable within an
   interrupt handler. */
void
cond_broadcast (struct condition *cond, struct lock *lock) 
{
  ASSERT (cond != NULL);
  ASSERT (lock != NULL);

  while (!list_empty (&cond->waiters))
    cond_signal (cond, lock);
}


/* pj1 */
/*******/
/* New function definitions. */
/**********************************************************************************************************************************/
/* Removes thread t's semaphore_elem(capsule) from the condition variable's waiters.
   Inserts it(capsule) again in the list(waiters) in descending order of priority. */
void semphoare_elem_remove_and_insert_ordered(struct list* waiters, struct thread* t)
{
  struct condition* cond = t->cond;
  ASSERT(cond != NULL);
  struct semaphore_elem* capsule;
  struct semaphore_elem tmp_semaphore_elem;
  uint8_t* offset = (uint8_t*)8;
  /* Get the capsule(semaphore_elem). */
  capsule= (struct semaphore_elem*)((uint8_t*)t->sema - offset);

  /* First, remove it from the waiters. */
  list_remove(&capsule->elem);

  struct list_elem* semaphore_elem_iter;
  struct semaphore_elem* tmp_capsule;

  /* Find appropriate place to insert again. */
  for(semaphore_elem_iter = list_begin(waiters); semaphore_elem_iter != list_end(waiters); semaphore_elem_iter=list_next(semaphore_elem_iter))
  {
    tmp_capsule = list_entry(semaphore_elem_iter, struct semaphore_elem, elem);
    if(thread_get_max_priority(&tmp_capsule->semaphore.waiters) < t->priority)
    {
      break;
    }
  }
  list_insert(semaphore_elem_iter, &capsule->elem);


}

/* If lock A's waiting max priority >= lock B's waiting max priority, returns true. Else returns false. */  
void lock_max_priority_ge(struct list_elem* a, struct list_elem* b, void* aux)
{
  struct lock* lock_a = list_entry(a, struct lock, elem);
  struct lock* lock_b = list_entry(b, struct lock, elem);
  return thread_get_max_priority(&lock_a->semaphore.waiters) >= thread_get_max_priority(&lock_b->semaphore.waiters); 
}
/**********************************************************************************************************************************/
/*******/
