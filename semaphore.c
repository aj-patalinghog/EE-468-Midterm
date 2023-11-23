#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> /* For threads, mutex, cond variables */ 
#include <unistd.h> /* for sleep and usleep */


struct th_params {
   int id;
   int start_delay;
   int occ_delay;
   pthread_t thread;
   pthread_attr_t attr;
};

#define NTHREADS 20

pthread_mutex_t user_lock; /* Lock for the web page */
pthread_cond_t user_wait;
int count = 3;

void sleep100ms(int n);

void start_access(struct th_params * p) {
   pthread_mutex_lock(&user_lock);
   while(count <= 0) {
      pthread_cond_wait(&user_wait, &user_lock);
   }
   count--;
   pthread_mutex_unlock(&user_lock);
}

void done_access(struct th_params *p) {
   pthread_mutex_lock(&user_lock);
   pthread_cond_signal(&user_wait);
   count++;
   pthread_mutex_unlock(&user_lock);
}

void *user_thread(void *p)
{
   struct th_params *params  = (struct th_params *) p;

   sleep100ms(params->start_delay);
   printf("User %2d Arrives\n", params->id);
   start_access(params);
   /*  Start of critical section */
   printf("      >>> User %2d Enters\n", params->id);
   sleep100ms(params->occ_delay);
   printf("      <<< User %2d Exits\n", params->id);
   /*  End of critical section */
   done_access(params);

   pthread_exit((void*) 0);
}

void user_go(int id, int start_delay, int occ_delay, struct th_params  params[])
{
   params[id].id = id;
   params[id].start_delay = start_delay;
   params[id].occ_delay = occ_delay;
   pthread_create(&(params->thread), NULL, user_thread, (void *) &params[id]);
}

int main()
{
   pthread_mutex_init(&user_lock, NULL);
   struct th_params th[NTHREADS];
   user_go(0,1,8,th);
   user_go(1,2,8,th);
   user_go(2,3,8,th);
   user_go(3,6,8,th);
   user_go(4,7,8,th);
   user_go(5,8,8,th);
   user_go(6,19,6,th);
   user_go(7,20,6,th);
   user_go(8,21,6,th);
   user_go(9,22,6,th);

   sleep(5); /* Delay until all threads are finished */

   pthread_mutex_destroy(&user_lock);
}


void sleep100ms(int n) /* Slee for n quarter-seconds */
{
   usleep(100000*n);
}


