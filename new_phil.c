#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <semaphore.h>
#include <string.h>


// definiçoes

#define REF 25 // número de refeiçoes servidas
#define FIL 5 // número de filósofos


// variáveis globais

int ref_comidas[FIL] = {0,0,0,0,0};
volatile int taca[2] = {0,1};
volatile int temTaca[2] = {-1,-1};
volatile int temGarfo[5] = {-1,-1,-1,-1,-1};
volatile int terceiro = 0;
volatile int ref = REF;
// cabeçalhos
void imprime();
void create_phils(pthread_t *phil, int *phil_id);
void f_thread (void *v);
void pensa(int phil_id);
void EsperaTaca(int phil_id);
void EsperaGarfo(int phil_id);
void PegaGarfo(int phil_id);
void PegaOutroGarfo( int phil_id);
void PegaUnicoGarfo(int phil_id);
void Come(int phil_id);
void SoltaTalher(int phil_id); 

// Main

int main(){

  pthread_t phil[FIL];
  int r, i, phil_id[FIL];

  r = random()%5;
  taca[0] = r;
  taca[1] = (r+1)%5;
  
  create_phils(phil,phil_id);
  
  for (i = 0; i < FIL; i++) 
    pthread_join(phil[i], NULL); 

  imprime();
  return 0;
}

// f_thread

void f_thread (void *v) {
  int t_id = *(int *) v;
  printf("Filósofo %d sentou-se a mesa. ʘ‿ʘ \n", t_id);

  while (ref > 0) {
    pensa(t_id);
    EsperaTaca(t_id);
    EsperaGarfo(t_id);
    PegaUnicoGarfo(t_id);
    PegaOutroGarfo(t_id);
    Come(t_id);
    SoltaTalher(t_id);
	Mesa();
  }
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
  int j, i = 0;

  for (j=0;j<5;j++)
    if ( phil_id == temGarfo[j])
      return;
  
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
  int i = 0;
  while ( temGarfo[i] != -1 && i <= 4)
    i++;
  printf("%d\n",i);
  if (temGarfo[i] == -1) {
    temGarfo[i] = phil_id;
    printf("filosofo %d pegou outro garfo ಠ_ಠ \n", phil_id);
  }
  else
    exit(1);
}

void PegaUnicoGarfo(int phil_id) {

  if (phil_id%2 == 0 && terceiro == 0) {
    int counter = 0, i, nowner=(phil_id+2)%5;
    for(i=0;i<5;i++) {
      if(temGarfo[i] != -1) {
	counter++;
      }
    }
  
    for(i=0;i<5;i++) {
      if(nowner == temGarfo[i]) {
	nowner=(nowner+1)%5;
	i=0;
      }
    }

    if (counter < 5) { 
      PegaGarfo(nowner);
      terceiro = 1;
    }
  }

}

void Come(int phil_id) {
  int i, counter = 0;
  for (i=0;i<5;i++)
    if (phil_id == temGarfo[i])
      counter++;
  if (counter == 2) {
    printf("filósofo %d comeu ( ͡° ͜ʖ ͡°) \n", phil_id);
    ref_comidas[phil_id]++;
    ref--;
  }
}

void SoltaTalher (int phil_id) {
  int i, counter=0;
  
  if (temTaca[0] == phil_id) {
    temTaca[0] = -1;
    taca[0]++;
  }
  else if (temTaca[1] == phil_id) {
    temTaca[1] = -1;
    taca[1];
  }

  for(i=0;i<5;i++) {
    if (temGarfo[i] == phil_id) {
      temGarfo[i] = -1;
      counter++;
    }
  }

  
  if (counter != 2)
    exit(1);
}

void imprime() {
  int i;
  for (i=0; i<5;i++)
    printf("filósofo %d comeu %d vezes\n",i,ref_comidas[i]);
}

void Mesa(){
	char phil0[100];
	char phil1[100];
	char phil2[100];
	char phil3[100];
	char phil4[100];

	char eating[10] = '( ͡° ͜ʖ ͡°)';
	char taking[10] = '(ಠ_ಠ)';

	
}