#include "userprog/exception.h"
#include <inttypes.h>
#include <stdio.h>
#include "userprog/gdt.h"
#include "threads/interrupt.h"
#include "threads/thread.h"

/* pj2 */
/*******/
#include "userprog/syscall.h"
/*******/

/* pj3 */
/*******/
#ifdef VM
#include "vm/page.h"
#include "threads/vaddr.h"
#include "vm/frame.h"
#include "threads/malloc.h"
#include "vm/swap.h"

#define MAX_STACK_SIZE 0x800000

#endif
/*******/ 

/* Number of page faults processed. */
static long long page_fault_cnt;

static void kill (struct intr_frame *);
static void page_fault (struct intr_frame *);

/* Registers handlers for interrupts that can be caused by user
   programs.

   In a real Unix-like OS, most of these interrupts would be
   passed along to the user process in the form of signals, as
   described in [SV-386] 3-24 and 3-25, but we don't implement
   signals.  Instead, we'll make them simply kill the user
   process.

   Page faults are an exception.  Here they are treated the same
   way as other exceptions, but this will need to change to
   implement virtual memory.

   Refer to [IA32-v3a] section 5.15 "Exception and Interrupt
   Reference" for a description of each of these exceptions. */
void
exception_init (void) 
{
  /* These exceptions can be raised explicitly by a user program,
     e.g. via the INT, INT3, INTO, and BOUND instructions.  Thus,
     we set DPL==3, meaning that user programs are allowed to
     invoke them via these instructions. */
  intr_register_int (3, 3, INTR_ON, kill, "#BP Breakpoint Exception");
  intr_register_int (4, 3, INTR_ON, kill, "#OF Overflow Exception");
  intr_register_int (5, 3, INTR_ON, kill,
   "#BR BOUND Range Exceeded Exception");

  /* These exceptions have DPL==0, preventing user processes from
     invoking them via the INT instruction.  They can still be
     caused indirectly, e.g. #DE can be caused by dividing by
     0.  */
  intr_register_int (0, 0, INTR_ON, kill, "#DE Divide Error");
  intr_register_int (1, 0, INTR_ON, kill, "#DB Debug Exception");
  intr_register_int (6, 0, INTR_ON, kill, "#UD Invalid Opcode Exception");
  intr_register_int (7, 0, INTR_ON, kill,
   "#NM Device Not Available Exception");
  intr_register_int (11, 0, INTR_ON, kill, "#NP Segment Not Present");
  intr_register_int (12, 0, INTR_ON, kill, "#SS Stack Fault Exception");
  intr_register_int (13, 0, INTR_ON, kill, "#GP General Protection Exception");
  intr_register_int (16, 0, INTR_ON, kill, "#MF x87 FPU Floating-Point Error");
  intr_register_int (19, 0, INTR_ON, kill,
   "#XF SIMD Floating-Point Exception");

  /* Most exceptions can be handled with interrupts turned on.
     We need to disable interrupts for page faults because the
     fault address is stored in CR2 and needs to be preserved. */
  intr_register_int (14, 0, INTR_OFF, page_fault, "#PF Page-Fault Exception");
}

/* Prints exception statistics. */
void
exception_print_stats (void) 
{
  printf ("Exception: %lld page faults\n", page_fault_cnt);
}

