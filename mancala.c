#include <stdio.h>
#include <stdlib.h>
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

int movimentar(int* tabuleiro, int posicao) {
  int quant, tam_total = (TAMANHO*2)+2;

  if(posicao == TAMANHO || posicao == tam_total-1) {
    printf("posicao invalida!\n");
    return 0;
  }
  quant = tabuleiro[posicao];
  tabuleiro[posicao] = 0;
  while(quant--) {
    posicao++;
    tabuleiro[posicao%tam_total]++;
  }

  return 1;
}

int input(int* tabuleiro, int vez) {
  int menoridx, maioridx, posicao;
  if(vez == 1) {
    menoridx = 0;
  }
  else {
    menoridx = TAMANHO+1;
  }
  maioridx = menoridx + TAMANHO-1;

  printTabuleiro(tabuleiro);
  printf("Vez do jogador %d\nEscolha uma casa:", vez);
  scanf("%d", &posicao);
  posicao--;
  while(!(posicao >= menoridx && posicao <= maioridx && tabuleiro[posicao] != 0)) {
    printTabuleiro(tabuleiro);
    printf("Vez do jogador %d\nCasa invalida\nEscolha uma casa:", vez);
    scanf("%d", &posicao);
    posicao--;
  }
  return (posicao);
}

int jogar(int* tabuleiro) {
  int vez = 1; /* 1 => jogador 1, -1 => jogador 2*/
  int posicao, continuar = 1;
  while(continuar) {
    posicao = input(tabuleiro, vez);
    movimentar(tabuleiro, posicao);
    vez++;
    if(vez == 3)
      vez = 1;
  }
  return vez;
}

int main() {
  int* tabuleiro;

  tabuleiro = criarTabuleiro();
  jogar(tabuleiro);

  free(tabuleiro);
  return 0;
}
