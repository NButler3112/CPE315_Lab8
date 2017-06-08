/* memory management, code density, Cache emulation - statistics generation */
/* Generated for CSC 315 Lab 5 */
#define AMAX 10			            /* Maximum (square) array size */
#define CACHESIM 1		            /* Set to 1 if simulating Cache */
#define CACHESIZE 16                /* Set to cache size (16 or 256) */
#define ASSOC 1                     /* Set to associativity option (1, 2, or 4) */
#define BOLEN 2                     /* Set to log2(sizeof(int)) = 2 */
#define INDEXLEN 6                  /* Set to log2(CACHESIZE) = 6 or 10 */
#define TAGLEN 56                   /* Set to server size (64) - BOLEN - INDEXLEN = 56 or 52 */
#define BOMASK 0x0000000000000003   /* Set to get the byte offset out of the address */
#define INDEXMASK 0x00000000000000FC/* Set to get the index out of the address */
#define TAGMASK 0xFFFFFFFFFFFFFF00  /* Set to get the tag out of the address */
#include <stdio.h>
#include <math.h>
#include "matmul2.h"

/* Statically define the arrays a, b, and mult, where mult will become the cross product of a and b, i.e., a x b.*/
static int a[AMAX][AMAX], b[AMAX][AMAX], mult[AMAX][AMAX];
static double hit = 0, miss = 0, read = 0, write = 0;
static Cache array[CACHESIZE];

/* This function gets called with each "read" reference to memory */
/* They actually do nothing. WE ACTUALLY NEVER USE DATA FROM THE CACHE */
void mem_read(int *mp)
{
   int found  = 0, i, j;
   read++;
   for (i = 0; i < TAGLEN / ASSOC; i += ASSOC & miss++)
   for (j = 0; j < ASSOC; j += 1)
   {
      if (array[j].tag == tag)
      {
         hit++;
         break;
      }
   }
   /* printf("Memory read from location %p\n", mp);  */
}

/* This function gets called with each "write" reference to memory */
/* They actually do nothing. WE ACTUALLY NEVER USE DATA FROM THE CACHE */
void mem_write(int *mp)
{
   write++;
   /* printf("Memory write to location %p\n", mp); */
}

void matmul(r1, c1, c2)
{
   int i, j, k;
   int *mp1, *mp2, *mp3;

   /* Initializing elements of matrix mult to 0.*/
   for(i=0; i<r1; ++i)
   for(j=0; j<c2; ++j)
   {
      mult[i][j]=0;
   }

   /* Multiplying matrix a and b and storing in array mult. */
   for(i=0; i<r1; ++i)
   for(j=0; j<c2; ++j)
   for(k=0; k<c1; ++k)
   {
      #if CACHESIM		/* "Hooks" to measure memory references - enabled if CACHESIM  */

      mp1 = &mult[i][j];
   	mp2 = &a[i][k];
	   mp3 = &b[k][j];
      fprintf(stderr, "mp1: %p, mp2: %p, mp3: %p\n", (void *)mp1, (void *)mp2, (void *)mp3);
	   mem_read(mp1);
 	   mem_read(mp2);
	   mem_read(mp3);
	   mem_write(mp1);
      #endif

      mult[i][j]+=a[i][k]*b[k][j];
   }
}

int main(int argc, char **argv)
{
   int r1, c1, r2, c2, i, j/*, k*/;
   int *mp1/*, *mp2, *mp3*/;

   printf("Size of int is: %lu\n\n", sizeof(int));

   printf("Size of pointer is: %lu\n\n", sizeof(mp1));

   printf("Enter rows and column for first matrix: ");
   scanf("%d%d", &r1, &c1);
   printf("Enter rows and column for second matrix: ");
   scanf("%d%d",&r2, &c2);

   /* If column of first matrix in not equal to row of second matrix, asking user to enter the size of matrix again. */
   while (c1 != r2)
   {
      printf("Error! column of first matrix not equal to row of second.\n");
      printf("Enter rows and column for first matrix: ");
      scanf("%d%d", &r1, &c1);
      printf("Enter rows and column for second matrix: ");
      scanf("%d%d",&r2, &c2);
   }

   /* Storing elements of first matrix. */
   printf("\nEnter elements of matrix 1:\n");
   for(i=0; i<r1; ++i)
   for(j=0; j<c1; ++j)
   {
      /*printf("Enter elements a%d%d: ",i+1,j+1);
      scanf("%d",&a[i][j]);*/

	   a[i][j] = i+j; /*build sample data*/
   }

   /* Storing elements of second matrix. */
   printf("\nEnter elements of matrix 2:\n");
   for(i=0; i<r2; ++i)
   for(j=0; j<c2; ++j)
   {
      /*printf("Enter elements b%d%d: ",i+1,j+1);
      scanf("%d",&b[i][j]);*/

      b[i][j] = 10 + i + j;
   }

   matmul(r1, c1, c2);  	/* Invoke matrix multiply function */	

   /* Displaying the multiplication of two matrix. */
   printf("\nOutput Matrix:\n");
   for(i=0; i<r1; ++i)
   for(j=0; j<c2; ++j)
   {
      printf("%d  ",mult[i][j]);
      if(j == c2-1)
         printf("\n\n");
   }

   fprintf(stderr, "Read/Write Ratio: %f\n", read / write);
   fprintf(stderr, "Hit Rate: %f\n", (hit * 100) / (hit + miss));
   return 0;
}
