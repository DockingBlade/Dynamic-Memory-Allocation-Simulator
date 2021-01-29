/*
 ============================================================================
 Name        : Dynamic-Memory-Allocation-Simulator.c
 Author      : Shaan Kalola
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
static char myblock[4096];

typedef struct _metadata{ //our metadata struct
    short size;
    char flag;
    char magic;
}metadata;

void initializearray(){ //function used to make first block
    metadata * intial = (metadata*) myblock;
    intial -> size = 4096-sizeof(metadata);
    intial -> flag = 'n';
    intial -> magic = '@';
}

void coalesce(){ //merges together two adjacent freed blocks
    metadata* ptr = (metadata*)myblock; // lagging pointer
    int index = sizeof(metadata) + ptr->size;
    metadata* next = (metadata*)((char*)ptr + sizeof(metadata) + ptr->size);//leading pointer

    while(index <= 4095){
        if(ptr->flag == 'n' && next->flag == 'n'){//if both freed
            if(isMeta(ptr) && isMeta(next)){
                ptr->size = ptr->size + sizeof(metadata) + next->size;//give ptr all of next's data
                next -> magic = ':';//remove magic numbers
                next -> flag = '}';
                next =(metadata*) ((char*)ptr + sizeof(metadata) + ptr->size);//go to new next
                index = index + sizeof(metadata) +ptr->size;
            }
            else{
                break;//if glitch and aren't actually blocks exits, can tell if the size is valid
            }
        }

        else{
            index += sizeof(metadata) + next->size;//used to loop through the array
            ptr = next;
            next = (metadata*)((char*)next + sizeof(metadata) + next->size);
        }

    }



}

void * mymalloc(size_t size, char*file, int line){
    metadata * checkblock = (metadata*) (myblock);
    if(!isMeta(checkblock)){
      initializearray();//checks if initialized
    }


    short space = size;
    if(space ==0){
	printf("Invalid malloc, need more than zero bytes, returning NULL, FILE: %s, Line : %d\n",file,line);
	return NULL;
    }
    short index= 0;
    while (index < 4096){
         metadata * block = (metadata*)((char*)(myblock) + (index));
         if(block -> flag == 'n' && block->size >= (space)){ //if space is free and big enough
            if(block->size - space > sizeof(metadata)){ // if the space is enough to hold more take then asked for (including metadata) then splits block into two
                short blockSize = block -> size;
                short newblocksize = block->size - space - sizeof(metadata);

                block->size = space;
                block->flag = 'y';
                short newindex = index + sizeof(metadata) + space;
                metadata * newblock = (metadata*)((char*) (myblock)+newindex);
                newblock -> size = newblocksize;
                newblock -> flag = 'n';
                newblock -> magic = '@';
                void * ptr = (void*) ((char*)myblock+index+sizeof(metadata));
                return ptr;
            }
            else{ // else gives all the extra data since it can't hold the metadata for a malloc
              block -> flag = 'y';
              void * ptr = (void*) ((char*)myblock+index+sizeof(metadata));
              return ptr;
            }
         }
         else{
           index = index + sizeof(metadata) + block->size;//loops
         }
    }
    printf ("Not enough memory, FILE : %s, LINE : %d\n",file,line); //if fails says not enough memory
    return NULL;
}


void printArray(){ //utility function ignore
    int i = 0;
    while (i<21){
        printf("myblock[%d]: %c\n", i, myblock[i]);
        i++;
    }
    printf("\n");
}



void * myfree(void*ptr, char*file, int line){
    if (ptr == NULL){ // checks if NULL
        printf("Error: attempted to free NULL pointer, maybe a redunant freeing, FILE: %s, LINE : %d\n",file,line);
        return NULL;
    }
    if (ptr < (void*)myblock || ptr > (void*)&(myblock[4095])){ //checks if its in the array
        printf("Error: this ptr is not dynamically allocated, FILE: %s, LINE :d\n",file,line);
        return NULL;
    }
    metadata* meta = (metadata*)((char*)ptr - sizeof(metadata));
    if(!isMeta(meta)){ //checks that the ptr is actually a block using magic numbers
        printf("Error: this ptr was not allocated by malloc FILE: %s, LINE :d\n", file, line);
        return NULL;
    }
    if (meta -> flag == 'n'){ //checks if already freed
      printf("Error: Redunant freeing, FILE: %s, LINE:d\n", file,line);
      return NULL;
    }
    meta -> flag = 'n';//frees finally
    coalesce();
    return NULL;


}



int isMeta(metadata * ptr){//checks if metadata with magic numbers
    if(ptr->magic == '@' && ptr -> flag == 'n'){
        return 1;
    }
    else if(ptr->magic == '@' && ptr -> flag == 'y'){
        return 1;
    }
    else{
        return 0;
    }
}

void printMeta (char*ptr){// debugging function ignore
    ptr  = ptr - sizeof(metadata);
    printf("pointer is %p\n ", ptr);
}
