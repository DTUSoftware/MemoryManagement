# Memory Management

_62588 Operating Systems - Mandatory Assignment: Memory Management_

The problem will focus on memory. You will implement your own version of malloc() and free(), using a variety of
allocation strategies.

You will be implementing a memory manager for a block of memory. You will implement routines for allocating and
deallocating memory, and keeping track of what memory is in use. You will implement the following four strategies for
selecting in which block to place a new requested memory block:

1. **First-fit**:
   Select the first suitable block with the smallest address.
2. **Best-fit**:
   Select the smallest suitable block.
3. **Worst-fit**:
   Select the largest suitable block.
4. **Next-fit**:
   Select the first suitable block after the last block allocated (with wraparound from end to beginning).

Here, "suitable" means "free, and large enough to fit the new data".

## Functions to implement

Here are the functions you will need to implement:

- **initmem()**:
  Initialize memory structures.
- **mymalloc()**:
  Like malloc(), this allocates a new block of memory.
- **myfree()**:
  Like free(), this deallocates a block of memory.
- **mem_holes()**:
  How many free blocks are in memory?
- **mem_allocated()**:
  How much memory is currently allocated?
- **mem_free()**:
  How much memory is NOT allocated?
- **mem_largest_free()**:
  How large is the largest free block?
- **mem_small_free()**:
  How many small unallocated blocks are currently in memory?
- **mem_is_alloc()**:
  Is a particular byte allocated or not?

A structure has been provided for use to implement these functions. It is a doubly-linked list of blocks in memory (both
allocated and free blocks). Every malloc and free can create new blocks, or combine existing blocks. You may modify this
structure, or even use a different one entirely. However, do not change function prototypes or files other than mymem.c.

**IMPORTANT NOTE:**
**_Regardless of how you implement memory management, make sure that there are no adjacent free blocks. Any such blocks
should be merged into one large block._**

A few functions are given to help you monitor what happens when you call your functions. Most important is the **
try_mymem()** function. If you run your code with `mem -try <args>`, it will call this function, which you can use to
demonstrate the effects of your memory operations. These functions have no effect on test code, so use them to your
advantage.

## Running the code

After running `make`, run

1. `mem` to see the available tests and strategies.
2. `mem -test <test> <strategy>` to test your code with provided tests.
3. `mem -try <args>` to run your code with your own tests (the try_mymem function).

You can also use `make test` and `make stage1-test` for testing. `make stage1-test` only runs the tests relevant to
stage 1.

Running `mem -test -f0 ...` will allow tests to run even after previous tests have failed. Similarly, using "**all**"
for a test or strategy name runs all of the tests or strategies. Note that if "**all**" is selected as the strategy, the
4 tests are shown as one.

One of the tests, "**stress**", runs an assortment of randomized tests on each strategy. The results of the tests are
placed in **tests.out**. You may want to view this file to see the relative performance of each strategy.

## Project Stages

### Stage 1

Implement all the above functions, for all the 4 strategy in a group. Use `mem -test all first` to test your
implementation.

### Stage 2

You should answer the 10 questions asked below together in a group. The strategy is passed to initmem(), and stored in
the global variable "**myStrategy**". Some of your functions will need to check this variable to implement the correct
strategy.

You can test your code with `mem -test all worst`, etc., or test all 4 together with `mem -test all all`. The latter
command does not test the strategies separately; your code passes the test only if all four strategies pass.

## Questions

Answer the following questions as part of your report.

1. **Why is it so important that adjacent free blocks not be left as such?**
   **What would happen if they were permitted?**

   It is important that adjacent free blocks are not left unattended because:
   When several programs have been allocated and deallocated, it will eventually leave gabs in the memory between the
   used memory. If the block to be freed is next to an unallocated block, and we don't combine them, then we will
   essentially end up with a bunch of unallocated blocks next to each other, and we won't be able to allocate memory
   that is bigger than a certain size, even though there is enough unallocated memory.

2. **Which function(s) need to be concerned about adjacent free blocks?**

   ***myfree(void \*block)*** needs to be concerned about adjacent free blocks in order to properly merge adjacent free
   blocks together when freeing. The other functions doesn't need to be concerned about adjacent free blocks, since they
   should be merged together by ***myfree(void \*block)***.

