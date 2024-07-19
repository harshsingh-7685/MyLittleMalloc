#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>   //For time

#ifndef REALMALLOC
#include "mymalloc.h"
#endif

//Adding some basic tests
void b_test(){
    int x;
    free(&x);

    int *p = malloc(sizeof(int)*2);
    free(p + 1);

    p = malloc(sizeof(int)*100);
    int *q = p;
    free(p);
    free(q);
}

//Implementng test 1
void test1(){
    for(int j = 1; j<=120; j++){
        void *p = malloc(1);
        free(p);
    }
}


//Implementing test 2
void test2() {
    void *ptrs[120];

    for(int j = 0; j<120; j++){
        ptrs[j] = malloc(1);
    }

    for(int j = 0; j<120; j++){
        free(ptrs[j]);
    }
}

//Implementing test 3
void test3(){
    int count = 0;
    void *ptrs[120];

    while(count < 120){
        //Choosing allocate or free
        int num = rand() % 2;

        if(num == 0){
            //Save pointer for free()
            ptrs[count] = malloc(1);
            count++;
        }
        else{
            //Check if count is greater than zero before accessing ptrs
            if (count > 0) {
                //Choosing which pointer to free
                int index = rand() % count;
                free(ptrs[index]);
                ptrs[index] = ptrs[count-1];
                count--;
            }
            //Set count to 0 if count is < 0
            else
                count = 0;
        }
    }

    //Free all pointers
    for(int j = 0; j<120; j++){
        free(ptrs[j]);
    }
}

//Random sizes and orders test
void p_test1() {
    void *ptrs[50];
    int count = 0; //Number of allocated blocks

    for(int i = 0; i < 100; i++) {
        //Choose between allocate and free
        if (rand() % 2 && count < 50) {
            //Allocate between 1 and 64 bytes
            size_t size = (rand() % 64) + 1;
            ptrs[count++] = malloc(size);
        }
        //Free only if there's anything allocated
        else if (count > 0) {
            int index = rand() % count;
            free(ptrs[index]);
            //Move last valid pointer to the freed slot
            ptrs[index] = ptrs[--count];
        }
    }

    //Free any remaining allocations
    while (count > 0) {
        free(ptrs[--count]);
    }
}


void p_test2() {
    //Large number of iterations
    for (int i = 0; i < 1000; i++) {
        //Allocate between 1 and 32 bytes
        size_t size = (rand() % 32) + 1;
        void *ptr = malloc(size);

        //Exit if allocation fails
        if (ptr == NULL) {
            printf("Allocation failed at iteration %d\n", i);
            return;
        }
        free(ptr);
    }
}



int main(int argc, char **argv)
{
    //Seed the random number generator
    srand(time(NULL));
    double time_taken = 0.0;

    b_test();
    printf("Basic test passed\n\n");

    for(int i = 0; i<50; i++){
        clock_t time = clock();

        test1();

        time = clock() - time;
        time_taken += (double)time / CLOCKS_PER_SEC;
    }

    printf("Average time taken for test 1: %f sec\n", time_taken/50);

    time_taken = 0.0;
    for(int i = 0; i<50; i++){
        time_t time = clock();

        test2();

        time = clock() - time;
        time_taken += (double)time / CLOCKS_PER_SEC;
    }

    printf("Average time taken for test 2: %f sec\n", time_taken/50);

    time_taken = 0.0;
    for(int i = 0; i<50; i++){
        time_t time = clock();

        test3();

        time = clock() - time;
        time_taken += (double)time / CLOCKS_PER_SEC;
    }

    printf("Average time taken for test 3: %f sec\n", time_taken/50);

	time_taken = 0.0;
	for(int i = 0; i<50; i++){
		clock_t time = clock();

		p_test1();

		time = clock() - time;
		time_taken = (double)time / CLOCKS_PER_SEC;
	}

   	printf("Total time taken for p_test1 (50 iterations): %f sec\n", time_taken);
    
    time_taken = 0.0;
	for(int i = 0; i<50; i++){
		clock_t time = clock();

		p_test2();

		time = clock() - time;
		time_taken = (double)time / CLOCKS_PER_SEC;
	}

   	printf("Average time taken for p_test2: %f sec\n", time_taken/50);
}
