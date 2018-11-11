#include <stdio.h>
#include <stdlib.h>
#include "arvorebin.h"
#define TAMANHO 6

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

void limparTela() {
    system(CLEAR);
}

int* criarTabuleiro() {
  /* Cria tabuleiro inicializando as casa em 4 e 0 pontuacao */
  int i, tam_total = (TAMANHO*2)+2;
  int* tabuleiro = malloc(sizeof(int)*tam_total);

  for(i = 0; i < tam_total; i++) {
    tabuleiro[i] = 4;
  }
  tabuleiro[TAMANHO] = 0;
  tabuleiro[(TAMANHO*2)+1] = 0;
  return tabuleiro;
}

void printTabuleiro(int* tabuleiro) {
  int i, tam_total = (TAMANHO*2)+2;

  limparTela();

  printf("%3c", ' ');
  for(i = tam_total-2; i > TAMANHO; i--) {
    printf("%d ", tabuleiro[i]);
  }
  printf("\n[%d]%*c[%d]\n", tabuleiro[tam_total-1], TAMANHO*2, ' ', tabuleiro[TAMANHO]);
  printf("%3c", ' ');
  for(i = 0; i < TAMANHO; i++) {
    printf("%d ", tabuleiro[i]);
  }
  printf("\n");
}

int semMovimento(int* tabuleiro, int vez) {
  /* Retorna 1 se o jogador 'vez' nao tiver mais movimentos */
  int idxini, idxfim;

  idxini = (vez-1)*(TAMANHO+1);
  idxfim = idxini+5;
  for(; idxini <= idxfim; idxini++) {
    if(tabuleiro[idxini] != 0) {
      return 0;
    }
  }
  return 1;
}

int fimJogo(int* tabuleiro) {
  /* Retorna o numero do jogador que venceu, 0 se nao tiver acabado */
  int i, jog1, jog2;
  jog1 = semMovimento(tabuleiro, 1);
  jog2 = semMovimento(tabuleiro, 2);

  if(jog1 == 0 && jog2 == 0) {
    return 0;
  }
  else if(jog1) {
    for(i = TAMANHO+1; i <= TAMANHO*2; i++) {
      tabuleiro[TAMANHO] += tabuleiro[i];
      tabuleiro[i] = 0;
    }
  }
  else {
    for(i = 0; i < TAMANHO; i++) {
      tabuleiro[TAMANHO] += tabuleiro[i];
      tabuleiro[i] = 0;
    }
  }
  /* retorna quem venceu */
  if(tabuleiro[TAMANHO] > tabuleiro[(TAMANHO*2)+1]) {
    return 1;
  }
  else if(tabuleiro[TAMANHO] < tabuleiro[(TAMANHO*2)+1]) {
    return 2;
  }
  else {
    return -1;
  }
}

void capturar(int* tabuleiro, int index, int vez) {
  /* Captura a casa do lado oposto */
  int somaidx, tmp;

  somaidx = index % 7;
  somaidx = (TAMANHO*2) - (somaidx*2);
  if(tabuleiro[index+somaidx] != 0) {
    tabuleiro[index] = 0;
    tmp = tabuleiro[(index+somaidx)%((TAMANHO*2)+2)];
    tabuleiro[(index+somaidx)%((TAMANHO*2)+2)] = 0;
    if(vez == 1) {
      tabuleiro[TAMANHO] += tmp+1;
    }
    else {
      tabuleiro[(TAMANHO*2)+1] += tmp+1;
    }
  }
}

int jogar(int* tabuleiro, int posicao, int vez) {
  /* 1 => jogador 1, 2 => jogador 2*/
  int quant, index, tmp;

  index = posicao-1;

  if(vez != 1) {
    index = (TAMANHO*2)-index;
    posicao = TAMANHO+1-posicao;
  }

  quant = tabuleiro[index];
  tmp = quant;
  if(quant == 0)
    return 0;
  tabuleiro[index] = 0;
  while(quant--) {
    index++;
    tabuleiro[(index%14)]++;
  }

  /* Verifica condicoes de captura */
  if(posicao+tmp <= 6 && tabuleiro[index] == 1) {
    capturar(tabuleiro, index, vez);
  }

  return 1;
}

int input(int* tabuleiro, int vez) {
  /* Pede input para o jogador */
  int posicao;

  printTabuleiro(tabuleiro);
  printf("Vez do jogador %d\nEscolha uma casa:", vez);
  scanf("%d", &posicao);
  while(posicao > TAMANHO && posicao < 1) {
    printTabuleiro(tabuleiro);
    printf("Vez do jogador %d\nCasa invalida\nEscolha uma casa:", vez);
    scanf("%d", &posicao);
  }
  return (posicao);
}

int iniciarJogo(int* tabuleiro) {
  /* Loop do jogo */
  int vez = 1; /* 1 => jogador 1, 2 => jogador 2*/
  int posicao, continuar = 1, fim = 1;
  while(continuar) {
    posicao = input(tabuleiro, vez);
    vez += jogar(tabuleiro, posicao, vez);
    if(vez == 3)
      vez = 1;

    fim = fimJogo(tabuleiro);
    if(fim) {
      continuar = 0;
    }
  }

  /* Printa quem venceu */
  printTabuleiro(tabuleiro);
  if(fim == -1) {
    printf("EMPATE!!!\n");
  }
  else {
    printf("Jogador %d ganhou\n", fim);
  }
}

int main() {
  int* tabuleiro;

  tabuleiro = criarTabuleiro();
  iniciarJogo(tabuleiro);

  return 0;
}
