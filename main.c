/******************************************************************************

Trabalho LP: Exercicio 2
Autores: Leonardo Apicelo Valente e Tiago Acris Borges de Moraes

*******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct lista{
    char id[5];                 // estrutura genérica que irá armazenar a lista de variáveis e a lista de área livre
    int indice;
    int blocos;
    struct lista *prox;
}list;

list *Iniciar(){
    list *novo= (list*)malloc(sizeof(list));
    novo->indice=0;
    novo->blocos=20;
    novo->prox= NULL;
}

list *ajustar( list* L, int i, int b){
    list *aux=NULL;
    
    if(L->indice == i){
        L->indice = i+b;
        L->blocos = (L->blocos) - b;
        
        if(L->prox != NULL){
            aux= L->prox;
            if( L->indice == aux->indice){
                free(L);
                return aux;
            }
        }    
        else{
            return L;
        }
    }
    
    L->prox = ajustar(L->prox, i, b);
    
    return L;
}

list *First( list* V, list *L, char *id, int b){
    
    if(L->blocos >= b){
       
       list *aux= (list*)malloc(sizeof(list));
       
       strcpy( aux->id, id);
       aux->indice= L->indice;
       aux->blocos= b;
       aux->prox= V;
       return aux;
    }
    else{
        return First(V, L->prox, id, b);
    }
}

void Next(){}

void Best(){}

void Worst(){}

void exibe(bool *h, list *V, list *L){
    
    printf("Visão da heap:");
    for(int i=0; i<20; i++){
        printf(" %d", h[i]);
    }
    printf("\n");
    
    printf("Variaveis declaradas:\nID    Indice inicial    QTD.blocos ocupados\n");
    
    for(list *aux=V; aux!=NULL; aux= aux->prox){
        printf("%s     %d                 %d\n", aux->id, aux->indice, aux->blocos);                   
    }
    
    printf("Áreas livres:\nIndice inicial    QTD.blocoslivres\n");
    
    for(list *aux=L; aux!=NULL; aux= aux->prox){
        printf("%d                %d", aux->indice, aux->blocos);
    }
}

int main()
{
    int set,blocos;
    
    bool heap[20];
    for(int i=0; i<20; i++){
        heap[i]= 0;
    }
    
    list *AreaL=Iniciar();
    list *Var=NULL;
    
    char instrucao[21], id[5], id2[5];
    FILE *arq= fopen("entrada.txt", "rt");
    
    while( fscanf(arq,"%s",instrucao) != EOF ){
        
        if( strcmp(instrucao,"heap") == 0 ){            // setup do tipo de manipulação de memória no heap
        
            fscanf(arq,"%s",instrucao);
            if( strcmp(instrucao,"first") == 0 ){
                set=1;
            }
            else if( strcmp(instrucao,"next") == 0){
                set=2;
            }
            else if( strcmp(instrucao,"best") == 0){
                set=3;
            }
            else if( strcmp(instrucao,"worst") == 0){
                set=4;
            }
            printf("\n");
            
        }                                               
        
        else if(strcmp(instrucao,"new") == 0){
            fscanf(arq,"%s %d", id, &blocos);                       // armazena o id da variavel e a quantidade de espaço que ela ocupa
            
            switch(set){                                            // chamada dos diferentes métodos de manipulação
                case 1:
                
                    Var= First(Var, AreaL, id, blocos);
                    
                break;
                
                case 2:
                
                     //Var= Next(Var, AreaL, id, blocos)
                    
                break;
                
                case 3:
                
                     //Var= Best(Var, AreaL, id, blocos);
                    
                break;
                
                case 4:
                
                     //Var= Worst(Var, AreaL, id, blocos);
                    
                break;
            }
            
            AreaL= ajustar(AreaL, Var->indice, blocos);                     // ajustar controla a lista de areas livres no caso do new
            
            for(int i = Var->indice; i< (Var->indice)+blocos; i++){          // esse for modifica a heap de acordo
                heap[i]=1;
            }
        }// obs: não criei um caso onde não há espaço na heap para adição da váriavel, então sempre parto do principio que há espaço na heap(talvez modifique isso depois)
        
        else if(strcmp(instrucao,"del") == 0){
            printf("del");
            
            fscanf(arq,"%s", id);
            printf(": %s", id);
            printf("\n");
        }
        
        else if(strcmp(instrucao,"exibe") == 0){
            printf("\n");
            exibe(heap, Var, AreaL);
            printf("\n\n");
        }
        
        else{        // atribuição de 2 variáveis
            
            fscanf(arq,"%s",id);        //acabei usando o fscanf 2 vezes para passar pelo '=' no arquivo, talvez tenha uma forma mais elegante
            fscanf(arq,"%s",id);
            printf("%s recebe %s", instrucao, id);
            printf("\n");
        }
    }
    
    fclose(arq);
    return 0;
}
