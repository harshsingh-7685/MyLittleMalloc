#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Including the mymalloc.h header file
#include "mymalloc.h"

//Metadata structure to store size and free information
typedef struct Metadata {
    size_t size;
    int free;
} meta;

//Define the memory array
#define MEMLENGTH 4096
static char memory[MEMLENGTH];

void initialize_mem() {
    //Initialize the entire memory array to 0
    for(int i = 0; i < MEMLENGTH; i++)
        memory[i] = 0;

    //Set the head block as free with the size accounting for metadata
    meta *metadata = (meta *)&memory[0];
    metadata->size = MEMLENGTH - sizeof(meta);
    metadata->free = 1; // Mark as free
}

//Merge adjacent free blocks
void merge() {
    meta *curr = (meta *)memory;
    meta *next;

    while ((char *)curr < memory + MEMLENGTH) {
        next = (meta *)((char *)curr + sizeof(meta) + curr->size);

        //Merge with next block, else move on
        if ((char *)next < memory + MEMLENGTH && curr->free && next->free)   //(char *)next < memory + MEMLENGTH makes sure we don't go out of bounds
            curr->size += sizeof(meta) + next->size;
        else
            curr = next;
    }
}
void *mymalloc(size_t size, char *file, int line) {
    if (size <= 0 || size + sizeof(meta) > MEMLENGTH) {
        printf("Error 1: Invalid size in mymalloc() at %s:%d\n", file, line);
        return NULL;
    }

    //If memory is uninitialized
    if (((meta*)memory)->size == 0)
        initialize_mem();

    //Merge any free blocks before allocating
    merge();

    //Align size to 8 bytes
    size_t alignSize = (size + 7) & ~7;

    //Iterate through memory
    meta *metad = (meta *)memory;
    while ((char *)metad < memory + MEMLENGTH) {
        if (metad->free && metad->size >= alignSize) {

            //Split blocks if needed
            if (metad->size >= alignSize + sizeof(meta) + 1) {
                meta *newMeta = (meta *)((char *)metad + sizeof(meta) + alignSize);

                newMeta->size = metad->size - alignSize - sizeof(meta);
                newMeta->free = 1;
                metad->size = alignSize;
            }

            //Mark as used
            metad->free = 0;
            return (char *)metad + sizeof(meta);
        }

        //Move to next block
        metad = (meta *)((char *)metad + sizeof(meta) + metad->size);
    }

    printf("Error 2: Not enough memory in mymalloc() at %s:%d\n", file, line);
    return NULL;
}

void myfree(void *ptr, char *file, int line) {
    if (ptr == NULL){
        printf("Error 1: Invalid pointer in myfree() at %s:%d\n", file, line);
        return;
    }

    meta *metad = (meta *)((char *)ptr - sizeof(meta));

    //Check if pointer is in bounds
    if (metad < (meta *)memory || (char *)metad >= memory + MEMLENGTH)
    {
        printf("Error 2: Pointer out of bounds in myfree() at %s:%d\n", file, line);
        return;
    }

    //Check if pointer is already free
    if (metad->free){
        printf("Error 3: Pointer already free in myfree() at %s:%d\n", file, line);
        return;
    }

    //Check if pointer aligns with allocated block
    meta *valid = (meta *)memory;
    int isValid = 0;
    while((char *)valid < memory + MEMLENGTH) {
        if(valid == metad){
            isValid = 1;
            break;
        }
        
        //Move to next block
        valid = (meta *)((char *)valid + sizeof(meta) + valid->size);
    }

    //If pointer was not allocated by mymalloc(), return;
    if(!isValid){
        printf("Error 4: Pointer not allocated by mymalloc() in myfree() at %s:%d\n", file, line);
        return;
    }

    //Mark as free
    metad->free = 1;
}