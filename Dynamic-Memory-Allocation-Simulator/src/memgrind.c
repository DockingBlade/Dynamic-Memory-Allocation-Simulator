/*
 * memgrind.c
 *
 *  Created on: Jan 29, 2021
 *      Author: shaan
 */

#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

//A: malloc() 1 byte and immediately free it - do this 150 times;
void workloadA(){
  int i =0;
  for (i = 0; i < 150; i++){
    char * test = (char*) malloc(sizeof(char));
    free(test);
  }
}

/*
B: malloc() 1 byte, store the pointer in an array - do this 150 times.
 Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
 */
 void workloadB(){
    int countmalloc = 0;
    int countfree = 0;
    void * array [50];
    int i = 0;
    for(i =0; i < 3; i++){ //allocates the entire array, then frees it three times
        int countmalloc = 0;
        int countfree = 0;
        while (countmalloc < 50){
        //char * temp = array+ countmalloc;
            array[countmalloc] = (void*) malloc(1);
            countmalloc = countmalloc +1;
        }
        while (countfree < 50){
        //char * temp = array + countfree;
            free(array[countfree]);
            countfree = countfree + 1;
        }
    }
   }
/*
C: Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer
> do this until you have allocated 50 times
- Keep track of each operation so that you eventually malloc() 50 bytes, in total
> if you have already allocated 50 times, disregard the random and just free() on each
 iteration
- Keep track of each operation so that you eventually free() all pointers
 > don't allow a free() if you have no pointers to free()
 */
 void workloadC (){
    int countalloc = 0;
    int index = 0;
    void * array [50];
    while (countalloc < 50){
        int random = rand() % 2;
        if (random == 1){ // malloc
            array[index] = malloc(1);
            index = index +1;
            countalloc = countalloc + 1;
        }
        else if (random == 0){ //free
            if(index == 0){
                continue; // if index is 0, the entire array is freed
            }
            else{
                free(array[index-1]);
                index = index - 1;
            }
        }
    }
    index = index - 1; //current index always empty, must decrement to get to allocated data
    while (index >= 0){
        free(array[index]);
        index--;

    }
 }


 /*
   D: Randomly choose between a randomly-sized malloc() or free()ing a pointer – do this many
										      times (see below)
- Keep track of each malloc so that all mallocs do not exceed your total memory capacity
						     - Keep track of each operation so that you eventually malloc() 50 times
						     - Keep track of each operation so that you eventually free() all pointers
- Choose a random allocation size between 1 and 64 bytes
*/

 void workloadD(){
    int countalloc = 0;
    int countmalloc =0;
    int index = 0;
    void * array [70];
    while (countalloc < 4096 && countmalloc <50){ //uses random number to decide whether to allocate
	int random = rand() % 2;
        if (random == 1){
            int bytes =1 +  rand() %68;
            countalloc = countalloc + bytes;
	    countmalloc = countmalloc + 1;
            array[index] = malloc(bytes);
            index = index +1;
        }
	 else if (random == 0){
            if(index == 0){// if index is 0, the entire array is freed, thus the program continues
                continue;
            }
            else{
                free(array[index-1]);
                index = index - 1;
            }
        }
    }
     index = index - 1;// the current index is constructed to be empty, thus it must be decremented in order to be at a index with a allocated pointer
     while (index >= 0){
        free(array[index]);
        index = index - 1;

    }
 }
/*
 * Test Case E: Similar to D, expect allocates up to a 100 instead (including metadata) instead of 68. Additionally runs until 10000 bits are allocated, which indicates a probability of activating a failure case in mymalloc or myfree. This is impossible in Test D because 50 * 68 = 3400 which less than 4096.
 */
 void workloadE (){
    int countalloc = 0;
    int index = 0;
    void * array [2000];//an array of pointers
    while (countalloc < 10000){
        int random = rand() % 2;
        if (random == 1){
            int bytes =1 +  rand() %96;
            countalloc = countalloc + bytes; //tracks bytes allocated
            array[index] = malloc(bytes);
            index = index +1;
        }
        else if (random == 0){
            if(index == 0){
                continue;
            }
            else{
                free(array[index-1]);
                index = index - 1;
            }
        }
    }
    index = index - 1;//the current index is set to be at an empty index, thus it must be decremented in order to be at an index with a pointer
    while (index >= 0){
        free(array[index]);
        index = index - 1;

    }
 }
// Case F, simply allocate then free i bytes starting from 4092 (which occupies the whole array with the metadata) until the bytes, its a speed test, and though its the longest one the average time is still less than 1 second.
 void workloadF(){
    int bytes = 4092;
    while(bytes > 0){
     char * temp = malloc(bytes);
     free(temp);
     bytes = bytes - 1;
    }
 }

void timer ( void (*f) (), char letter){ //function used to calculate average runtime
  int i =0;
  struct timeval t0;
  struct timeval t1;
  gettimeofday(&t0,0);//takes start time
  for (i = 0; i < 100; i++){
    f(); // runs function
  }
  gettimeofday(&t1,0); //takes end time

  long seconds =  t1.tv_sec - t0.tv_sec;
  long  microseconds = (t1.tv_usec - t0.tv_usec);
  seconds = (1000000)*seconds + microseconds;
//  printf("Total time elapsed for workload %c: %d in microseconds\n", letter, seconds );
  printf("Average time elapsed for workload %c: %ld %ld/%d in microseconds\n", letter, seconds/100, seconds%100, 100 );//prints the average time as a whole number and then a fraction
}

int main (int argc, char** argv){
	timer (workloadA,'A');
	timer (workloadB,'B');
	timer (workloadC,'C');
	timer (workloadD,'D');
	timer (workloadE,'E');
	timer (workloadF,'F');

}
