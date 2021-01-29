# Dynamic-Memory-Allocation-Simulator
Project written in C to simulate dynamic memory allocation 


  We implemented our own versions of the C system's
malloc() and free() functions. Our versions fulfill the same function as malloc() and free();
however, they also detect common programming errors that users make when using malloc() and
free().

  Our malloc() and free() functions are named mymalloc() and myfree() respectively. In
order to simulate a main memory, we used a global static char array of size 4096. Mymalloc()
takes a size in bytes from the user and returns a pointer to an address in this array where there is
at least as much unallocated space as was requested by the user. In order to manage and keep
track of allocated and unallocated block in the array, data block were treated like a flattened,
contiguous linked list, with alternating blocks of allocated and unallocated memory as “nodes.”
Each of these blocks was preceded by a “metadata” struct, which we created to record the size of
the following usable data block and a flag to indicate status (allocated/unallocated) of the
following block. This struct included a short variable “size,” a char “flag,” and a char “magic,”
which was set to an uncommon character (4 bytes in all). Flag was either set to “y” (allocated) or
“n” (unallocated). In order to check whether a given address was metadata rather than usable
memory space, we looked at the magic and flag characters in combination, considering that there
was a nominal chance that a user would store the uncommon magic character in the array,
followed immediately by a “y” or “n” character. Since the user can type in at least 60 chars, the
probability of the user typing in either n or y and @ is (2/60) * (1/60), which is less than 0.5%.
As the only memory we were able to utilize was that in our main array, we designed our
metadata struct to be as space-efficient as possible, using shorts rather than integer variables. We
also utilize flag for both a status and a magic check rather than using a char array for magic in
order to conserve space and eliminate padding, reducing the size of our metadata to only four
bytes.

  We designed myfree() to take an address and first check whether the address given refers
to a valid metadata pointer in the array. If not, myfree() returns one of three informative error
messages, which also print out the current file name and line number. These errors include a
warning against attempting to free NULL, against attempting to free a pointer which is not
dynamically allocated, against trying to free a pointer which was not allocated my mymalloc(),
and against redundant freeing (trying to free the same pointer more than once). If the address is a
valid pointer, myfree() proceeds to mark the data block referred to by the address as unallocated.
myfree() then runs our coalesce() function, which examines the array from index 0 to 4095 for
any adjacent unallocated memory blocks. If coalesce() identifies two such freed blocks, it
coalesces them to form one larger unallocated block with a greater data capacity.
Our other utility functions included initializearray(), printArray(), and isMeta(). These
functions improved our program by modularizing it, providing a more elegant and
easy-to-manipulate program. initializearray() initialized the array in the case that no blocks had
been allocated into it yet; printArray() printed out the contents of the array within a customizable
range; and isMeta() takes an address and returns an integer to indicate whether the given address
is a valid pointer (1 == valid, 0 == invalid). Signatures of all functions used in mymalloc.c, as
well as any definitions, declarations, structures, and macros, were included in a mymalloc.h
library file.

  To test our program, we wrote and ran a series of “workloads” found in memgrind.c,
including workloads A through F. For workloads A through D, we followed the specifications
provided in the assignment description.For workload E, we randomly allocated and freed blocks of a size of 100 bytes (including metadata) until a total of 10,000 bytes’ worth of data had been allocated. Upon allocating a total of 10,000 bytes, workload E then frees all allocated blocks before terminating. This was done in order to test the ability of our mymalloc() and myfree() programs to handle large individual block sizes. In workload F, we allocated and subsequently freed a pointer i, testing every possible size of i between the minimum possible size of one byte and the maximum possible size of 4092 bytes (the size of a metadata struct subtracted from the size of the main array). In this workload, the size of individual blocks of allocated data were varied; this was done so as to test mymalloc() and myfree()’s ability to handle a wide range of different block sizes, and in particular to ensure that the functions would handle a maximum allocation of its maximum size without malfunctioning or permitting additional allocations without freeing the size-4092 block.. The data which we collected consisted of the total
time taken to compile each workload; each workload was run 100 times consecutively, and the
an average of these times was calculated and reported. We found that the average compilation
times of workloads A through F were approximately 4.45, 54.15, 4.53, 4.95, 31.59, and 120.65
microseconds, respectively. The program gives these numbers in fraction form.
Workload F, which varied the size of the data blocks allocated by a user, took
significantly longer relative to the other workloads. This is because F was designed to malloc and
free every valid number for an array size. We chose this test because since it was so intensive we
knew the algorithm would be fast enough if it did this test under a second. The functions still
managed to handle 100 iterations of each of the seven workloads with an average compilation
time of less than one second. Based on these results, we can conclude that our implementation of
mymalloc() and myfree() successfully fulfill the same basic function served by malloc() and
free(), while also making improvements to detect common errors on a user’s part.