/* Handler for an exception (probably) caused by a user process. */
static void
kill (struct intr_frame *f) 
{
  /* This interrupt is one (probably) caused by a user process.
     For example, the process might have tried to access unmapped
     virtual memory (a page fault).  For now, we simply kill the
     user process.  Later, we'll want to handle page faults in
     the kernel.  Real Unix-like operating systems pass most
     exceptions back to the process via signals, but we don't
     implement them. */

  /* The interrupt frame's code segment value tells us where the
     exception originated. */
  switch (f->cs)
  {
    case SEL_UCSEG:
      /* User's code segment, so it's a user exception, as we
         expected.  Kill the user process.  */
    printf ("%s: dying due to interrupt %#04x (%s).\n",
      thread_name (), f->vec_no, intr_name (f->vec_no));
    intr_dump_frame (f);
      /* pj2 */
      /*******/
      /* kill might exit process with exit_status -1. */
    syscall_exit(-1); 
      /*******/

    case SEL_KCSEG:
      /* Kernel's code segment, which indicates a kernel bug.
         Kernel code shouldn't throw exceptions.  (Page faults
         may cause kernel exceptions--but they shouldn't arrive
         here.)  Panic the kernel to make the point.  */
    intr_dump_frame (f);
    PANIC ("Kernel bug - unexpected interrupt in kernel"); 

    default:
      /* Some other code segment?  Shouldn't happen.  Panic the
         kernel. */
    printf ("Interrupt %#04x (%s) in unknown segment %04x\n",
     f->vec_no, intr_name (f->vec_no), f->cs);
      /* pj2 */
      /*******/
      /* kill might exit process with exit_status -1. */
    syscall_exit(-1); 
      /*******/
  }
}

/* Page fault handler.  This is a skeleton that must be filled in
   to implement virtual memory.  Some solutions to project 2 may
   also require modifying this code.

   At entry, the address that faulted is in CR2 (Control Register
   2) and information about the fault, formatted as described in
   the PF_* macros in exception.h, is in F's error_code member.  The
   example code here shows how to parse that information.  You
   can find more information about both of these in the
   description of "Interrupt 14--Page Fault Exception (#PF)" in
   [IA32-v3a] section 5.15 "Exception and Interrupt Reference". */
