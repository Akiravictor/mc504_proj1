/* Felipe Viglioni 106665
   Victor Akira    106976 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <semaphore.h>


// definiçoes

#define REF 15 // número de refeiçoes servidas
#define FIL 5 // número de filósofos


// variáveis globais

volatile int ref_comidas[FIL];               // contador de refeiçoes
volatile int taca[2];                            // define quem começa com as taças
volatile int temTaca[2] = {-1,-1};              // armazena quem tem as taças em dado momento    
volatile int temGarfo[FIL];   // idem sobre os garfos
volatile int terceiro = -1;                    // verifica que um 3o filosofo pegue um garfo  
volatile int ref = 0;                        // conta as refeiçoes feitas  
volatile char estado[FIL];

// cabeçalhos
void coolprint(char i);
void MostraEstado();
void imprime();                                          // imprime o numero de refeiçoes feitas
void create_phils(pthread_t *phil, int *phil_id);       // cria as threads
void f_thread (void *v);                               // 
void pensa(int phil_id);                              // filósofos pensam  
void EsperaTaca(int phil_id);                        // filosófos se degladeiam pela taça na mesa
void EsperaGarfo(int phil_id);                      // quem conseguir taça, briga pelos garfos
void PegaGarfo(int phil_id);                       // quem consegue, pega o garfo
void PegaOutroGarfo( int phil_id);                // pega o outro garfo na mesa 
void PegaUnicoGarfo(int phil_id);                // os fils sem copo se degladeiam pelo prox garfo
void Come(int phil_id);                         // quem pode come, quem nao pode fica na fome
void SoltaTalher(int phil_id);                 // os que comeram soltam o talher, o ciclo recomeça
                                              // com a vantagem de quem tiver pegado o unico garfo
                                             // na rodada anterior 

// Main

int main(){

  pthread_t phil[FIL];                         
  int r, i, phil_id[FIL];

  for(i=0;i<FIL;i++) {
    temGarfo[i]=-1;
    ref_comidas[i] = 0;
    estado[i] = 'z';
  }

  r = random()%FIL;
  taca[0] = r;
  taca[1] = (r+1)%FIL;
  
  create_phils(phil,phil_id);
  
  for (i = 0; i < FIL; i++) 
    pthread_join(phil[i], NULL); 

  return 0;
}

// f_thread

void f_thread (void *v) {
  int t_id = *(int *) v;
  //printf("Filósofo %d sentou-se a mesa. ʘ‿ʘ \n", t_id);
  
  while (1) {
    pensa(t_id);
    MostraEstado();
    EsperaTaca(t_id);
    MostraEstado();
    EsperaGarfo(t_id);
    MostraEstado();
    PegaUnicoGarfo(t_id);
    MostraEstado();
    PegaOutroGarfo(t_id);
    MostraEstado();
    Come(t_id);
    MostraEstado();
    SoltaTalher(t_id);
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
  estado[phil_id]='g';
  puts("");
  sleep(1);
}

void EsperaTaca(int phil_id) {
  estado[phil_id]='a';
  while (1) {

    if ( phil_id == taca[0]%FIL) {
      //printf("filosofo %d pegou a taça \t\t ಠ_ಠ φ\n", phil_id);
      temTaca[0] = phil_id;
      break;
    }
    else if ( phil_id == taca[1]%FIL) {
      //printf("filosofo %d pegou a taça \t\t ಠ_ಠ φ\n", phil_id);
      temTaca[1] = phil_id;
      break;
    }
  }
  estado[phil_id]='b';
}

void EsperaGarfo(int phil_id){
  
  while(1) {
 
    if ( phil_id == temTaca[0] || phil_id == temTaca[1]) {
      PegaGarfo(phil_id);
      break;
    }
  }
  estado[phil_id]='c';
}

void PegaGarfo (int phil_id) {
  int j, i = 0;

  for (j=0;j<FIL;j++)
    if ( phil_id == temGarfo[j])
      return;
  
  while ( temGarfo[i] != -1 || i == 4)
    i++;
  if (temGarfo[i] == -1) {
    temGarfo[i] = phil_id;
    //printf("filosofo %d pegou um garfo \t\t ಠ_ಠ \n", phil_id);
  }
  else
    exit(1);
}

void PegaOutroGarfo( int phil_id) {

  int i = 0;
  while ( temGarfo[i] != -1 && i <= 4)
    i++;
  if (temGarfo[i] == -1) {
    temGarfo[i] = phil_id;
    //printf("filosofo %d pegou outro garfo \t\t ಠ_ಠ \n", phil_id);
  }
  else
    exit(1);
  estado[phil_id]='d';
}

void PegaUnicoGarfo(int phil_id) {
  estado[phil_id]='e';
  if (phil_id%2 == 0 && terceiro == -1) {
    int counter = 0, i, nowner=2011*(phil_id+FIL+1)%FIL;
    for(i=0;i<FIL;i++) {
      if(temGarfo[i] != -1) {
	counter++;
      }
    }
  
    for(i=0;i<FIL;i++) {
      if(nowner == temGarfo[i]) {
	nowner=(nowner+1)%FIL;
	i=0;
      }
    }

    if (counter < FIL) { 
      PegaGarfo(nowner);
      terceiro = nowner;
    }
  }

}

void Come(int phil_id) {
  int i, counter = 0;
  for (i=0;i<FIL;i++)
    if (phil_id == temGarfo[i])
      counter++;
  if (counter == 2) {
    //printf("filósofo %d comeu \t\t ( ͡° ͜ʖ ͡°) \n", phil_id);
    estado[phil_id]='f';
    ref_comidas[phil_id]++;
    ref++;
    if (ref == REF) {
      imprime();
      exit(0);
    }
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
        taca[1]++;
  }

  for(i=0;i<FIL;i++) {
    if (temGarfo[i] == phil_id) {
      temGarfo[i] = -1;
      counter++;
    }
  }

  taca[0] = terceiro;
  terceiro=-1;

  if (counter != 2)
    exit(1);

}

void imprime() {
  int i;
  puts("");
  for (i=0; i<FIL;i++)
    printf("filósofo %d comeu %d vezes\n",i,ref_comidas[i]);
}


void MostraEstado() {
  int i;
  puts("");
  puts("");
  for(i=0;i<FIL;i++) {
    printf("F%d\t\t|",i);
  }
  puts("");
  for(i=0;i<FIL;i++) {
    coolprint(estado[i]);
  }

}

void coolprint(char i) {
  switch(i){
  case 'a': {
    printf("ಠ╭╮ಠ\t\t|");
    break;
  }
  case 'b': {
    printf("ಠ_ಠ Y\t\t|");
    break;
  }
  case 'c': {
    printf("ಠ_ಠ ψY\t\t|");
    break;
  }
  case 'd': {
    printf("ψ ಠ_ಠ ψY\t|");
    break;
  }
  case 'e': {
    printf("ಠ_ಠ ψ\t\t|");
    break;
  }
  case 'f': {
    printf("( ͡° ͜ʖ ͡°)\t|");
    break;
  }
  case 'g': {
    printf("(˘⌣˘)\t\t|");
    break;
  }
  
  }
}
