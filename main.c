#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct node {
    uint32_t size;
    bool mark;
    struct node *next;
} node_t;
static const node_size = sizeof(node_t);

static node_t *free_blocks;
static node_t *used_blocks;
static node_t starter_block;
static uintptr_t stack_bottom;

/* put block into the free blocks */
static void add_to_free(node_t *block) {
    node_t *current = free_blocks;

    // find the position that the block has been partitioned from
    while(current < block || current->next > block){
        if (current >= block->next && (block > current || block < current->next))
            break;
            current = current->next;
    }
    
    // if block connects to current->next
    if (block + block->size == current->next) {
        block->size = current->next->size;
        block->next = current->next->next;
    } else {
        block->next = current->next;
    }

    // if curret connects to block
    if (current + current->size == block) {
        current->size += block->size;
        current->next = block->next;
    } else {
        current->next = block;   
    }
    free(block);
}

static node_t * alloc_more(size_t num) {
    void * voidp;
    node_t *block;

    if (num>4096)
        num = 4096 / sizeof(node_t);
    if ((voidp = sbrk(num * sizeof(node_t))) == (void *) -1)
        return NULL;
        
    block = (node_t *) voidp;
    block->size = num;
    add_to_free(block);
    return free_blocks;
}

// move 
void * gc_malloc(size_t alloc_size) {
    size_t num;
    node_t *current, *previous;

    num = (alloc_size + sizeof(node_t)-1) / sizeof(node_t) +1;
    previous = free_blocks;
    current = previous->next;
    while(current->next!=NULL) {
        if (current->size == num)
            previous->next = current->next;
        else if (current->size > num) {
            current->size -= num;
            current += p->size;
            current->size = num;
        }

        free_blocks = precious;
        
        // add block to used blocks
        if (used_blocks = NULL)
            used_blocks = current->next = p;
        else {
            current->next = used_blocks->next;
            used_blocks->next = current;
        }
        
        // need to allocation more memory
        if (current==free_blocks) {
            current = alloc_more(num);
            if (current==NULL)
                return NULL;
        }
        precious = current;
        current = current->next;
    }
}


// mark and sweep section
// | node | -------------rest of block----------------|
//                 ^ i need to check if something points to that, but not check the node area

void scan_region(uintptr_t start, uintptr_t end) {
    for(uintptr_t current = start; current < end; current++) {
        // loop through used_blocks and see if the address at current points to any of them
        // this would mean dereferencing current as a pointer
        uintptr_t to_check = *current; // dereference current to a pointer
        node_t *walker = used_blocks;
        while(walker->next != NULL) {
            // check if to_check is inside the space specified by walker
            if((to_check >= walker + node_size) && (to_check < walker + node_size + walker->size)) {
                walker->mark = true;
                break;
            }
            walker = walker->next;
        }
    }
}

// scan marked blocks for references to unmarked blocks
void scan_heap() {
    node_t *walker = used_blocks;
    while(walker->next != NULL) {
        if(walker->mark == true) {
            scan_region((uintptr_t)(walker + node_size), (uintptr_t)(walker + node_size + walker->size));
        }
        walker = walker->next;
    }
}

void get_stack_bottom() {
    FILE *stat_file;
    stat_file = fopen("/proc/self/stat", "r");
    assert(stat_file != NULL);
    // got from online, this gets the bottom of the stack from the process file
    fscanf(stat_file,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld "
           "%*ld %*ld %*ld %*ld %*llu %*lu %*ld "
           "%*lu %*lu %*lu %lu", &stack_bottom);
    fclose(stat_file);
}

void gc_init() {
    static bool init_flag;
    if(init_flag == true) {
        // init was already run, exit the function
        return;
    } else {
        init_flag = true;
    }
    get_stack_bottom();
    used_blocks = NULL;
    starter_block.next = free_blocks = &starter_block;
    starter_block.size = 0;
}

void gc_collect() {
    uintptr_t stack_top;
    extern char end, etext; // memory locations provided by linker
    
    if(used_blocks == NULL) {
        // nothing has been allocated yet, so we don't want to run the gc
        return;
    }

    // scan BSS and data segments
    scan_region(&etext, &end);
    
    // get stack top address
    asm volatile ("movl %%ebp, %0" : "=r" (stack_top));
    // scan stack
    scan_region(stack_top, stack_bottom);

    // scan heap
    scan_heap();

    // remove unmarked nodes
    node_t *p
    node_t *prev_p;
    node_t *tp;
    for(prevp = used_blocks, p = used_blocks->next; ; prevp = p, p = p->next) {
        next_chunk:
            if(p->next->mark == false) {
                tp = p;
                p = p->next;
                add_to_free(tp);

                if(used_blocks = tp) {
                    used_blocks = NULL;
                    break;
                }

                prev_p->next = p;
                goto next_chunk;
            }
        p->next->mark = false;
        if(p == used_blocks) {
            break;
        }
    }
    // call free on list
}










