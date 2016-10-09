#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <semaphore.h>


// definiçoes

#define REF 50 // número de refeiçoes servidas
#define FIL 5 // número de filósofos


// variáveis globais

int ref_comidas[FIL] = {0,0,0,0,0};
volatile int taca[2] = {0,1};
volatile int temTaca[2] = {-1,-1};
volatile int temGarfo[5] = {-1,-1,-1,-1,-1};


// cabeçalhos

void create_phils(pthread_t *phil, int *phil_id);
void f_thread (void *v);
void pensa(int phil_id);
void EsperaTaca(int phil_id);
void EsperaGarfo(int phil_id);

// Main

int main(){

  pthread_t phil[FIL];
  int i, phil_id[FIL];

  create_phils(phil,phil_id);
  
  for (i = 0; i < FIL; i++) 
    pthread_join(phil[i], NULL); 

  
  return 0;
}

// f_thread

void f_thread (void *v) {
  int t_id = *(int *) v;
  printf("Filósofo %d sentou-se a mesa. ʘ‿ʘ \n", t_id);

  pensa(t_id);
  EsperaTaca(t_id);
  
  return NULL;
}

// outras funçoes

void create_phils(pthread_t *phil, int *phil_id) {
  int i;
  for (i = 0; i < FIL; i++) {
    phil_id[i] = i;
    pthread_create(&phil[i], NULL, f_thread, (void*) &phil_id[i]);
  }
}


void pensa(int phil_id) {
  sleep(1);
}

void EsperaTaca(int phil_id) {
  while (1) {
    if ( phil_id == taca[0]%5) {
      printf("filosofo %d pegou a taça ಠ_ಠ \n", phil_id);
      temTaca[0] = phil_id;
      break;
    }
    else if ( phil_id == taca[1]%5) {
      printf("filosofo %d pegou a taça ಠ_ಠ \n", phil_id);
      temTaca[1] = phil_id;
      break;
    }
  }
}

void EsperaGarfo(int phil_id){
  
}
