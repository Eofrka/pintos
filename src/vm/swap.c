#include "vm/swap.h"
#include "threads/vaddr.h"
#include "devices/disk.h"
/* Initializes the swap_table. */
void swap_init(void)
{
  swap_table.disk = disk_get(1,1);
  lock_init(&swap_table.lock);
  size_t sectors_in_page = PGSIZE/DISK_SECTOR_SIZE;
   //# of sectors / (PGSIZE/DISK_SECTOR_SIZE)
  size_t swap_size =disk_size(swap_table.disk)/sectors_in_page;
  swap_table.used_map = bitmap_create(swap_size);
}


void swap_destroy(void)
{
  bitmap_destroy (swap_table.used_map); 
}


void swap_out(void)
{

}


void swap_in(void)
{

}

