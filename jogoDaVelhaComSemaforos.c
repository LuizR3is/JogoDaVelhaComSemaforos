#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>

// Para compilar o programa é necessário abrir o terminal, acessar o diretório correspondente e
// compilar digitando o comando gcc -Wall jogoDaVelhaComSemaforos.c -o jdv -lpthread
// Em seguida ./jdv


//Definição do número de threads e do semáforo
#define NUM_THREADS 2
sem_t s ;

//Variável pra controlar quando o jogo encerra
bool acabou = false;

//Tabuleiro em que o jogo será jogado
char tabuleiro[] = {'_', '_', '_', '_', '_', '_', '_', '_', '_'};


//Função que preenche o tabuleiro
void desenha_tabuleiro(){
  printf("          |%c|%c|%c|                 |0|1|2|         \n", tabuleiro[0], tabuleiro[1], tabuleiro[2]);
  printf("          |%c|%c|%c|                 |3|4|5|         \n", tabuleiro[3], tabuleiro[4], tabuleiro[5]);
  printf("          |%c|%c|%c|                 |6|7|8|         \n\n", tabuleiro[6], tabuleiro[7], tabuleiro[8]);
  
}

//Checa o tabuleiro para verificar se houve uma sequência de vitória ou empate
bool checa_partida(long tid){
    if((tabuleiro[0] == 'X' && tabuleiro[1] == 'X' && tabuleiro[2] == 'X') ||
            (tabuleiro[3] == 'X' && tabuleiro[4] == 'X' && tabuleiro[5] == 'X') ||
            (tabuleiro[6] == 'X' && tabuleiro[7] == 'X' && tabuleiro[8] == 'X') ||
            (tabuleiro[0] == 'X' && tabuleiro[3] == 'X' && tabuleiro[6] == 'X') ||
            (tabuleiro[1] == 'X' && tabuleiro[4] == 'X' && tabuleiro[7] == 'X') ||
            (tabuleiro[2] == 'X' && tabuleiro[5] == 'X' && tabuleiro[8] == 'X') ||
            (tabuleiro[0] == 'X' && tabuleiro[4] == 'X' && tabuleiro[8] == 'X') ||
            (tabuleiro[2] == 'X' && tabuleiro[4] == 'X' && tabuleiro[6] == 'X')){
        printf("\n************FIM DO JOGO************\n");
        printf("         O JOGADOR %ld VENCEU!       \n", tid);
        printf("***********************************\n\n\n");
        return true;
    } else if((tabuleiro[0] == 'O' && tabuleiro[1] == 'O' && tabuleiro[2] == 'O') ||
            (tabuleiro[3] == 'O' && tabuleiro[4] == 'O' && tabuleiro[5] == 'O') ||
            (tabuleiro[6] == 'O' && tabuleiro[7] == 'O' && tabuleiro[8] == 'O') ||
            (tabuleiro[0] == 'O' && tabuleiro[3] == 'O' && tabuleiro[6] == 'O') ||
            (tabuleiro[1] == 'O' && tabuleiro[4] == 'O' && tabuleiro[7] == 'O') ||
            (tabuleiro[2] == 'O' && tabuleiro[5] == 'O' && tabuleiro[8] == 'O') ||
            (tabuleiro[0] == 'O' && tabuleiro[4] == 'O' && tabuleiro[8] == 'O') ||
            (tabuleiro[2] == 'O' && tabuleiro[4] == 'O' && tabuleiro[6] == 'O')){
        printf("\n*************FIM DO JOGO*************\n");
        printf("         O JOGADOR %ld VENCEU!         \n", tid);
        printf("*************************************\n\n\n");
        return true;
    } else if((tabuleiro[0] != '_') && (tabuleiro[1] != '_') && (tabuleiro[2] != '_') &&
             (tabuleiro[3] != '_') && (tabuleiro[4] != '_') && (tabuleiro[5] != '_') && 
             (tabuleiro[6] != '_') && (tabuleiro[7] != '_') && (tabuleiro[8] != '_')){
                 printf("\n************FIM DO JOGO************\n");
    		 printf("           SEM VENCEDORES          \n");
    		 printf("***********************************\n\n\n");
                 return true;      
    } else{
        return false;
    }
}

//Marca no tabuleiro o símbolo correspondente do jogador (thread)
void preenche_tabuleiro(int posicao, long tid){
  if(tid == 0){
    tabuleiro[posicao] = 'O';
  } 
  else if(tid == 1){
    tabuleiro[posicao] = 'X';
  }
}
  
