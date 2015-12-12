#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int info;
//struct Grafo_lista;
//struct Grafo_sub_lista;
//struct Header;

typedef struct Grafo_lista grafo_lista;

typedef struct Grafo_sub_lista grafo_sub_lista;

typedef struct Header header;


struct Grafo_lista
{
	grafo_lista *prox;
	info num;
	info valor;
	grafo_sub_lista *inicio_sub_lista;
	grafo_sub_lista *fim_sub_lista;
	grafo_lista *ant;
};

struct Grafo_sub_lista
{
	struct Grafo_lista *prox;
	info origem_destino[2];
	int custo;
	int usado;
	grafo_lista *ptr_destino;
	grafo_lista *ant;
};

struct Header
{
	grafo_lista *inicio_lista;
	grafo_lista *fim_lista;
	int tamanho;
};

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
	grafo_lista *aux=NULL;
	for(aux=header_grafo->inicio_lista;((aux->num!=chave)&&(aux->prox!=NULL));aux=aux->prox);
	return (aux);
}
void gera_grafo(header *header_grafo)
{
	char aux[30], *ptr=NULL;
	info info_aux[3];
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
		getc(fp);
		fscanf(fp,"%d",&info_aux[1]);
		getc(fp);
		//printf("%d-%d\n",info_aux[0],info_aux[1]);
		fscanf(fp,"%[^'\n']",aux);
		insere_grafo_lista(header_grafo,info_aux[0],info_aux[1]);
		info_aux[0]=info_aux[1]=0;
	}
	rewind(fp);
	//printf ("\n");
	while(!feof(fp))
	{
		fscanf(fp,"%[^'\n-\r']",aux);
		getc(fp);
		ptr=strchr (aux, '-')+1;
		if ((ptr!=NULL))
		{
			info_aux[2]=atoi(aux);
			ptr = strtok (ptr," ");
			//printf("string aux=%d\n", info_aux[2]);
	  		while ((ptr != NULL))
	  		{
	    		if(*ptr!='.')
	    		{
	    			sscanf (ptr,"%d",&info_aux[0]);
	    			ptr = strtok (NULL, " ");
	    			sscanf (ptr,"%d",&info_aux[1]);
	    			//printf ("%d-%d\n",info_aux[0],info_aux[1]);
	    			ptr_aux=busca_grafo(header_grafo,info_aux[2]);
	    			insere_grafo_sub_lista(header_grafo,ptr_aux,info_aux[0],info_aux[1]);
	    		}
	    		ptr = strtok (NULL, " ");
	  		}
	  	}
	 }
}

void percorrer_printar_grafo (header *header_grafo)
{
	grafo_lista *corrente;
	grafo_sub_lista *evcorrente;
	for (corrente=header_grafo->inicio_lista;corrente!=NULL;corrente=corrente->prox)
	{
		printf("\nNo = %d Valor = %d\n",corrente->num,corrente->valor);

		if(corrente->inicio_sub_lista!=NULL)
		{
			printf("Ligacoes\n");
			for(evcorrente = corrente->inicio_sub_lista; evcorrente != NULL; evcorrente = evcorrente->prox)
			{
				if(evcorrente->usado==0)
					printf("->%d  arco = %d\n",evcorrente->origem_destino[1],evcorrente->custo);
			}
		}
		printf("_____________________");
	}
	printf("\n");
}
void adicionar_no(header *header_grafo)
{
	int no[2],arco[2];
	char aux;
	grafo_lista *ptr;
	printf("Digite o NUMERO do no' a ser adicionado no grafo:");
	scanf("%d",&no[0]);
	getchar();
	printf("Digite o VALOR do no' a ser adicionado no grafo:");
	scanf("%d",&no[1]);
	getchar();
	insere_grafo_lista (header_grafo, no[0], no[1]);
	do
	{
		printf("Deseja adicionar um arco a este no'  S-N:");
		scanf("%c",&aux);
		getchar();
		if((aux=='S')||(aux=='s'))
		{
			printf("Digite a quem o arco se conecta:");
			scanf("%d",&arco[0]);
			getchar();
			ptr=busca_grafo(header_grafo, arco[0]);
			if(ptr==NULL)
			{
				printf("Este no ainda nao esta no grafo, para ligar um novo no a ele, o insira primeiro.\n");
				exit(EXIT_FAILURE);
			}
			printf("Digite o valor do arco:");
			scanf("%d",&arco[1]);
			getchar();
			ptr=busca_grafo(header_grafo, no[0]);
			insere_grafo_sub_lista(header_grafo,ptr, arco[0], arco[1]);
		}
	}while((aux=='s')||(aux=='S'));
}
void remover_no(header *header_grafo)
{
	int no;
	char aux;
	grafo_lista *ptr[3];
	printf("Digite o NUMERO do no' a ser retirado do grafo:");
	scanf("%d",&no);
	ptr[1]=busca_grafo(header_grafo, no);
	if(ptr[1]==NULL)
	{
		printf("No não esta presente no Grafo\n");
		exit(EXIT_FAILURE);
	}

	grafo_lista *corrente,*elimina_lista;
	grafo_sub_lista *evcorrente,*elimina_sub_lista ;

	for (corrente=header_grafo->inicio_lista;corrente!=NULL;corrente=corrente->prox)
	{
		if(corrente->inicio_sub_lista!=NULL)
		{
			for(evcorrente = corrente->inicio_sub_lista; evcorrente != NULL; evcorrente = evcorrente->prox)
			{
				if(evcorrente->origem_destino[1]==no)
				{
					evcorrente->usado=1;
				}
			}
		}
		if(corrente->num==no)
		{
			if(header_grafo->inicio_lista==corrente)
				header_grafo->inicio_lista=corrente->prox;
			if(header_grafo->fim_lista==corrente)
				header_grafo->fim_lista=corrente->ant;
			if(corrente->ant!=NULL)
				corrente->ant->prox=corrente->prox;
			if(corrente->prox!=NULL)
				corrente->prox->ant=corrente->ant;
			free(corrente);
		}
	}
}
void menu(header *header_grafo)
{
	int i;
	printf("\n\t\tMENU\n");
	printf("1 - Inserir no Grafo\n");
	printf("2 - Retiras do Grafo\n");
	scanf("%d",&i);
	getchar();
	switch (i)
	{

		case 1:
		adicionar_no(header_grafo);
		break;

		case 2:
		remover_no(header_grafo);
		break;

		default:
		printf("Opcao invalida\n");
		exit(EXIT_FAILURE);
		break;
	}
}

int main()
{
	header *header_grafo=(header *)calloc(1,sizeof(header));
	gera_grafo(header_grafo);
	percorrer_printar_grafo(header_grafo);
	menu(header_grafo);
	percorrer_printar_grafo(header_grafo);

	return 0;
}