3. **Name one advantage of each strategy.**

   ***First Fit***:  
   The advantage of first fit is that it is quite fast, since when it finds the first fit it instantly allocates the
   memory, and it's quite simple to implement.

   ***Best Fit***:  
   The advantage of best fit is that it will use the memory block which is closest to the memory needed, so there will
   be as little as possible remaining memory in that block. Reduce wasted space in the memory pool.

   ***Worst Fit***:  
   The advantage of worst fit is that it will alaways allocate the the largest block, so there won't be a lot of big
   unused memory blocks - less small holes.

   ***Next Fit***:  
   The advantage of next fit is the same as first fit, that it will be quite fast at allocating the memory since it
   doesn't have to run through the entire linked list, but only find the fit after the next. But compared to first fit
   that will allocate at the start if it can, next fit will allocate throughout and start after where the last one was
   allocated, leaving a more evenly distributed memory pool.  
   In certain circimstances, for example if you are allocating a lot of blocks, with Next Fit, you do not have to go
   through all of the memory that you just allocated, but just start off from where you currently are.

4. **Run the stress test on all strategies, and look at the results (tests.out). What is the significance of "Average
   largest free block"?**

   The significanse of Average largest free block, is that if there is a large block avaible it will be easier to
   allocate a big block of memory, and there should generally be less failed allocations.

   **Which strategy generally has the best performance in this metric? Why do you think this is?**

   Best fit has the "best performance" in this metric, since it will try to use the smallest block of memory possible,
   the bigger blocks will be left for last.

5. **In the stress test results (see Question 4), what is the significance of "Average number of small blocks"?**

   The significanse is that, a lot of small blocks can be hard to allocate, since they might be too small for
   allocation, and a lot of small blocks will take a longer time to compact.

   **Which strategy generally has the best performance in this metric? Why do you think this is?**

   Worst fit has the "best performance" in this metric, since it doesn't have a lot of small blocks. This makes sense
   since worst fit will always allocate the largest block and thereby the block of unused memory will always be as large
   as possible.

6. Eventually, the many mallocs and frees produces many small blocks scattered across the memory pool. There may be
   enough space to allocate a new block, but not in one place. It is possible to compact the memory, so all the free
   blocks are moved to one large free block.  
   **How would you implement this in the system you have built?**

   Because the system is built using a doubly linked list, you can either move all of the free blocks to the end of the
   memory pool, and merge them together into one large block one-by-one, or you can free each of the free memory blocks,
   keep the size, and create a new block at the end of the memory pool with the size of all the freed memory blocks.  
   The problem with performing the compaction in this way, is that your pointers to the memory would have to be changed.
   If you would like to keep your memory pointers intact, you would instead have to relocate all of the current
   allocated memory, by performing a bunch of malloc and freeing.

7. **If you did implement memory compaction, what changes would you need to make in how such a system is invoked
   (i.e. from a user's perspective)?**

   Ideally the user would not know nor notice that memory compaction was implemented, but it would take up quite a bit
   of CPU-load to perform the compaction, in terms of IO, etc..  
   Because of this, you would either run it as a manual "garbage collection" task invoked by the user, or you could
   perform the compaction only when it is needed (i.e. trying to allocate a big program which does not fit into any
   current free block, but there is enough free memory in total in the memory pool), or you could perform the compaction
   when the user is not busy performing other actions (use idle-time to make future actions more effecient instead of
   wasting idle-time) and stop the compaction when the user resumes activity, and you could actually make a mix of all
   of them.

8. **How would you use the system you have built to implement realloc?**
   (Brief explanation; no code)

   Realloc is be relatively easy if you have adjacent memory on the "right side" of the current memory, big enough to
   hold the desired amount of memory. You would then simply "decrease" the size of the adjacent free block and move its
   pointer, and increase the size of the memory block you are trying to reallocate.

   If that is not possible, you would have to call malloc with new requested size, and then you write the old data to
   the newly allocated block, and then you free your old block of memory, after moving the pointer to the new block of
   memory.

9. **Which function(s) need to know which strategy is being used? Briefly explain why this/these and not others.**

   Only ***mymalloc(size_t requested)*** needs to know which strategy is being used, in order to pick the currect block
   of memory, in accordance to the strategy specifications. Technically, ***initmem*** also "requires" to know the
   strategy being used, in order to adjust the global variable, which ***mymalloc(size_t requested)*** uses.

10. **Give one advantage of implementing memory management using a linked list over a bit array, where every bit tells
    whether its corresponding byte is allocated.**

    Using a linked list the number of elements/blocks does not have to be predefined, and you can change the size of
    blocks without risk of data overflowing. Link list should also make the process run faster since it can make bigger
    jumps, where a bit array you would have to iterate over every bit in an allocated block to find an unallocated,
    since it's harder to determine the size of the blocks, where in a linked list you just go from allocated to
    unallocated in possibly one jump.
