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

list *ajustarNew( list* L, int i, int b){
    list *aux=NULL;
    
    if(L->indice == i){                                     // ajustarNew é a função que ajusta a lista de areas livres na lista encadeada
        L->indice = i+b;
        L->blocos = (L->blocos) - b;
        
        if(L->prox != NULL){
            aux= L->prox;
            if( L->indice == aux->indice || L->blocos == 0){
                free(L);
                return aux;
            }
        }  
        if(L->blocos == 0){
            free(L);
            return NULL;
        }
        return L;
    }
    
    L->prox = ajustarNew(L->prox, i, b);
    
    return L;
}

list *First( list* V, list *L, char *id, int b, int *f){
    
    if(L != NULL){
        if(L->blocos >= b){
           
           
            list *aux= (list*)malloc(sizeof(list));
                
            strcpy( aux->id, id);
            aux->indice= L->indice;
            aux->blocos= b;
            aux->prox= V;
            return aux;
           
           
        }
        else{
            return First(V, L->prox, id, b, f);
        }
    }
    printf("Falta de memória, variável não alocada\n\n");
    *f= -1;
    return V;
}

list *Next( list *V, list *livre, list **Pnext, char *id, int b){    // função next sempre parte do pressuposto que a heap tem area livre para armazenar
    
    if( *Pnext == NULL){
        *Pnext = livre;
    }
    if( (*Pnext)->blocos >=b){
        
        list *aux= (list*)malloc(sizeof(list));                             
        
        strcpy(aux->id, id);
        aux->indice= (*Pnext)->indice;
        aux->blocos= b;
        aux->prox= V;
        return aux;
    }
    else{
        *Pnext= (*Pnext)->prox;
        return Next(V, livre, Pnext, id, b);
    }
}

int auxBestWorst(bool flag, list *L, int b, int indice, int dif){
    
    if(L == NULL){
        return indice;
    }
    
    int sub= L->blocos - b;
    if(flag == 1){
        
        if(sub>=0){
            if( dif > sub){
                
                return auxBestWorst(1, L->prox, b, L->indice, sub);
            } 
        }
        return auxBestWorst(1, L->prox, b, indice, dif);
    }
    else{
        if( dif < sub){
            return auxBestWorst(0, L->prox, b, L->indice, sub);
        }
        return auxBestWorst(0, L->prox, b, indice, dif);
    }
}

list *Best( list *V, list *L, char *id, int b, int *f){
    
    int i= auxBestWorst( 1, L, b, -1, 100);
    if(i < 0){
        printf("Falta de memória, variável não alocada\n\n");
        *f= -1;
        return V;
    }
    else{
        list *aux= (list*)malloc(sizeof(list));
        
        strcpy(aux->id , id);
        aux->indice= i; 
        aux->blocos= b;
        aux->prox= V;
        return aux;
    }
}

list *Worst( list *V, list *L, char *id, int b, int *f){
    
    int i= auxBestWorst( 0, L, b, -1, -1);
    if(i < 0){
        printf("Falta de memória, variável não alocada\n\n");
        *f= -1;
        return V;
    }
    else{
        list *aux= (list*)malloc(sizeof(list));
        
        strcpy(aux->id , id);
        aux->indice= i; 
        aux->blocos= b;
        aux->prox= V;
        return aux;
    }
}

list *Deletar( list *V, int i){
    
    if(V!=NULL){
        if(V->indice == i){         // Deletar mantem a váriavel na lista (para as atribuições) mas registra que ela agora ocupa 0 blocos na heap
            V->indice= 0;
            V->blocos= 0;
            
        }
        
        V->prox= Deletar( V->prox, i);
        return V;
        
    }
}

list *ajustarDel( list *L, int i, int b){                   // ajustarDel organiza a lista de areas livres no caso do deletar
    
    if(L!= NULL){
        
        list *aux=NULL;
        
        if(L->indice > i){                              // testo para ver se o novo bloco de area livre ficará antes ou depois do bloco atual(para manter a ordenação pelos indices)
            if(i+b == L->indice){                       //checo se os 2 blocos devem ser mesclados
                L->indice= i;
                L->blocos= L->blocos + b;
            
                return L;
            }
            else{
                aux= (list*)malloc(sizeof(list));
                aux->indice= i;
                aux->blocos= b;
                aux->prox= L;
                return aux;
            }
        }
        else{                                       // caso o novo bloco deva ir depois do atual
            
            if( (L->indice + L->blocos) == i ){    // checo se eles devem se mesclar
                L->blocos= L->blocos + b;
            
                if(L->prox != NULL){
                    aux= L->prox;
                
                    if( (L->indice + L->blocos) == aux->indice ){   // checagem se os 2 blocos mesclados devem se unir ao proximo 
                        L->blocos= L->blocos + aux->blocos;
                        L->prox= aux->prox;
                        free(aux);
                    }
                }
                return L;
            }
            else{                                   // caso eles sejam blocos separados
                
                if(L->prox != NULL){
                    L->prox= ajustarDel(L->prox, i, b);         // chamada recursiva para o proximo bloco
                    return L;
                }
                
                aux= (list*)malloc(sizeof(list));           //caso não haja proximo bloco ( L->prox = NULL) e os blocos não foram mesclados então um novo bloco é criado apos o atual 
                aux->indice= i;
                aux->blocos= b;
                aux->prox= L->prox;
                L->prox= aux;
                
                return L;
            }
        }
    }
}

