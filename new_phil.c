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
volatile int terceiro = 0;

// cabeçalhos

void imprime() {
  int i;
  printf("[");
  for (i=0; i<5;i++)
    printf("%d, ",temGarfo[i]);
  puts("]");
}


void create_phils(pthread_t *phil, int *phil_id);
void f_thread (void *v);
void pensa(int phil_id);
void EsperaTaca(int phil_id);
void EsperaGarfo(int phil_id);
void PegaGarfo(int phil_id);
void PegaOutroGarfo( int phil_id);
void PegaUnicoGarfo();

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
  EsperaGarfo(t_id);
  PegaOutroGarfo(t_id);
  imprime();
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
  while(1) {
    if ( phil_id == temTaca[0] || phil_id == temTaca[1]) {
      PegaGarfo(phil_id);
      break;
    }
  } 
}

void PegaGarfo (int phil_id) {
  int i = 0;
  while ( temGarfo[i] != -1 || i == 4)
    i++;
  if (temGarfo[i] == -1) {
    temGarfo[i] = phil_id;
    printf("filosofo %d pegou um garfo ಠ_ಠ \n", phil_id);
  }
  else
    exit(1);
}

void PegaOutroGarfo( int phil_id) {
   

  while (terceiro == 0)
    PegaUnicoGarfo(); // 3o filosofo pega um garfo

  int i = 0;
  while ( temGarfo[i] != -1 || i == 4)
    i++;
  if (temGarfo[i] == -1) {
    temGarfo[i] = phil_id;
    printf("filosofo %d pegou outro garfo ಠ_ಠ \n", phil_id);
  }
  else
    exit(1);


}

void PegaUnicoGarfo() {
  int counter = 0, i, owner[2]={-1,-2},j=0;
  
  for (i=0;i<5;i++)
    if ( temGarfo[i] != -1) {
      counter++;
      if (owner[j] != owner[(j+1)%2]) {
	owner[j] = temGarfo[i];
	j++;
      }
    }

  if (counter == 2) {
    PegaGarfo((13*(owner[0]+owner[1]))%5);
    terceiro = 1;
  }

}

