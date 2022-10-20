#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>
#include <stdbool.h>


/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

struct memoryList {
    // doubly-linked list
    struct memoryList *last;
    struct memoryList *next;

    int size;            // How many bytes in this block?
    char alloc;          // 1 if this block is allocated,
    // 0 if this block is free.
    void *ptr;           // location of block in memory pool.
};

strategies myStrategy = NotSet;    // Current strategy


size_t mySize;
void *myMemory = NULL;

static struct memoryList *head;
static struct memoryList *next;


/* initmem must be called prior to mymalloc and myfree.

   initmem may be called more than once in a given exeuction;
   when this occurs, all memory you previously malloc'ed  *must* be freed,
   including any existing bookkeeping data.

   strategy must be one of the following:
		- "best" (best-fit)
		- "worst" (worst-fit)
		- "first" (first-fit)
		- "next" (next-fit)
   sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
*/

void initmem(strategies strategy, size_t sz) {
    myStrategy = strategy;

    /* all implementations will need an actual block of memory to use */
    mySize = sz;

    if (myMemory != NULL) free(myMemory); /* in case this is not the first time initmem2 is called */

    /* TODO: release any other memory you were using for bookkeeping when doing a re-initialization! */


    myMemory = malloc(sz);

    /* TODO: Initialize memory management structure. */
    head = (struct memoryList *) malloc(sizeof(struct memoryList));
    // head->next and head-> last is set to head to make sure we have a circular linked list
    head->next = head;
    head->last = head;
    // size is set to the size of the initialized memory sz
    head->size = sz;
    // makes sure that the head is not allocated
    head->alloc = 0;
    // makes sure that head points to the start of the memory pointer
    head->ptr = myMemory;
    // sets the next pointer to the head as a initialization
    next = head;

}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1 
 */

void *mymalloc(size_t requested) {
    assert((int) myStrategy > 0);
    // initializes block that is where the fits are allocated to.
    struct memoryList *block = NULL;
    // chooses which of the strategies used, where block gets the values of the node from the linked list
    switch (myStrategy) {
        case NotSet:
            return NULL;
        case First:
            block = FirstFit(requested);
            break;
        case Best:
            block = BestFit(requested);
            break;
        case Worst:
            block = WorstFit(requested);
            break;
        case Next:
            block = NextFit(requested);
            break;
        default:
            return NULL;
    }
    // should never get here, but just in case something went horribly wrong
    if (block == NULL) {
        printf("Something horrible has happened (╯‵□′)╯︵┻━┻\n");
        return NULL;
    }
    // marks the block as allocated
    block->size = 1;
    // if the block size and the requested size maches we just move the next pointer, since there's no remaining
    // memory in that fit
    if (block->size == requested) {
        // moves the next pointer
        next = block->next;

        // handles the remaining memory / creates a new space for the remaining memory where the left memory is right after
        // the block we made before
    } else if (block->size > requested) {
        // initializes the ledt/ remaining memory block
        struct memoryList *left = malloc(sizeof(struct memoryList));
        // moves the last pointer of the remainder to the previous block
        left->last = block;
        // moves the next pointer from left to what the pointer pointed at
        left->next = block->next;
        // makes sure that the block after left points to left and not the block before left
        left->next->last = left;
        // makes the block go to what's left after the block
        block->next = left;
        // sets the size of the left block to what was left after the block took what it needed
        left->size = block->size - requested;
        // sets the block size to the requested memory
        block->size = requested;
        // marks the left as unallocated
        left->alloc = 0;
        // lets the left pointer to the end of the blocks ptr
        left->ptr = block->ptr + requested;
        // sets the next value to left
        next = left;
    }
        // should never run this but just in case.
    else {
        printf("something went horribly wrong again sadge ┻━┻ ︵ヽ(`Д´)ﾉ︵ ┻━┻\n");
        return NULL;
    }
    // returns the pointer to the just attached block.
    return block->ptr;
}

