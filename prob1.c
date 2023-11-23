/*
 * EE 468 Midterm Exam 1
 *
 * Do not change the child.
 *
 * Modify the program so that children 0, 1, 2 are created then terminated.
 * Then children 3, 4, 5 are created then terminated.  Then
 * children 6, 7, 8 are created then terminated.  Note that
 * a child goes to sleep for one second.  So your program should run
 * for approximately 3 seconds.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main(void) {
   pid_t childPID;
   int k = 0;

   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
         childPID = fork();
         k++;

         if (childPID == 0) {    // Start of Child
            printf("Child %d: Created\n", k);
            sleep(1);
            printf("Child %d: Terminated\n", k);
            return 0;
         }                       // End of Child
                                 //
         else if (childPID < 0) {
            printf("\n Creating child %d failed, quitting!\n", k);
            return 1;
         } 
      }

      for (int j = 0; j < 3; j++) {
         wait(NULL);
      }
   }

   return 0;
}
