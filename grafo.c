#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int info;
struct Grafo_lista;
struct Grafo_sub_lista;
struct Header;

typedef struct Grafo_lista
{
	struct Grafo_lista *prox;
	info num;
	info valor;
	struct Grafo_sub_lista *inicio_sub_lista;
	struct Grafo_sub_lista *fim_sub_lista;
	struct Grafo_lista *ant;
}grafo_lista;

typedef struct Grafo_sub_lista
{
	struct Grafo_lista *prox;
	info origem_destino[2];
	int custo;
	struct Grafo_lista *ptr_destino;
	struct Grafo_lista *ant;
}grafo_sub_lista;

typedef struct Header
{
	struct Grafo_lista *inicio_lista;
	struct Grafo_lista *fim_lista;
	int tamanho;
}header;

grafo_lista *busca_grafo(header *header_grafo, info chave);

grafo_sub_lista* insere_grafo_sub_lista(header *header_grafo, grafo_lista *nodo_grafo, info destino, info custo)
{
    grafo_sub_lista *ptr_aux = (grafo_sub_lista *) calloc(1, sizeof(grafo_sub_lista));
    ptr_aux->origem_destino[0]=nodo_grafo->num;
    ptr_aux->origem_destino[1]=destino;
    ptr_aux->custo=custo;

    ptr_aux->ptr_destino=busca_grafo(header_grafo,destino);
    if (nodo_grafo->inicio_sub_lista == NULL)
        nodo_grafo->inicio_sub_lista = ptr_aux;
    else
 		nodo_grafo->fim_sub_lista->prox = ptr_aux;
    nodo_grafo->fim_sub_lista = ptr_aux;
    return ptr_aux;
}
grafo_lista *insere_grafo_lista (header *header_grafo, info num, info valor)
{
    grafo_lista *ptr_aux = (grafo_lista *) calloc(1,sizeof (grafo_lista));
    ptr_aux->num=num;
    ptr_aux->valor=valor;
    ptr_aux->ant = header_grafo->fim_lista;
    if(header_grafo->inicio_lista == NULL)
        header_grafo->inicio_lista = ptr_aux;
    else
        header_grafo->fim_lista->prox = ptr_aux;
    header_grafo->fim_lista = ptr_aux;
    header_grafo->tamanho++;
    return(ptr_aux);
}
grafo_lista *busca_grafo(header *header_grafo, info chave)
{
	grafo_lista *aux;
	for(aux=header_grafo->inicio_lista;((aux->num!=chave)&&(aux->prox!=NULL));aux=aux->prox);
	return (aux);
}
void gera_grafo(header *header_grafo)
{
	char aux[30];
	info info_aux[2];
	grafo_lista *ptr_aux;
	FILE *fp=fopen("grafo.txt","r");
	if(!fp)
	{
		printf("Arquivo nao está presente.\n");
		exit(1);
	}
	while(!feof(fp))
	{
		fscanf(fp,"%d",&info_aux[0]);
		fscanf(fp,"%d",&info_aux[1]);
		printf("%d-%d\n",info_aux[0],info_aux[1]);
		getc(fp);
		fscanf(fp,"%[^'\n']",aux);
		insere_grafo_lista(header_grafo,info_aux[0],info_aux[1]);
	}
	rewind(fp);

	while(!feof(fp))
	{
		fscanf(fp,"%d",&info_aux[0]);
		fscanf(fp,"%d",&info_aux[1]);
		getc(fp);
		do
		{
			//printf("teste2\n");
			fscanf(fp,"%s",aux);
			info_aux[0]=atoi(aux);
			fscanf(fp,"%s",aux);
			info_aux[1]=atoi(aux);
			ptr_aux=busca_grafo(header_grafo,info_aux[0]);
			aux[0]=getc(fp);
			if(ptr_aux!=NULL)
			{
				insere_grafo_sub_lista(header_grafo,ptr_aux,info_aux[0],info_aux[1]);
			}
		}
		while((((char*) memchr (aux, '.', 1+strlen(aux)))!=NULL) && (((char*) memchr (aux, '\n', 1+strlen(aux)))!=NULL)) ;
	}
}

void percorrer_printar_grafo (header *header_grafo)
{
	grafo_lista *corrente;
	grafo_sub_lista *evcorrente;
	for (corrente=header_grafo->inicio_lista;corrente!=NULL;corrente=corrente->prox)
	{
		printf("\nNó-%d Valor-%d ",corrente->num,corrente->valor);

		if(corrente->inicio_sub_lista!=NULL)
		{
			printf("\nLigações\n");
			for(evcorrente = corrente->inicio_sub_lista; evcorrente != NULL; evcorrente = evcorrente->prox)
			{
				printf("%d->%d\n", evcorrente->origem_destino[0],evcorrente->origem_destino[1]);
			}
			printf("\n");
		}
	}
	printf("\n");
}

int main()
{
	header *header_grafo=(header *)calloc(1,sizeof(header));
	gera_grafo(header_grafo);
	percorrer_printar_grafo(header_grafo);

	return 0;
}