// finds the first available block
void *FirstFit(size_t requested) {
    // starts the traverse through the linked list at the head
    struct memoryList *current = head;
    // the pointer to the node that we're trying to find initialized as NULL
    struct memoryList *memoryListPtr = NULL;
    // only runs if the memory pointer is not found and there still is a current to find
    while (memoryListPtr == NULL && current) {
        // checks if there's avaivable space for the current node, and that the block is not allocated
        if (current->size >= requested && current->alloc == 0) {
            // allocated the block
            // sets the memorypointer to the current
            memoryListPtr = current;
        }
        // amikes the current the next to traverse through the llinkedlist
        current = current->next;
        // if head is reached we break
        if (current == head) break;
    }
    // returns the ptr we're looking for
    return memoryListPtr;
}

// finds the best available block
void *BestFit(size_t requested) {
    // starts the traverse through the linked list at the head
    struct memoryList *current = head;
    // the pointer to the node that we're trying to find initialized as NULL
    struct memoryList *memoryListPtr = NULL;
    // set's the starting remainder as the max value for an integer to make sure the first fit found is stored
    int remaining = INT_MAX;
    // loop to travers through the linked list
    while (current) {
        // checks if the memory block is big enough and is allocated
        if (current->size >= requested && current->alloc == 0) {
            // if the size is smaller than the previous remaining size it's a better fit and should now be stored
            // instead
            if ((current->size - requested) < remaining) {
                // calculates the new remainding to check for
                remaining = current->size - requested;
                // set's the pointer to the current
                memoryListPtr = current;
            }
        }
        // sets the current to the next to traverse through the linked list
        current = current->next;
        // breaks if the head is reached.
        if (current == head) break;
    }
    // returns the ptr we're looking for
    return memoryListPtr;
}

// finds the worst available block
void *WorstFit(size_t requested) {
    // starts the traverse through the linked list at the head
    struct memoryList *current = head;
    // the pointer to the node that we're trying to find initialized as NULL
    struct memoryList *memoryListPtr = NULL;
    // sets the remaining to 0 to make sure that the the first fit found is automatically the worst
    int remaining = 0;
    // loops through the list
    while (current) {
        // checks if the memory block is big enough and is allocated
        if (current->size >= requested && current->alloc == 0) {
            // checks that if the current memory block is thw worst fit
            if ((current->size - requested) > remaining) {
                // calculates the new remaining
                remaining = current->size - requested;
                // sets the new memmoryListPtr
                memoryListPtr = current;
            }
        }
        // sets the current to the next to traverse through the linked list
        current = current->next;
        // breaks if the head is reached.
        if (current == head) break;
    }
    // returns the ptr we're looking for
    return memoryListPtr;
}

// finds the next fit
void *NextFit(size_t requested) {
    // set's the current to next since it's a next fit
    struct memoryList *current = next;
    // the pointer to the node that we're trying to find initialized as NULL
    struct memoryList *memoryListPtr = NULL;
    // only runs if the memory pointer is not found and there still is a current to find
    while (memoryListPtr == NULL && current) {
        // checks if there's avaivable space for the current node, and that the block is not allocated
        if (current->size >= requested && current->alloc == 0) {
            // allocated the block
            // sets the memorypointer to the current
            memoryListPtr = current;
        }
        // amikes the current the next to traverse through the llinkedlist
        current = current->next;
        // breaks if next is reached.
        if (current == next) break;
    }
    // returns the ptr we're looking for
    return memoryListPtr;
}

