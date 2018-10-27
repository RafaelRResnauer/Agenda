/*
 * Sistemas Operacionais
 * CCET - PUCPR
 * Prof. Luiz Lima Jr.
 *
 * PERSISTENCIA (Sistemas de Arquivos)
 * Agenda de nomes - aniversários - telefones
 *
 * O programa abaixo não é persistente (isto é, não armazena os dados da
 * agenda lidos do usuário). Altere-o de forma a torná-lo persistente.
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXTAB  10
#define MAXNOME 64
#define MAXTEL  32

typedef char Nome[MAXNOME];     /* Nome = char [MAXNOME] */

typedef struct {
    int dia;
    int mes;
} DataAniversario;              /* Data do aniversário */

typedef char Telefone[MAXTEL];  /* Telefone = char [MAXTEL] */

typedef struct {
    Nome nome;                 /* nome da pessoa */
    DataAniversario niver;     /* dia e mes do aniversario da pessoa */
    Telefone tel;              /* telefone da pessoa */
} Registro;

typedef Registro Tabela[MAXTAB];  /* Tabela = vetor de MAXTAB Registros */

/* PROGRAMA PRINCIPAL */

int main()
{
    Tabela tab;
    unsigned int N;        /* no. de entradas na tabela */
    Nome nome_aux;         /* auxiliar temporario para o nome */
    unsigned int i;        /* contador */
    int fd; /*agenda */
	char c; /*char auxiliar*/
	int loop = 1; /* controlador de loop*/
	
    N = 0;
    
    printf("\n*** AGENDA ***\n");
    printf("  SO - PUCPR\n\n");
    fd = open("agenda.dat", O_RDWR);
	/*verifica se o arquivo existe*/
	if(fd < 0) /*se o arquivo nao existir previamente*/
	{
		fd = creat("agenda.dat",0600);
		/* Le tabela de nomes/aniversarios/telefones */
    
		printf(">> Criando tabela de nomes\n");
		printf("   (Digite \"fim\" no nome para terminar.)\n\n");
		printf("      Nome: ");
		scanf("%s",nome_aux);
		while ((strcmp(nome_aux,"fim") != 0) && (N<MAXTAB)) /* enq nome != "fim" */
		{
			strcpy(tab[N].nome,nome_aux);      /* tab[N].nome = nome_aux */
			printf("      Dia aniv.: ");
			scanf("%d",&(tab[N].niver.dia));
			printf("      Mes aniv.: ");
			scanf("%d",&(tab[N].niver.mes));
			printf("      Telefone: ");
			scanf("%s",tab[N].tel);
			N++;
			/* Le proximo nome */
			printf("\n      Nome: ");
			scanf("%s",nome_aux);
		}
		
		/* Procura nome e imprime data de aniversario e tel correspondentes */
		printf("\n>> Lendo nomes da tabela\n");
		printf("   (Digite \"fim\" para terminar.)\n\n");
		printf("      Nome: ");
		scanf("%s",nome_aux);
		while (strcmp(nome_aux,"fim") != 0) 
		{
		
			/* procura nome na tabela */
			for (i=0; i<N; i++) 
			{
				if (strcmp(nome_aux,tab[i].nome) == 0)   /* nome_aux == tab[i].nome */
					break;    /* encontrou */
			}
			if (i == N)
			{
				/* nome nao enconrtado */
				printf("\n\t* Nome \"%s\" nao encontrado na tabela!\n\n",nome_aux);
			} else 
			{
				/* nome encontrado */
			  printf("\n\t* %s => %d/%d (Tel: %s)\n\n",tab[i].nome,tab[i].niver.dia,tab[i].niver.mes,tab[i].tel);
			}
		
			/* Le proximo nome */
			printf("      Nome: ");
			scanf("%s",nome_aux);
		}
	}else /*se o arquivo existir*/
	{
		read(fd, &N, sizeof(unsigned int));
		read(fd, &tab, sizeof(Tabela));
		while(loop)
		{
			printf("Deseja adicionar nomes?(s/n)\n");
			scanf("%c",&c);
			if(c != 'n' && c != 'N' && c != 's' && c != 'S')
			{
				printf("Escolha invalida!\n");
			}else
			{
				loop = 0;
			}
		}
		if(c == 's' || c == 'S')
		{
			/* Le tabela de nomes/aniversarios/telefones */
			printf(">> Adicionando a tabela de nomes\n");
			printf("   (Digite \"fim\" no nome para terminar.)\n\n");
			printf("      Nome: ");
			scanf("%s",nome_aux);
			while ((strcmp(nome_aux,"fim") != 0) && (N<MAXTAB)) /* enq nome != "fim" */
			{
				strcpy(tab[N].nome,nome_aux);      /* tab[N].nome = nome_aux */
				printf("      Dia aniv.: ");
				scanf("%d",&(tab[N].niver.dia));
				printf("      Mes aniv.: ");
				scanf("%d",&(tab[N].niver.mes));
				printf("      Telefone: ");
				scanf("%s",tab[N].tel);
				N++;
				/* Le proximo nome */
				printf("\n      Nome: ");
				scanf("%s",nome_aux);
			}
		}
		/* Procura nome e imprime data de aniversario e tel correspondentes */
		printf("\n>> Lendo nomes da tabela\n");
		printf("   (Digite \"fim\" para terminar.)\n\n");
		printf("      Nome: ");
		scanf("%s",nome_aux);
		while (strcmp(nome_aux,"fim") != 0) 
		{
			/* procura nome na tabela */
			for (i=0; i<N; i++) {
				if (strcmp(nome_aux,tab[i].nome) == 0)   /* nome_aux == tab[i].nome */
					break;    /* encontrou */
			}
			if (i == N) {
				/* nome nao enconrtado */
				printf("\n\t* Nome \"%s\" nao encontrado na tabela!\n\n",nome_aux);
			} else {
				/* nome encontrado */
			  printf("\n\t* %s => %d/%d (Tel: %s)\n\n",tab[i].nome,tab[i].niver.dia,tab[i].niver.mes,tab[i].tel);
			}
		
			/* Le proximo nome */
			printf("      Nome: ");
			scanf("%s",nome_aux);
		}
	}
	/*salva o conteudo no arquivo*/
    lseek(fd,0,SEEK_SET);
	write(fd, &N, sizeof(unsigned int));
	write(fd, &tab, sizeof(Tabela));
	
	/*fecha o arquivo*/
	close(fd);
	
    printf("\nFIM\n");
    
    return 0;
}