#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "arvorebin.h"
#define TAMANHO 6
#define ALTURA_ARVORE 10

#ifndef _WIN32
    int kbhit() {
        /* Retorna 1 caso alguma tecla do teclado esteja sendo pressionada
        * Retorna 0 caso nao esteja */
        struct termios oldt, newt;
        int ch, oldf;
        tcgetattr(STDIN_FILENO,&oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        if(ch != EOF) {
            ungetc(ch,stdin);
            return 1;
        }
        return 0;
    }
    int getch(void) {
        /* Retorna tecla do teclado pressionada */
        int ch;
        struct termios oldt;
        struct termios newt;
        tcgetattr(STDIN_FILENO,&oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#else
    #include <conio.h>
#endif

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

int* teste;

int* copiarTab(int* original) {
  int i, tam_total = (TAMANHO*2)+2;
  int* vetor = malloc(tam_total * sizeof(original));

  for(i = 0; i < tam_total; i++) {
    vetor[i] = original[i];
  }
  return vetor;
}

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
  tabuleiro[tam_total-1] = 0;
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
  int idxini = 0, idxfim;

  if(vez != 1) {
    idxini = TAMANHO+1;
  }
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
      tabuleiro[TAMANHO*2+1] += tabuleiro[i];
      tabuleiro[i] = 0;
    }
  }
  else {
    for(i = 0; i < TAMANHO; i++) {
      tabuleiro[TAMANHO] += tabuleiro[i];
      tabuleiro[i] = 0;
    }
  }
  printTabuleiro(tabuleiro);
  /* retorna quem venceu */
  if(tabuleiro[TAMANHO] > tabuleiro[(TAMANHO*2)+1]) {
    return 1;
  }
  else if(tabuleiro[TAMANHO] < tabuleiro[(TAMANHO*2)+1]) {
    return -1;
  }
  else {
    return 2;
  }
}

