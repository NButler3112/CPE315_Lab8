#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "matmul2.h"

/* memory management, code density, Cache emulation - statistics generation */
/* Generated for CSC 315 Lab 9 */
#define AMAX 10 			            /* Maximum (square) array size */
#define CACHESIZE 2               /* Set to cache size (16 or 256) */
#define ASSOC 1                     /* Set to associativity option (1, 2, or 4) */
#define CACHESIM 1		            /* Set to 1 if simulating Cache */

/* Statically define the arrays a, b, and mult, where mult will become the cross product of a and b, i.e., a x b.*/
static int a[AMAX][AMAX], b[AMAX][AMAX], mult[AMAX][AMAX];
static int hit = 0, miss = 0, read = 0, write = 0;
static Cache array[ASSOC][CACHESIZE];

int get_tag(int * mp) {
   int tag;

   if (CACHESIZE == 16) {
      tag = (uintptr_t) mp & 0xFFFFC0;
   } else {
      tag = (uintptr_t) mp & 0xFFFC00;
   }

   return tag;
}

int get_index(int * mp) {
   int index;

   if (CACHESIZE == 16) {
      index = (uintptr_t) mp & 0x00003C;
   } else {
      index = (uintptr_t) mp & 0x0003FC;
   }

   return index >> 2;
}

/* This function gets called with each "read" reference to memory */
/* They actually do nothing. WE ACTUALLY NEVER USE DATA FROM THE CACHE */
void mem_read(int *mp) {
   int tag = get_tag(mp);
   int index = get_index(mp);
   int random = rand() % ASSOC;
   int found = 0;
   int j;
   
   read += 1;
   
   for (j = 0; j < ASSOC; j += 1) {
      
      if (array[j][index].tag == tag) {
         found = 1;
         hit += 1;
      }
      
   }
   
   if (found == 0) {
      array[random][index].tag = tag;
      write += 1;
      miss += 1;
   }
}

/* This function gets called with each "write" reference to memory */
/* They actually do nothing. WE ACTUALLY NEVER USE DATA FROM THE CACHE */
void mem_write(int *mp)
{
   int tag = get_tag(mp);
   int index = get_index(mp);
   int random = rand() % ASSOC;

   write += 1;
   
   array[random][index].tag = tag;

}

void matmul() {

   int i, j, k;
   int *mp1, *mp2, *mp3;

   /* Initializing elements of matrix mult to 0.*/
   printf("Starting initialization.\n");
   for (i = 0; i < AMAX; i += 1) {  
      for (j = 0; j < AMAX; j += 1) {
         mult[i][j]=0;
      }
   }
   printf("\tFinished initialization.\n");


   /* Multiplying matrix a and b and storing in array mult. */
   printf("Starting multiplying.\n");
   for (i = 0; i < AMAX; i += 1) {
      for (j = 0; j < AMAX; j += 1) {
         for (k = 0; k < AMAX; k += 1) {
            #if CACHESIM		/* "Hooks" to measure memory references - enabled if CACHESIM  */
            mp1 = &mult[i][j];
   	      mp2 = &a[i][k];
	         mp3 = &b[k][j];
      
            mem_read(mp1);
            mem_read(mp2);
            mem_read(mp3);
	         mem_write(mp1);
            #endif

            mult[i][j]+=a[i][k]*b[k][j];
         }  
      }
   }
   printf("\tFinished multiplying.\n");
}

int main(int argc, char **argv)
{
   int i, j;

   printf("Cache Size: %d\nAssociativity: %d\nMatrix Size: %d\n\n", CACHESIZE, ASSOC, AMAX);

   for (i = 0; i < ASSOC; i += 1) {
      for (j = 0; j < CACHESIZE; j += 1) {
         array[i][j].tag = 0;
         array[i][j].data = 0;
      }
   }
   
   /* If column of first matrix in not equal to row of second matrix, asking user to enter the size of matrix again. */
   for (i = 0; i < AMAX; i += 1) {
      for (j = 0; j < AMAX; j += 1) {
         a[i][j] = i + j; /*build sample data*/
      }
   }

   /* Storing elements of second matrix. */
   for (i = 0; i < AMAX; i += 1) {
      for (j = 0; j < AMAX; j += 1) {
         b[i][j] = 10 + i + j;
      }
   }

   matmul();  	/* Invoke matrix multiply function */	

   /* Displaying the multiplication of two matrix. */
   printf("\nOutput Matrix:\n");
   for (i = 0; i < AMAX; i += 1) {
      for (j = 0; j < AMAX; j += 1) {
         printf("%d\t", mult[i][j]);

         if (j == AMAX - 1) {
            printf("\n\n");
         }
      }
   }

   printf("Read = %d, Write = %d\n", read, write);
   printf("Read/Write Ratio: %f\n", (double) read / (double) write);
   printf("Hits = %d, Misses = %d\n", hit, miss);
   printf("Hit Rate: %f\n", (double) (hit * 100) / (double) (hit + miss));
   
   return 0;
}