//Função responsável por perguntar à thread em questão em qual posição deseja jogar
void realiza_jogada(long tid){

  if(acabou == false){

    int posicao_jogada;
    bool validado = false;
  
    //verifica se a jogada foi válida
    while(validado == false){
      char simbolo;
      
      if(tid == 0){
        simbolo = 'O';
      }else if(tid == 1){
        simbolo = 'X';
      }
      
      printf("Vez do jogador %ld (%c): Em qual posição você quer jogar? [0 a 8] ", tid, simbolo);
      scanf("%d", &posicao_jogada);
    
      if(tabuleiro[posicao_jogada] == '_' && posicao_jogada >= 0 && posicao_jogada < 9){
        validado = true;
      } else{
          printf("A posição informada já está preenchida ou está fora do escopo do tabuleiro! JOGUE NOVAMENTE!\n");
      }
    }
    preenche_tabuleiro(posicao_jogada, tid);
    desenha_tabuleiro();
    acabou = checa_partida(tid);
  } else{
      pthread_exit(NULL);
  }
   
}

//Função responsável pelo corpo da thread. Nela, os jogadores (threads) vão sinalizar para o semáforo quando forem
//entrar na área crítica (realização da jogada / acesso ao tabuleiro), para que não haja conflito
void *threadBody(void *id){
  long tid = (long) id ; // ID da thread

  while(acabou == false){
    sem_wait (&s);
    realiza_jogada(tid);
    sem_post (&s);
    sleep(1);
    
  }
  
  pthread_exit (NULL) ;
}

//Função que faz a introdução do jogo
void introducao(){
    printf("***************************************************\n");   
    printf("            Bem vindo ao Jogo da Velha!            \n");
    printf("***************************************************\n\n");
}


//Função que roda o jogo e que possui instruções comuns para a criação de threads
void jogar(){
  system("clear");
  
  introducao();
  desenha_tabuleiro();
  
  //Define as threads
  pthread_t thread [NUM_THREADS] ;
  pthread_attr_t attr ;
  long i, status ;

  // initialize semaphore to 1
  sem_init (&s, 0, 1) ;

  // define attribute for joinable threads
  pthread_attr_init (&attr) ;
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE) ;

  // create threads
  for(i=0; i<NUM_THREADS; i++)
  {
    status = pthread_create (&thread[i], &attr, threadBody, (void *) i) ;
    if (status)
    {
      perror ("pthread_create") ;
      exit (1) ;
    }
  }
   
  // wait all threads to finish   
  for (i=0; i<NUM_THREADS; i++)
  {
    status = pthread_join (thread[i], NULL) ;
    if (status)
    {
      perror ("pthread_join") ;
      exit (1) ;
    }
  }
  
  pthread_attr_destroy (&attr) ;
  pthread_exit (NULL) ;
  
  
}

//Função que mostra as regras do jogo
void regras(){
  system("clear");
  printf("REGRAS:\n\n");
  printf("- O jogo da velha deve ser jogado por 2 players (representados por threads).\n");
  printf("- Cada jogador (thread) terá acesso à realização de uma jogada \npor vez, controlado pelo semáforo.\n");
  printf("- Vence o jogo aquele player que conseguir completar uma sequência de \ntrês símbolos iguais, seja na horizontal, vertical ou diagonal.\n");
  printf("- Em caso de nenhum jogador completar uma sequência, o jogo terminará em empate.\n"); 
  printf("- A concorrência definirá qual jogador (thread) começará jogando, ou seja, \na thread que entrar primeiro na zona crítica, fará a primeira jogada.\n\n\n");
}

//Função que exibe o menu do jogo
void menu(){
  int opcao;
  int fim = 0;
  
  while(fim == 0){
   
    printf("\n\n***************************************************\n");   
    printf("****************** JOGO DA VELHA ******************\n");
    printf("***************************************************\n");
    printf("MENU:\n");
    printf("1- JOGAR.\n");
    printf("2- REGRAS.\n");
    printf("3- ENCERRAR\n");
    printf("***************************************************\n");
    printf("Digite o número da opção que deseja selecionar: ");
    scanf("%d", &opcao);
  
    switch(opcao){
      case 1:
      jogar();
      break;
    
      case 2:
      regras();
      break;
    
      case 3:
      fim = 1;
      break;
    
      default:
      printf("\n\n>>>>> VALOR INVÁLIDO! <<<<<\n\n");
    }
  
  }
  
}

int main (){
  menu();
  
  return 0;
}