void capturar(int* tabuleiro, int index, int vez) {
  /* Captura a casa do lado oposto */
  int somaidx, tmp;

  somaidx = index % 7;
  somaidx = (TAMANHO*2) - (somaidx*2);
  if(tabuleiro[(index+somaidx)%14] != 0) {
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
  int quant, index, tmp, kahala = TAMANHO*2+1;

  if(posicao < 1 || posicao > TAMANHO) {
    return 0;
  }


  index = posicao-1;

  if(vez != 1) {
    index = (TAMANHO*2)-index;
    posicao = TAMANHO+1-posicao;
    kahala = TAMANHO;
  }


  quant = tabuleiro[index];
  tmp = quant;
  if(quant == 0)
    return 0;
  tabuleiro[index] = 0;
  while(quant--) {
    index++;
    index %= 14;
    if((index) == kahala) {
      index++;
    }
    tabuleiro[index%14] += 1;
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

  printf("Vez do jogador %d\nEscolha uma casa:", vez);
  scanf("%d", &posicao);
  while(posicao > TAMANHO && posicao < 1) {
    printTabuleiro(tabuleiro);
    printf("Vez do jogador %d\nCasa invalida\nEscolha uma casa:", vez);
    scanf("%d", &posicao);
  }
  return (posicao);
}

int avaliarFinal(int* tabuleiro) {
  int i, res = 0;

  for(i = 0; i <= 6; i++) {
    res -= tabuleiro[i];
  }
  for(i = 7; i<= 13; i++) {
    res += tabuleiro[i];
  }

  return res;
}

int avaliarTabuleiro(int* tabuleiro) {
  int i, res = 0;

  for(i = 0; i < 6; i++) {
    if(tabuleiro[i]+i==6) {
      res -= tabuleiro[i];
    }
    if(tabuleiro[i]+i>6) {
      res+=(tabuleiro[i]+i-7);
      if(tabuleiro[i]+i-12 > 0) {
        res+=(tabuleiro[i]+i-12);
      }

    }
  }
  for(i = 7; i< 13; i++) {
    if(tabuleiro[i]+i==13) {
      res += tabuleiro[i];
    }
    if(tabuleiro[i]+i>13) {
      res-=(tabuleiro[i]+i-14);
      if(tabuleiro[i]+i-19 > 0) {
        res-=(tabuleiro[i]+i-19);
      }
    }
  }

  res-=tabuleiro[6];
  res+=tabuleiro[13];

  return res;
}
/*
int avaliarTabuleiro(int* tabuleiro) {
  int res = 0;
  res += (tabuleiro[13] - teste[13]);
  res -= (tabuleiro[6] - teste[6]);
  return res;
}*/

int decidir(no* arvore, int vez, int altura) {
  int* tabtmp;
  int j, i, minmax[TAMANHO], tmp, maior = 1;
  no* subarvore;

  if((semMovimento(arvore->tabuleiro, 1) || semMovimento(arvore->tabuleiro, -1)) ) {
    return avaliarFinal(arvore->tabuleiro);
  }
  else if(altura == ALTURA_ARVORE) {
    return avaliarTabuleiro(arvore->tabuleiro);
  }

  j = 0;
  for(i = 0; i < 6; i++) {
    tabtmp = copiarTab(arvore->tabuleiro);
    tmp = jogar(tabtmp, i+1, vez*-1);
    if(tmp) {
      subarvore = criarNo(tabtmp);
      arvore->proximo[i] = subarvore;
      minmax[j] = decidir(arvore->proximo[i], vez*-1, altura+1);
      j++;
    }
    else {
      free(tabtmp);
      arvore->proximo[i] = NULL;
    }
  }

  if(vez == 1) {
    maior = -1;
  }

  tmp = minmax[0];
  for(i = 1; i < j; i++) {
    if(maior*minmax[i] > maior*tmp) {
      tmp = minmax[i];
    }
  }
  return tmp;
}

int jogarIA(no* arvore) {
  int i, *tabtmp, tmp, tmp2;
  int valor=(-TAMANHO*8), posicaojogar=0;
  no *subarvore;

  /*copia = copiarTab(tabuleiro);
  arvore = criarNo(copia);*/

  for(i = 0; i < 6; i++) {
    if(arvore->proximo[i] == NULL) {
      tabtmp = copiarTab(arvore->tabuleiro);
      tmp = jogar(tabtmp, i+1, -1);

      if(tmp) {
        subarvore = criarNo(tabtmp);
        arvore->proximo[i] = subarvore;
        tmp2 = decidir(subarvore, -1, 2);
      }
      else {
        free(tabtmp);
      }
    }
    else {
      tmp2 = decidir(arvore->proximo[i], -1, 2);
    }
    /* Verificar maior possibilidade de vencer */
    if(tmp2 > valor) {
      valor = tmp2;
      posicaojogar = i;
    }
  }
  return (posicaojogar+1);
}

void freeNivel(no* arvore, int excessao) {
  int i;

  for(i = 0; i < TAMANHO; i++) {
    if(i != excessao) {
      freeArvore(arvore->proximo[i]);
    }
  }
  free(arvore->tabuleiro);
  free(arvore);
}

int iniciarJogo(int* tabuleiro) {
  /* Loop do jogo */
  int vez = 1, jogadaP1 = -1; /* 1 => jogador 1, -1 => jogador 2*/
  int posicao, continuar = 1, fim = 1;
  int* copia = copiarTab(tabuleiro);
  no* arvore = criarNo(copia);
  no* tmpNo;

  jogarIA(arvore); /* Cria arvore de decisao */
  printTabuleiro(tabuleiro);
  while(continuar) {
    if(vez == 1) {
      posicao = input(tabuleiro, vez);
      jogadaP1 = posicao;
    }
    else {
      if(jogadaP1 >= 1 && jogadaP1 <= 6) {
        jogadaP1--;
        tmpNo = arvore;
        arvore = arvore->proximo[jogadaP1];
        freeNivel(tmpNo, jogadaP1);
      }
      else {
        printf("JogadaP1 fora de alcance\n");
        return -1;
      }
      jogadaP1 = -1;
      posicao = jogarIA(arvore);
      printf("IA jogou %d\nPressione algum botao\n", posicao);
      getch();
      getch();

      tmpNo = arvore;
      arvore = arvore->proximo[posicao-1];
      freeNivel(tmpNo, posicao-1);
    }

    if(jogar(tabuleiro, posicao, vez)) {
      vez *= -1;
      printTabuleiro(tabuleiro);

      fim = fimJogo(tabuleiro);
      if(fim) {
        continuar = 0;
      }
    }
  }

  /* Printa quem venceu */
  printTabuleiro(tabuleiro);
  if(fim == 2) {
    printf("EMPATE!!!\n");
  }
  else if(fim == 1) {
    printf("Jogador 1 ganhou!\n");
  }
  else {
    printf("IA ganhou!\n");
  }

  freeArvore(arvore);
  return 0;
}

int main() {
  int* tabuleiro;

  tabuleiro = criarTabuleiro();
  teste = tabuleiro;
  iniciarJogo(tabuleiro);
  free(tabuleiro);

  return 0;
}
