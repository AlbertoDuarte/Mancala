#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdlib.h>

typedef struct no {
    int* tabuleiro;
    struct no* proximo[6];
} no;

int max(int a, int b) {
    if(a > b)
        return a;
    return b;
}

no* criarNo(int* tabuleiro) {
  int i;
    no* arvore = malloc(sizeof(no));
    arvore->tabuleiro = tabuleiro;
    for(i = 0; i < 6; i++) {
      arvore->proximo[i] = NULL;
    }
    return arvore;
}

int alturaArvore(no* arvore) {
  int i, maior = 0;

  if (arvore == NULL){
    return 0;
  }

  for(i = 0; i < 6; i++) {
    maior = max(alturaArvore(arvore->proximo[i]), maior);
  }
  return maior;
}

/*TODO: Otimizar aproveitamento da arvore*/
void freeArvore(no* arvore) {
  int i;

  if(arvore == NULL) {
    return;
  }
  for(i = 0; i < 6; i++) {
    if(arvore->proximo[i] != NULL){
      freeArvore(arvore->proximo[i]);
    }
  }
  if(arvore->tabuleiro != NULL) {
    free(arvore->tabuleiro);
  }
  free(arvore);
}

#endif
