 #include <pthread.h>
 #include <stdio.h>
 #include <unistd.h>
 
 static int global_value = 0;
 static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
 static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
 
 void* threadFunc1(void* arg)
 {
     pthread_mutex_lock(&lock);
     while (global_value != 3) {
         pthread_cond_wait(&cond, &lock);
     }
     printf("Half way there!\n");
     pthread_mutex_unlock(&lock);
     return NULL; 
 }
 
 void* threadFunc2(void* arg)
 {
     pthread_mutex_lock(&lock);
     while (global_value != 0) {
         pthread_cond_wait(&cond, &lock);
     }
     printf("Liftoff!\n");
     pthread_mutex_unlock(&lock);
     return NULL;
 }
 
 int main(void)
 {
     pthread_t tid1, tid2;
     global_value = 6;
 
     pthread_create(&tid1, NULL, threadFunc1, NULL);
     pthread_create(&tid2, NULL, threadFunc2, NULL);
 
     while (1) {
         pthread_mutex_lock(&lock);
         printf("%d\n", global_value);
         if (global_value == 0) {
             pthread_cond_broadcast(&cond);
             pthread_mutex_unlock(&lock);
             break;
         }
         global_value--;
         pthread_cond_broadcast(&cond);
         pthread_mutex_unlock(&lock);
         sleep(1);
     }
 
     pthread_join(tid1, NULL);
     pthread_join(tid2, NULL);
     return 0;
 }
 

 /*
    If we call pthread_cond_signal() instead of pthread_cond_broadcast(),
    we might wake up only one thread. The other thread may never run, so
    its message may never print
 */
 
