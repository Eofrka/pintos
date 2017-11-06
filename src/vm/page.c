#include "vm/page.h"
#include "threads/malloc.h"
#include <debug.h>
#include <stdio.h>


/* New functions defintions. */
/* Returns a hash value for supplementary page table entry spte. */
unsigned spte_hash(const struct hash_elem *he, void* aux UNUSED)
{
  const struct supplemental_page_table_entry* spte = hash_entry(he, struct supplemental_page_table_entry, he);
  return hash_bytes(&spte->uvaddr, sizeof spte->uvaddr);
}

/* Returns true if spte_a precedes spte_b. */
bool spte_less (const struct hash_elem* he_a, const struct hash_elem* he_b, void* aux UNUSED)
{
  const struct supplemental_page_table_entry* spte_a = hash_entry(he_a, struct supplemental_page_table_entry, he);
  const struct supplemental_page_table_entry* spte_b = hash_entry(he_b, struct supplemental_page_table_entry, he);
  return spte_a->uvaddr < spte_b->uvaddr;
}

/* Initializes the supplemental_page_table. */
void spt_init(void)
{
  bool success = hash_init(&spt, spte_hash, spte_less, NULL);
  if(!success)
  {
    PANIC("supplemental page table initializing is failed");
  }
}

/* Destroys the supplemental_page_table. */
void spt_destroy(void)
{
  hash_destroy(&spt, spte_free);
}

//TODO: switch cases for enum SPTE_STATE
/* Frees the spte. */
void spte_free(struct hash_elem* spte_he, void* aux  UNUSED)
{
  struct supplemental_page_table_entry* spte = hash_entry(spte_he, struct supplemental_page_table_entry, he);
  SAFE_FREE(spte);
}

/* Creates a supplemental_page_table_entry. */
struct supplemental_page_table_entry* spte_create(void)
{
  struct supplemental_page_table_entry* spte =(struct supplemental_page_table_entry*)malloc(sizeof(struct supplemental_page_table_entry));
  if(spte == NULL)
  {
    PANIC("not enough memory to allocate struct supplemental_page_table_entry");
  }
  return spte;
}


/* Prints the spte informations. */
void spte_print(struct hash_elem* spte_he)
{
  struct supplemental_page_table_entry* spte = hash_entry(spte_he, struct supplemental_page_table_entry, he);
  printf("****************************************\n");
  printf("uvaddr: 0x%x\n", (uint32_t)spte->uvaddr);
  switch(spte->state)
  {
    case SPTE_FILE:
      printf("SPTE_STATE: SPTE_FILE\n");    
      break;
    case SPTE_SWAP:
      printf("SPTE_STATE: SPTE_SWAP\n");    
      break;
    case SPTE_LOADED:
      printf("SPTE_STATE: SPTE_LOAD\n");    
      break;
  }
  printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

}


/* Inserts spte into spt. If the same uvaddr is already in the spt, PANIC. */
bool spte_insert(struct hash* spt, struct hash_elem* spte_he)
{

  if(hash_insert(spt, spte_he) != NULL)
  {
    PANIC("There is already the same uvaddr spte");
  }
  else
  {
    return true;
  }
}


