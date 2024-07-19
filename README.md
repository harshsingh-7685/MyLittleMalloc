Harsh Singh, NETID: hks59
Assignment 1: My Little Malloc

struct Metadata:
Includes information such as the size of the block and whether the block is free or not.

initialize_mem():
Sets all values in the memory to 0 , it also sets the metadata for the head to have the correct size and free information.

merge():
Iterates through the memory once, and merges adjacent blocks if they are free.

mymalloc(size_t size,  char *file, int line):
1. If the size is invalid, prints error message, returns NULL.
2. If memory not already initialized, calls initialize_mem().
3. Aligns size to 8
4. Calls merge()
5. Itercates through the memory and returns a pointer to the first free block that is large enough to satisfy the request.
    (If the memory is too big, it splits the block to an appropriate size.)
6. Marks block as in use and return a memory address to the user.
7. If no block is large enough, prints error message and returns NULL.
8. The codes for the error messages represent where in the code the problem occures,
    Error 1 is before Error 2 in the actual code of mymalloc()

myfree(void *ptr,  char *file, int line):
1. If the pointer is NULL, prints error message and returns.
3. Checks if the pointer is in the heap.
4. Checks if the pointer is already free.
4. Runs a loop to check if ptr was allocated by mymalloc().
2. Sets the metadata to free.
8. Error messages follow the same pattern as those in mymalloc(), Error 1 is before Error 2 in the actual code of myfree(),
    so on and so forth.
---------------------------------------------------------------------------------------------------------------------------------
Test Cases (all tests except memtest.c and b_test() are repeated 50 times in the main function):
memtest.c:
    memtest.c runs properly if MEMSIZE in mymalloc.c is larger than 4608.

memgrind.c:
    b_test():
        Includes error detection test cases for free() as outlined in the assignment.
    test1():
         mallocs and immediately frees a 1 byte object 120 times.
    test2():
        Uses malloc to get 120 1 byte objects, it then frees all 120 objects.
    test3():
        1. Randomly chooses to allocate or free a 1 byte object until 120 objects have been allocated.
        2. Once number of objects is = 0 or is <= 0, count is set to stay at zero to prevent infinite loops.
        3. It then frees all 120 objects.
    p_test1():
        1. This test allocates and randomly frees blocks of random sizes (between 1 and 64 bytes).
        2. Time is present as total of all 50 runs because double precision is used.
    p_test2():
        This test stress the memory allocation algorithm by allocating and freeing blocks of random sizes(between 1 and 32 bytes)
        a large number of times.

Average times for each test:
Average time taken for test 1: 0.000002 sec
Average time taken for test 2: 0.000086 sec
Average time taken for test 3: 0.002781 sec
Total time taken for p_test1 (50 iterations): 0.000006 sec
Average time taken for p_test2: 0.000001 sec