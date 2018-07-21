#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Student: Maria Ylnah De Melo Santos - 201420006309
 *          Micael Santos Nascimento - 201420014955
 *          Milene Vieira Lopes - 201420029519
 * created in 01/03/2017 @Geany
 */

#define MAX 50

struct sinonimo {
    char palavraSin[50];
};

struct dicionario {
    char palavra[31];
    int qntSin;
    struct sinonimo sinonimos[MAX];
};

typedef struct arquivo TipoArquivo;
typedef struct folha *ArvoreAVL;
typedef struct folha TipoArvore;

struct folha {
    int altura;
    struct folha *direita;
    struct folha *esquerda;
    struct dicionario dados;
};

ArvoreAVL* criarArvore() {
    ArvoreAVL *raiz = (ArvoreAVL*) malloc(sizeof (ArvoreAVL));
    if (raiz != NULL) {
        *raiz = NULL;
    }
    return raiz;
}

int altura_no(TipoArvore *no) {//Retorna a altura da sub-árvore
    if (no == NULL) {
        return -1;
    }
    return no->altura;
}

int fatorBalanc(TipoArvore *no) {//Retorna o fator do nó
    return labs(altura_no(no->esquerda) - altura_no(no->direita));
}

int maior(int a, int b) {//verificar qual maior altura
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

void rotacaoLL(ArvoreAVL *raiz) {//Fazer a rotação para esquerda
    TipoArvore* no;
    no = (*raiz)->esquerda;
    (*raiz)->esquerda = no->direita;
    no->direita = *raiz;
    (*raiz)->altura = maior(altura_no((*raiz)->esquerda), altura_no((*raiz)->esquerda)) + 1;
    no->altura = maior(altura_no(no->esquerda), (*raiz)->altura) + 1;
    *raiz = no;
}

void rotacaoRR(ArvoreAVL *raiz) {//Fazer a rotação para direita
    TipoArvore* no;
    no = (*raiz)->direita;
    (*raiz)->direita = no->esquerda;
    no->esquerda = *raiz;
    (*raiz)->altura = maior(altura_no((*raiz)->esquerda), altura_no((*raiz)->direita)) + 1;
    no->altura = maior(altura_no(no->direita), (*raiz)->altura) + 1;
    *raiz = no;
}

void rotacaoLR(ArvoreAVL *raiz) { //rotação dupla esquerda, direita
    rotacaoRR(&(*raiz)->esquerda);
    rotacaoLL(raiz);
}

void rotacaoRL(ArvoreAVL *raiz) { //rotação dupla direita, esquerda
    rotacaoLL(&(*raiz)->direita);
    rotacaoRR(raiz);
}

int inserirArvore(ArvoreAVL *raiz, struct dicionario novaPalavra) {//Inserção na árvore
    int verificar;
    if (*raiz == NULL) {//Inserção na arvoreAVL vazia ou chegou em um nó folha
        TipoArvore* no = (TipoArvore*) malloc(sizeof (TipoArvore));
        if (no == NULL) {//Verificar se a alocação deu certo
            return 0;
        }
        //armazenar dados no nó
        no->dados = novaPalavra;
        no->altura = 0;
        no->esquerda = NULL;
        no->direita = NULL;
        *raiz = no;
        return 1;
    }
    TipoArvore *atual = *raiz;
    if (novaPalavra.palavra[0] < atual->dados.palavra[0]) {
        //Chama a função recursivamente para a esquerda até achar NULL
        verificar = inserirArvore(&(atual->esquerda), novaPalavra);
        if (verificar == 1) {
            if (fatorBalanc(atual) >= 2) {
                if (novaPalavra.palavra[0] < (*raiz)->esquerda->dados.palavra[0]) {
                    rotacaoLL(raiz);
                } else {
                    rotacaoLR(raiz);
                }
            }
        }
    } else {
        if (novaPalavra.palavra[0] >= atual->dados.palavra[0]) {
            //Chama a função recursivamente para a direita até achar NULL
            verificar = inserirArvore(&(atual->direita), novaPalavra);
            if (verificar == 1) {
                if (fatorBalanc(atual) >= 2) {
                    if ((*raiz)->direita->dados.palavra[0] < novaPalavra.palavra[0]) {
                        rotacaoRR(raiz);
                    } else {
                        rotacaoRL(raiz);
                    }
                }
            }
        }
    }
    atual->altura = maior(altura_no(atual->esquerda), altura_no(atual->direita)) + 1;
    return verificar;
}

TipoArvore* procuraMenor(TipoArvore* atual){ //procurar elemento menor a direita
    TipoArvore* no1 = atual;
    TipoArvore* no2 = atual->esquerda;
    while(no2 != NULL){
        no1 = no2;
        no1 = no2->esquerda;
    }
    return no1;
}

int removerPalavra(ArvoreAVL* raiz, char* palavra) {
    if (*raiz == NULL) {//Palavra não esta na arvore
        printf("Palavra não está no dicionario\n");
        return 0;
    }
    int res;
    if (palavra[0] < (*raiz)->dados.palavra[0]) { //Filho da esquerda
        if ((res = removerPalavra(&(*raiz)->esquerda, palavra)) == 1)
            if (fatorBalanc(*raiz) >= 2) {
                if (altura_no((*raiz)->direita->esquerda)
                        <= altura_no((*raiz)->direita->direita))
                    rotacaoRR(raiz);
                else
                    rotacaoRL(raiz);
            }
    }
    if((*raiz)->dados.palavra[0] < palavra[0] ){ //Filho da direita
        if ((res = removerPalavra(&(*raiz)->direita, palavra)) == 1)
            if (fatorBalanc(*raiz) >= 2) {
                if (altura_no((*raiz)->esquerda->direita)
                        <= altura_no((*raiz)->esquerda->esquerda))
                    rotacaoLL(raiz);
                else
                    rotacaoLR(raiz);
            }
    }

    if (strcmp((*raiz)->dados.palavra, palavra) == 0) { //Ao achar a palavra igual
        if (((*raiz)-> esquerda == NULL || (*raiz)->direita == NULL)) {
            TipoArvore* no = (*raiz);
            if ((*raiz)->esquerda != NULL) {
                *raiz = (*raiz)->esquerda;
            } else {
                *raiz = (*raiz)->direita;
            }
            free(no);
        } else {
            TipoArvore* aux = procuraMenor((*raiz)->direita);
            strcpy((*raiz)->dados.palavra, aux->dados.palavra);
            removerPalavra(&(*raiz)->direita, (*raiz)->dados.palavra);
            if (fatorBalanc(*raiz) >= 2) {
                if (altura_no((*raiz)->esquerda->direita)
                        <= altura_no((*raiz)->esquerda->esquerda))
                    rotacaoLL(raiz);
                else
                    rotacaoLR(raiz);
            }
        }
        return 1;
    }
    return res;
}

void buscarArvore(ArvoreAVL* raiz) {
    if (*raiz != NULL) {
        printf("%s: ", (*raiz)->dados.palavra);
        int i=0;
        printf("[");
        while(i < (*raiz)->dados.qntSin - 1){
            printf("%s, ", (*raiz)->dados.sinonimos[i].palavraSin);
            i++;
            }
        printf("%s]\n", (*raiz)->dados.sinonimos[i].palavraSin);
        
        buscarArvore(&((*raiz)->esquerda));
        buscarArvore(&((*raiz)->direita));
    }
}

void buscarPalavra(ArvoreAVL* raiz, char* palavra) {
    if(*raiz != NULL){
        if (strcmp((*raiz)->dados.palavra, palavra) == 0) {
            printf("Palavra encontrada:\n");
            printf(">%s: ", (*raiz)->dados.palavra);
            //imprimir sinonimos
            int i=0;
            printf("[");
            while(i < (*raiz)->dados.qntSin - 1){
                printf("%s, ", (*raiz)->dados.sinonimos[i].palavraSin);
                i++;
                }
            printf("%s]\n\n", (*raiz)->dados.sinonimos[i].palavraSin);
            
            return;
        } else {
            if (palavra[0] < (*raiz)->dados.palavra[0]) {//esquerda
                buscarPalavra(&((*raiz)->esquerda), palavra);
            } else {//direita
                buscarPalavra(&((*raiz)->direita), palavra);
            }
        }
    } else {
        printf("\n>A palavra '%s' não está no dicionário\n\n", palavra);
    }
}

void escreverNoArquivo(ArvoreAVL *raiz, FILE* output, char* qnt){
      if (*raiz != NULL) {
        fprintf(output, (*raiz)->dados.palavra);
        fprintf(output," ");
        sprintf(qnt, "%i", (*raiz)->dados.qntSin);
        fprintf(output, qnt); //quantidade de palavras
        fprintf(output," ");
        //escrever sinonimos
        int i=0;
        while(i < (*raiz)->dados.qntSin - 1){
            fprintf(output, (*raiz)->dados.sinonimos[i].palavraSin);
            fprintf(output," ");
            i++;
            }
        fprintf(output, (*raiz)->dados.sinonimos[i].palavraSin);
        fprintf(output,"\n");
        
        escreverNoArquivo(&((*raiz)->esquerda), output, qnt);
        escreverNoArquivo(&((*raiz)->direita), output, qnt);
    }
}

int main(int argc, char **argv) {
    char palavra[31];
    char qnt[10];
    int qntP = 0;
    int opcao;
    int rodando = 1;
    int i = 0, j = 0;

    FILE* input = fopen("entrada.txt", "r");
    
    ArvoreAVL *raiz = criarArvore();
    struct dicionario novaPalavra;
    
    if(input != NULL){
        //Capturar a quantidade de palavras
        fscanf(input, "%d", &qntP);

        //Ler as palavras e seus respectivos sinonimos
        while (i < qntP) {
            fscanf(input, "%s", novaPalavra.palavra);
            fscanf(input, "%d", &novaPalavra.qntSin);

            for (j = 0; j < novaPalavra.qntSin; j++) {
                fscanf(input, "%s", novaPalavra.sinonimos[j].palavraSin);
            }

            if (inserirArvore(raiz, novaPalavra) != 1) {
                printf("ERRO NAS ENTRADAS");
            }
            i++;
        }

        fclose(input);
    }
    
    FILE* output = fopen("entrada.txt", "w");
    while (rodando != 0) {//PROGRAMA PRINCIPAL
        printf("----------DICIONARIO----------\n");
        printf("1 - Ver Todas As Palavras\n");
        printf("2 - Buscar Palavra\n");
        printf("3 - Adicionar Palavra\n");
        printf("4 - Excluir Palavra\n");
        printf("5 - Sair\n");
        printf("\nDigite uma opcao:\n");
        scanf("%d", &opcao);

        switch (opcao) {//SWITCH PRINCIPAL
            case 1:
                printf("--------TODAS AS PALAVRAS--------\n");
                if(raiz != NULL){
                    buscarArvore(raiz);
                     printf("\n");
                }else{
                    printf("Não ha elementos na arvore\n");
                }
                
                break;

            case 2:
                printf("---------BUSCAR PALAVRA---------\n");
                if(raiz != NULL){
                    printf("Digite palavra:");
                    scanf("%s", palavra);
                    buscarPalavra(raiz, palavra);
                }else{
                    printf("Não ha elementos na arvore\n");
                }
                
                break;
                
            case 3:
                printf("---------ADICIONAR PALAVRA---------\n");
                printf("Digite a palavra: ");
                scanf("%s", novaPalavra.palavra);
                do{
                printf("Digite quantidade de sinonimos de 1 a %d: ", MAX);
                scanf("%d", &novaPalavra.qntSin);
                }while((novaPalavra.qntSin < 1) || (novaPalavra.qntSin > MAX));
                
                i = 0;
                while(i < novaPalavra.qntSin){
                    printf("Sinonimo %d:", i+1);
                    scanf("%s",novaPalavra.sinonimos[i].palavraSin);
                    i++;
                }
                if(inserirArvore(raiz,novaPalavra) != 0){
                    printf("Palavra '%s' adicionada com sucesso\n\n",
                            novaPalavra.palavra);
                    qntP++;
                }else{
                    printf("Palavra '%s' nao pode ser adicionada\n\n",
                            novaPalavra.palavra);
                }
                break;
                
            case 4:
                printf("--------REMOVER PAVALRA--------\n");
                printf("Digite palavra a ser removida: ");
                scanf("%s", palavra);
                if(raiz != NULL){
                    if(removerPalavra(raiz, palavra) == 1){
                        printf("\nREMOVIDO!\n");
                        qntP--;
                    }else{
                        printf("\nPalavra nao esta no dicionario\n");
                    }
                }else{
                    printf("Não ha palavras no dicionario\n");
                }
                printf("\n");
                break;
                
            case 5:
                sprintf(qnt, "%i", qntP);
                fprintf(output, qnt);
                fprintf(output, "\n");
                
                escreverNoArquivo(raiz, output, qnt);
                rodando = 0;
                fclose(output);
                break;
            default:
                printf("Erro! Opcao invalida, tente novamente!\n\n");

        }
    }
    return 0;
}
