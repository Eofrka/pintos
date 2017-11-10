#include "vm/swap.h"
#include "threads/vaddr.h"
#include "devices/disk.h"
/* Initializes the swap_table. */
void swap_init(void)
{
  swap_table.disk = disk_get(1,1);
  size_t sectors_in_page = PGSIZE/DISK_SECTOR_SIZE;
   //# of sectors / (PGSIZE/DISK_SECTOR_SIZE)
  size_t swap_size =disk_size(swap_table.disk)/sectors_in_page;
  swap_table.used_map = bitmap_create(swap_size);
}

/* Destroys the swap_table. */
void swap_destroy(void)
{
  bitmap_destroy (swap_table.used_map); 
}

/* Swap out victim_fte. */
void swap_out(struct supplemental_page_table_entry* victim_spte, struct frame_table_entry* victim_fte)
{

  size_t swap_idx = bitmap_scan(swap_table.used_map, 0, 1, false);
  if(swap_idx == BITMAP_ERROR)
  {
    PANIC("swap disk is full");
  }
  size_t i;
  size_t sectors_in_page = PGSIZE/DISK_SECTOR_SIZE;
  for( i=0; i<sectors_in_page; i++)
  {
    disk_write(swap_table.disk, (swap_idx*sectors_in_page)+i, (uint8_t*)victim_fte->kpage+(i*DISK_SECTOR_SIZE));
  }

  bitmap_flip(swap_table.used_map, swap_idx);
  victim_spte->swap_idx = swap_idx;
  victim_spte->state = SPTE_SWAP;
  victim_spte->fte = NULL;

}

/* Swap in data using spte's inforamtion. */
void swap_in(struct supplemental_page_table_entry* spte, struct frame_table_entry* fte)
{
  size_t swap_idx = spte->swap_idx;
  size_t sectors_in_page = PGSIZE/DISK_SECTOR_SIZE;
  size_t i;
  for(i=0; i<sectors_in_page; i++)
  {
    disk_read(swap_table.disk, (swap_idx*sectors_in_page)+i, (uint8_t*)fte->kpage + (i*DISK_SECTOR_SIZE));
  }
  bitmap_flip(swap_table.used_map, swap_idx);


}


void swap_free(struct supplemental_page_table_entry* spte)
{
  size_t swap_idx = spte->swap_idx;
  bitmap_flip(swap_table.used_map, swap_idx);
}


