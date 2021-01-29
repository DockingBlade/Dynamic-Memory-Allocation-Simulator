/*
 * mymalloc.h
 *
 *  Created on: Jan 29, 2021
 *      Author: shaan
 */
#ifndef MYMALLOC_H
#define MYMALLOC_H
#include<stdio.h>
#include<stdlib.h>

//necessary functions
void * mymalloc(size_t,char*,int);
void * myfree(void*,char*,int);

//macro definitions
//#define malloc(size_t) mymalloc(size_t)
//#define free(void*) myfree(void*)

#define malloc(a) mymalloc(a,__FILE__,__LINE__)
#define free(b) myfree(b,__FILE__,__LINE__)

//typedef struct _metadata{
//    short size;
//    char flag;
//    char magic[2];
//}metadata;

//main char array
static char myblock[4096];

//metadata struct
//struct metadata;
typedef struct _metadata metadata;

//extra utility functions
void coalesce();
void initializearray();
void printArray();
int isMeta(metadata *);

#endif //end of MYMALLOC.H