list *atribuicao(list *V, char *id, int i, int b){
    if(V != NULL){
        if( strcmp(V->id, id) == 0){
            
            V->indice= i;
            V->blocos= b;
            return V;
        }
        else{
            V->prox= atribuicao(V->prox, id, i, b);
            return V;
        }
    }
}

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
    
    printf("Áreas livres:\nIndice inicial   QTD.blocoslivres\n");
    
    for(list *aux=L; aux!=NULL; aux= aux->prox){
        printf("%d                %d", aux->indice, aux->blocos);
        printf("\n");
    }
}

int main()
{
    int set,blocos,f=1;
    
    bool heap[20];
    for(int i=0; i<20; i++){
        heap[i]= 0;
    }
    
    list *AreaL=Iniciar();
    list *Var=NULL, *aux=NULL;
    
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
                
                    Var= First(Var, AreaL, id, blocos,&f);
                    
                break;
                
                case 2:
                
                    list *PonteiroNext;   
                    
                    Var= Next(Var, AreaL, &PonteiroNext, id, blocos); // não consegui implementar o next para caso não haja espaço na heap
                    
                break;
                
                case 3:
                
                    Var= Best(Var, AreaL, id, blocos,&f);
                    
                break;
                
                case 4:
                
                    Var= Worst(Var, AreaL, id, blocos,&f);
                    
                break;
            }
            
            if(f != -1){
                
                AreaL= ajustarNew(AreaL, Var->indice, blocos);                     // ajustar controla a lista de areas livres no caso do new
                
                for(int i = Var->indice; i< (Var->indice)+blocos; i++){          // esse for modifica a heap de acordo
                    heap[i]= 1;
                }
            }
        }
        
        else if(strcmp(instrucao,"del") == 0){              // instrução del
            
            fscanf(arq,"%s", id);
            
            aux= Var;
            while( aux !=NULL && strcmp( aux->id, id) != 0){                // recupero dentro da lista de variaveis a variavel que quero deletar
                aux= aux->prox;
            }
            
            if( aux->blocos != 0){
                for(int i= aux->indice; i < (aux->indice) + (aux->blocos); i++){            // "Limpa" a memória (vetor heap) de acordo
                    heap[i]= 0;
                }
                AreaL= ajustarDel( AreaL, aux->indice, aux->blocos );                       // chama a função para ajustar a lista de areas livres
            }
            Var= Deletar( Var, aux->indice);                                            // chama a função para ajustar a lista de variaveis
            
        }
        
        else if(strcmp(instrucao,"exibe") == 0){
            printf("\n");
            exibe(heap, Var, AreaL);
            printf("\n\n");
        }
        
        else{        // atribuição de 2 variáveis
            
            fscanf(arq,"%s",id);        //acabei usando o fscanf 2 vezes para passar pelo '=' no arquivo, talvez tenha uma forma mais elegante
            fscanf(arq,"%s",id);
            
            aux= Var;
            while( aux !=NULL && strcmp( aux->id, instrucao) != 0){
                aux= aux->prox;                                         // obtenho acesso a variavel que esta do lado esquerdo da atribuição
            }
            
            if( aux->blocos != 0){
                for(int i= aux->indice; i < (aux->indice) + (aux->blocos); i++){
                    heap[i]= 0;                                                     // com os dados da variavel obtida modifico o vetor, parecido com del
                }
                AreaL= ajustarDel( AreaL, aux->indice, aux->blocos);
            }
            
            aux= Var; 
            while( aux !=NULL && strcmp( aux->id, id) != 0){                // obtendo acesso a variavel do lado direito da atribuição
                aux= aux->prox;
            }                                           
            Var= atribuicao( Var, instrucao, aux->indice, aux->blocos);                  //modifico a lista de variaveis (simulando a atribuição como sendo a de ponteiros)
            
            printf("\n");
        }
    }
    
    fclose(arq);
    return 0;
}