static void
page_fault (struct intr_frame *f) 
{
  bool not_present;  /* True: not-present page, false: writing r/o page. */
  bool write;        /* True: access was write, false: access was read. */
  bool user;         /* True: access by user, false: access by kernel. */
  void *fault_addr;  /* Fault address. */

  /* Obtain faulting address, the virtual address that was
     accessed to cause the fault.  It may point to code or to
     data.  It is not necessarily the address of the instruction
     that caused the fault (that's f->eip).
     See [IA32-v2a] "MOV--Move to/from Control Registers" and
     [IA32-v3a] 5.15 "Interrupt 14--Page Fault Exception
     (#PF)". */
  asm ("movl %%cr2, %0" : "=r" (fault_addr));

  /* Turn interrupts back on (they were only off so that we could
     be assured of reading CR2 before it changed). */
  intr_enable ();

  /* Count page faults. */
  page_fault_cnt++;

  /* Determine cause. */
  not_present = (f->error_code & PF_P) == 0;
  write = (f->error_code & PF_W) != 0;
  user = (f->error_code & PF_U) != 0;

/* pj3 */
/*******/
#ifdef VM
  //
  struct thread* curr = thread_current();
  //printf("[fault_addr]:[0x%08x]\n", fault_addr);
  //spt_print(&curr->spt);

  /* Get the esp. */
  uint8_t* esp = user? (uint8_t*)f->esp : curr->esp; 


  /* Get the valid spte. */
  /*********************************************************************************************************************************/
  //1. Check the fault_addr is valid or not. It it is invalid,
  // terminates the process and thereby frees all of its resources(this part is in process_exit()).
  if(!not_present)
  {
    if(!user)
    {
      f->eip = (void*)f->eax;
      f->eax = 0xffffffff;
      return;
    }
    /* To implement virtual memory, delete the rest of the function
     body, and replace it with code that brings in the page to
     which fault_addr refers. */
    printf ("Page fault at %p: %s error %s page in %s context.\n",
      fault_addr,
      not_present ? "not present" : "rights violation",
      write ? "writing" : "reading",
      user ? "user" : "kernel");
    kill (f);

  }

  if(((uint32_t)fault_addr >= (uint32_t)PHYS_BASE) || ((uint32_t)fault_addr < (uint32_t)0x08048000))
  {
    if(!user)
    {
      f->eip = (void*)f->eax;
      f->eax = 0xffffffff;
      return;
    }
    /* To implement virtual memory, delete the rest of the function
     body, and replace it with code that brings in the page to
     which fault_addr refers. */
    printf ("Page fault at %p: %s error %s page in %s context.\n",
      fault_addr,
      not_present ? "not present" : "rights violation",
      write ? "writing" : "reading",
      user ? "user" : "kernel");
    kill (f);
  }

  //2. If the memory reference is valid.
  //2-1-1. If the fault_addr is growable stack region, grow the stack.
  uint32_t stack_limit = (uint32_t)PHYS_BASE - (uint32_t)MAX_STACK_SIZE;
  uint32_t additional_stack_base = (uint32_t)PHYS_BASE - (uint32_t)PGSIZE;


  bool cond1 = (stack_limit <= (uint32_t)esp) && ((uint32_t)esp < (uint32_t)PHYS_BASE);
  bool cond2 = (stack_limit <= (uint32_t)fault_addr) && ((uint32_t)fault_addr < additional_stack_base);
  bool cond3 = ((uint32_t)fault_addr >= (uint32_t)esp) || ((uint32_t)fault_addr == ((uint32_t)esp -(uint32_t)4)) || ((uint32_t)fault_addr == ((uint32_t)esp - (uint32_t)32));
  bool stack_growth_need = cond1 && cond2 && cond3;

  void* fault_page_vaddr = pg_round_down(fault_addr);


  if(stack_growth_need == true)
  {
    //1. Create a spte.
    struct supplemental_page_table_entry* spte = spte_create();
    if(spte == NULL)
    {
      PANIC("not enough memory to allocate spte");
    }
    //2. Initialize the spte.
    spte->uvaddr = fault_page_vaddr;
    spte->state = SPTE_ZERO;
    spte->fte = NULL;
    spte->file = NULL;
    spte->ofs = 0;
    spte->page_read_bytes =0;
    spte->page_zero_bytes =PGSIZE;
    spte->writable = true;
    spte->he.list_elem.prev = NULL;
    spte->he.list_elem.next = NULL;
    spte->is_stack_seg = true;
    spte->swap_idx = SWAP_IDX_DEFAULT;

    //3. Insert the spte into spt.
    spte_insert(&thread_current()->spt, &spte->he);
  }

  //2-1-2. Else skip

  //2-2. Search the spt and get the spte. If there are no spte[fault_addr] terminates the process.(Also invalid case)
  struct supplemental_page_table_entry* spte = spte_find(&curr->spt, fault_page_vaddr);
  if(spte == NULL)
  {
    if(!user)
    {
      f->eip = (void*)f->eax;
      f->eax = 0xffffffff;
      return;
    }
    /* To implement virtual memory, delete the rest of the function
     body, and replace it with code that brings in the page to
     which fault_addr refers. */
    printf ("Page fault at %p: %s error %s page in %s context.\n",
      fault_addr,
      not_present ? "not present" : "rights violation",
      write ? "writing" : "reading",
      user ? "user" : "kernel");
    kill (f);
  }

  /*********************************************************************************************************************************/
  //3. Obtain a frame to store the page. See Section 4.1.5 [Managing the Frame Table] for details.
  struct frame_table_entry* fte = fte_obtain(PAL_USER);

  //4. Fetch the data into the frame.
  bool fetch_success = fte_fetch(&fte->elem, &spte->he);
  ASSERT(fetch_success == true);

  //5. Insert the frame into frame table.
  fte_insert(&frame_table, &fte->elem);

  //6. Install the frame into actual page table.
  if(!fte_install(spte->uvaddr, fte->kvaddr, spte->writable))
  {
    PANIC("install failed");
  }
  //7. Update the spte, spt.
  spte->state = SPTE_LOADED;
  return;


#endif   
/*******/
  /* pj2 */
  /*******/
  if(!user)
  {
    f->eip = (void*)f->eax;
    f->eax = 0xffffffff;
    return;
  }
  /*******/

  /* To implement virtual memory, delete the rest of the function
     body, and replace it with code that brings in the page to
     which fault_addr refers. */
  printf ("Page fault at %p: %s error %s page in %s context.\n",
    fault_addr,
    not_present ? "not present" : "rights violation",
    write ? "writing" : "reading",
    user ? "user" : "kernel");
  kill (f);
}


