#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 32 

struct thread {
   pthread_t td;
   int a, b;
};
int array[N];
struct thread tds[N];

void genValues(int array[], int length); /* Initializes  a[] */
void prnValues(int array[], int length); /* Prints a[] */
void sumArray(int array[], int length); /* Displays the sum of the array a[] */
void * addArray(void * td); 

void main() {
   genValues(array, N);    /* Initialize a[] with random values */
   printf("Initial values:\n");
   prnValues(array, N);    /* Display the values */
   sumArray(array, N);    /* Display the sum value */

   for(int size = 1; size < N; size = 2 * size) {
      printf("*** adding a[i+%d] to a[i]\n",size);
      for(int i = 0; i + size < N; i += 2 * size) {
         tds[i].a = i;
         tds[i].b = i + size;
         if(pthread_create(&(tds[i].td), NULL, addArray, (void *)(&tds[i])) != 0) {
            perror("Error creating threads.\n");
            exit(1);
         }
      }
      for(int i = 0; i + size < N; i += 2 * size) {
         pthread_join(tds[i].td, NULL);
      }
   }

   printf("\nOutput:\n");
   prnValues(array, N); /* Display the sorted values */

}

void * addArray(void * td) {
   struct thread *t = (struct thread *)td;
   usleep(250000);
   array[t->a] += array[t->b];
}

/* Initializes array a[] random values. */
void genValues(int array[], int length) {
   int k = 2*length;
   int current = 0;
   for (int i=0; i<length; i++) {
      current = (current*73 + 19)%k;
      array[i] = current;
   }
}

/* Prints the values in the array a[] */
void prnValues(int array[], int length) {
   for (int i=0; i<length; i++) {
      printf(" %d ", array[i]);
      if ((i+1)%10 == 0) printf("\n");
   }
   printf("\n");
}

/* Sum the array */
void sumArray(int array[], int length) {
   int sum = 0;
   for (int i=0; i<length; i++) {
      sum += array[i];
   }
   printf("Sum of the array =%d\n", sum);
}