/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void *block) {
    // node to search through the block
    struct memoryList *searcher = head;
    // traverses through the list until the block is found
    while (searcher) {
        // checks if the searchar is the block and then breaks
        if (searcher->ptr == block) break;

        // set's the searcher to the next to traverse
        searcher = searcher->next;
    }

    // unallocates the searcher
    searcher->alloc = 0;

    // checks if the next is unallocated to combine with the current one
    if ((searcher->next->alloc != 1) && (searcher->next != head)) {
        // makes a next struct to be ready for merch later
        struct memoryList *afterSearcher = searcher->next;
        // sets the next pointer to the afterSearcher next
        searcher->next = afterSearcher->next;
        // makes sure that the one after searcher refers to searcher on the last.
        searcher->next->last = searcher;
        // adds the sizes together
        searcher->size += afterSearcher->size;

        // if head or tail is aftersearcher they are now pointing at searcher
        if (afterSearcher == head) {
            head = searcher;
        }
        if (afterSearcher == next) {
            next = searcher;
        }
        // frees the afterSearcher
        free(afterSearcher);
    }
    // checks if the previous is unallocated to combine with the current one
    if ((searcher->last->alloc != 1) && (searcher->last != head)) {
        // makes a next struct to be ready for merch later
        struct memoryList *beforeSearcher = searcher->last;
        // sets the before next  pointer to the searcher next pointer
        beforeSearcher->next = searcher->next;
        // makes sure that the one after beforeSearcher refers to searcher on the last.
        beforeSearcher->next->last = beforeSearcher;
        // adds the sizes together
        beforeSearcher->size += searcher->size;

        // if head or tail is searcher they are now pointing at searcher
        if (searcher == head) {
            head = beforeSearcher;
        }
        if (searcher == next) {
            next = beforeSearcher;
        }
        // frees the Searcher
        free(searcher);
    }
}

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when referred to "memory" here, it is meant that the
 * memory pool this module manages via initmem/mymalloc/myfree. 
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes() {
    // initializes a counter
    int counter = 0;
    // makes a searcher node
    struct memoryList *searcher = head;
    // searches thorugh the linked list
    while (searcher) {
        // if a black of memory is not allocated and the next space is (if it wasn't nothing would be allocate) and
        // then counts upwards
        if (searcher->alloc == 0 && searcher->next->alloc == 1) counter++;

        // moves the searcher forward
        searcher = searcher->next;

        // breaks if searcher finds the head because circular linked list and stuff
        if (searcher == head) break;
    }
    // returns the memory holes
    return counter;
}

/* Get the number of bytes allocated */
int mem_allocated() {
    // initialises the counter
    int allocated = 0;

    struct memoryList *searcher = head;
    // searches thorugh the linked list
    while (searcher) {
        // if memory is allocated adds it to the allocated sum
        if (searcher->alloc == 1) allocated += searcher->size;

        // moves the searcher forward
        searcher = searcher->next;

        // breaks if searcher finds the head because circular linked list and stuff
        if (searcher == head) break;
    }
    // returns how much is allocated
    return allocated;
}

/* Number of non-allocated bytes */
int mem_free() {
    // because mem total is given this is trivial because at this point i am lazy.
    return mem_total() - mem_allocated();
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free() {
    return 0;
}

/* Number of free blocks smaller than or equals to "size" bytes. */
int mem_small_free(int size) {
    return 0;
}

char mem_is_alloc(void *ptr) {
    return 0;
}

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may find them useful.
 */


//Returns a pointer to the memory pool.
void *mem_pool() {
    return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total() {
    return mySize;
}


// Get string name for a strategy. 
char *strategy_name(strategies strategy) {
    switch (strategy) {
        case Best:
            return "best";
        case Worst:
            return "worst";
        case First:
            return "first";
        case Next:
            return "next";
        default:
            return "unknown";
    }
}

// Get strategy from name.
strategies strategyFromString(char *strategy) {
    if (!strcmp(strategy, "best")) {
        return Best;
    } else if (!strcmp(strategy, "worst")) {
        return Worst;
    } else if (!strcmp(strategy, "first")) {
        return First;
    } else if (!strcmp(strategy, "next")) {
        return Next;
    } else {
        return 0;
    }
}


/* 
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory() {
    return;
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */
void print_memory_status() {
    printf("%d out of %d bytes allocated.\n", mem_allocated(), mem_total());
    printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n", mem_free(), mem_holes(),
           mem_largest_free());
    printf("Average hole size is %f.\n\n", ((float) mem_free()) / mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv) {
    strategies strat;
    void *a, *b, *c, *d, *e;
    if (argc > 1)
        strat = strategyFromString(argv[1]);
    else
        strat = First;


    /* A simple example.
       Each algorithm should produce a different layout. */

    initmem(strat, 500);

    a = mymalloc(100);
    b = mymalloc(100);
    c = mymalloc(100);
    myfree(b);
    d = mymalloc(50);
    myfree(a);
    e = mymalloc(25);

    print_memory();
    print_memory_status();

}
