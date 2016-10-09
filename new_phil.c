#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <semaphore.h>

#define REF 50 // número de refeiçoes servidas
#define FIL 5 // número de filósofos

int ref_comidas[FIL] = {0,0,0,0,0};

void create_phils(pthread_t *phil, int *phil_id);
void f_thread (void *v);

int main(){

  pthread_t phil[FIL];
  int i, phil_id[FIL];

  create_phils(phil,phil_id);
  
  for (i = 0; i < FIL; i++) 
    pthread_join(phil[i], NULL); 

  
  return 0;
}

void create_phils(pthread_t *phil, int *phil_id) {
  int i;
  for (i = 0; i < FIL; i++) {
    phil_id[i] = i;
    pthread_create(&phil[i], NULL, f_thread, (void*) &phil_id[i]);
  }
}

void f_thread (void *v) {
  int t_id = *(int *) v;
  printf("Filósofo %d sentou-se a mesa.\n", t_id);
  return NULL;
}
