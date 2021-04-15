/* 

French developer !!!!! 
// PSP 
Malloc(x) & Free() Yoshihiro 

*/ 


#include <stdlib.h> 
#include <errno.h> 

#include <sys/types.h> 
#include <unistd.h> 
#include <stddef.h> 
#include <stdarg.h> 

#include "psp.h"

int has_initialized = 0; 
void *managed_memory_start; 
void *last_valid_address; 


struct mem_control_block { 
   int is_available; 
   int size; 
}; 

register char *stack_ptr asm ("$sp"); 
static char alloc_buffer[16*1024*1024]; 

caddr_t 
sbrk_psp (int incr) 
{ 
  static char *heap_end = alloc_buffer; 
  char *prev_heap_end; 

  prev_heap_end = heap_end; 
  if (heap_end + incr > stack_ptr) 
    { 
     return (caddr_t)-1;
    }
  heap_end += incr; 
  return (caddr_t) prev_heap_end; 
} 

void malloc_psp_init() 

{ 
   last_valid_address = sbrk_psp(0); 
   managed_memory_start = last_valid_address; 
   has_initialized = 1; 
} 

void free_psp(void *firstbyte) { 
   struct mem_control_block *mcb; 

   mcb = firstbyte - sizeof(struct mem_control_block); 
   mcb->is_available = 1; 
   return; 
} 

void *malloc_psp(long numbytes) { 

   void *current_location; 


   struct mem_control_block *current_location_mcb; 

    
   void *memory_location; 

   if(! has_initialized)    { 

      malloc_psp_init(); 

   } 
 
   numbytes = ((numbytes+3)&~0x3) + sizeof(struct mem_control_block); 

    
   memory_location = 0; 

   current_location = managed_memory_start; 

   while(current_location != last_valid_address) 

   { 

      current_location_mcb = 

         (struct mem_control_block *)current_location; 

      if(current_location_mcb->is_available) 

      { 

         if(current_location_mcb->size >= numbytes) 

         { 

       
            current_location_mcb->is_available = 0; 

            memory_location = current_location; 

            break; 

         } 

      } 

      current_location = current_location + 

         current_location_mcb->size; 

   } 

   if(! memory_location) 

   { 
      sbrk_psp(numbytes); 

      memory_location = last_valid_address; 

    
      last_valid_address = last_valid_address + numbytes; 

      current_location_mcb = memory_location; 

      current_location_mcb->is_available = 0; 

      current_location_mcb->size = numbytes; 

   } 

    
   memory_location = memory_location + sizeof(struct mem_control_block); 

   return memory_location; 

 } 
