#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode {
    struct ListNode *urmator;
    struct ListNode *anterior;
    int prioritate;
    char *nume;
} ListNode;

typedef struct List {
    ListNode *primul;
    ListNode *ultimul;
} List;

//  total_ghisee preia numarul de ghisee deschise
int total_ghisee = 0;

int calculeaza_numar_oameni_ghiseu(List **ghisee, int numar_ghiseu) {
	int total_oameni = 0;

	if (ghisee == NULL)
	{
		printf("Nu exista niciun vector de ghisee!");
		exit(1);
	}
	
	List* ghiseu = ghisee[numar_ghiseu];
	ListNode *persoana_curenta = ghiseu->primul;

	while (persoana_curenta != NULL)
	{
		total_oameni++;
		persoana_curenta = persoana_curenta->urmator;	
	}
	
	return total_oameni;
}

int calculeaza_numar_total_oameni(List **ghisee) {
	int total_oameni = 0, i;

	for (i = 0; i < total_ghisee; i++)
	{
		total_oameni += calculeaza_numar_oameni_ghiseu(ghisee, i);
	}

	return total_oameni;
}

void adauga_persoana(List ***ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {

	List *ghiseu = (*ghisee)[numar_ghiseu];
	ListNode *persoana = ghiseu->primul;

	//daca nu este nimeni la coada 
	if (persoana == NULL)
	{
		ListNode *persoana_la_coada = malloc(sizeof(ListNode));
		persoana_la_coada->urmator = NULL;
		persoana_la_coada->anterior = NULL;
		persoana_la_coada->prioritate = prioritate;
		persoana_la_coada->nume = nume;

		ghiseu->primul = persoana_la_coada;
		ghiseu->ultimul = persoana_la_coada;
	}
	else
	{
		while (persoana != NULL && (prioritate < persoana->prioritate 
			|| (prioritate == persoana->prioritate && strcmp(persoana->nume,nume) < 0)))
		{
			persoana = persoana->urmator;		
		}
		
		//daca persoana este la sfarsitul cozii
		if (persoana == NULL)
		{
			ListNode *persoana_la_coada = malloc(sizeof(ListNode));
			persoana_la_coada->urmator = NULL;
			persoana_la_coada->anterior = ghiseu->ultimul;
			persoana_la_coada->prioritate = prioritate;
			persoana_la_coada->nume = nume;

			ghiseu->ultimul->urmator = persoana_la_coada;
			ghiseu->ultimul = persoana_la_coada;

		}
		else
		{
			ListNode *persoana_la_coada = malloc(sizeof(ListNode));
			persoana_la_coada->urmator = persoana;
			persoana_la_coada->anterior = persoana->anterior;
			persoana_la_coada->prioritate = prioritate;
			persoana_la_coada->nume = nume;

			//daca este prima la coada
			if (persoana_la_coada->anterior == NULL)
			{
				ghiseu->primul = persoana_la_coada; 
			} 
			else
			{
				persoana_la_coada->anterior->urmator = persoana_la_coada;
				
			}
			persoana->anterior = persoana_la_coada;
		}
	}	
}

ListNode** memoreaza_persoane(List **ghisee,int l,int *total)
{
	int i,j=0;

	*total = calculeaza_numar_total_oameni(ghisee);

	ListNode **result = malloc(*total *sizeof(ListNode*));
	
	for(i=0; i<l; i++)
	{
		
		List *ghiseu = ghisee[i];
		ListNode *primul = ghiseu->primul;

		while(primul != NULL)
		{
			result[j] = primul;
			j++;
			primul = primul->urmator;
		}	
	}

	for(i=0; i<*total -1; i++)
	{
		for(j=i+1; j<*total; j++)
		{
			if(result[i]->prioritate < result[j]->prioritate 
				|| (result[i]->prioritate == result[j]->prioritate 
				&& strcmp(result[i]->nume,result[j]->nume) > 0))
			{
				ListNode *aux;
				aux = result[i];
				result[i] = result[j];
				result[j] = aux;
			}
		}
	}

	return result;
}


void deschide_ghisee(List ***ghisee, int N) {
	int i;

	if (*ghisee == NULL)
	{
		*ghisee = malloc(N * sizeof(List*));
	
		for (i = 0; i < N; i++)
		{
			(*ghisee)[i] = malloc(sizeof(List));
			(*ghisee)[i]->primul = NULL;
			(*ghisee)[i]->ultimul = NULL;	
		}	
	}
	else
	{
		int total = 0;

		ListNode **copy = memoreaza_persoane(*ghisee,total_ghisee,&total);
		List **g;

		g = malloc(N * sizeof(List*));
		for (i = 0; i < N; i++)
		{
			g[i] = malloc(sizeof(List));
			g[i]->primul = NULL;
			g[i]->ultimul = NULL;	
		}

		for (i = 0; i < total; i++)
		{
			int j = i % N;
			adauga_persoana(&g, copy[i]->nume, copy[i]->prioritate, j);
	
		}
		*ghisee = g;

	}

	total_ghisee = N;
}

void inchide_ghisee(List ***ghisee) {
	
	int i;
	for(i=0; i<total_ghisee; i++)
	{
		List *ghiseu = (*ghisee)[i];
		ListNode *persoana = ghiseu->primul;
		while(persoana != NULL)
		{
			ListNode *aux = persoana->urmator;
			free(persoana);
			persoana = aux;
		}
	}

}

void sterge_persoana(List ***ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {
	
	if(numar_ghiseu >= total_ghisee)
	{
		return ;
	}

	List* ghiseu = (*ghisee)[numar_ghiseu];
	ListNode *persoana = ghiseu->primul;

	while (persoana != NULL)
	{
		ListNode *urmator = persoana->urmator;
		if (strcmp(persoana->nume,nume) == 0 
			&& persoana->prioritate == prioritate)
		{
			ListNode *anterior = persoana->anterior;

			if (anterior != NULL)
			{
				anterior->urmator = urmator;
				
				//daca exista cineva dupa persoana
				if (urmator != NULL)
				{
					urmator->anterior = anterior;		
				}
				//daca se afla la coada
				else
				{
					ghiseu->ultimul = anterior;
				}
				
			}
			else
			{
				if (urmator != NULL)
				{
					urmator->anterior = NULL;
				}
				
				ghiseu->primul = urmator;
			}
			
			free(persoana);
		}
		persoana = urmator;
	}
}

void sterge_primii_oameni(List ***ghisee) {
	int i;

	for (i = 0; i < total_ghisee; i++)
	{
		List* ghiseu = (*ghisee)[i];
		if (ghiseu->primul != NULL)
		{
			ListNode *urmator = ghiseu->primul->urmator;
			if (urmator != NULL)
			{
				urmator->anterior = NULL;
			}
			
			ListNode *p = ghiseu->primul;
			ghiseu->primul = urmator;
			free(p);
		}
	}
}



int gaseste_pozitie_persoana(List **ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {

	int count = 0;

	if(numar_ghiseu >= total_ghisee)
	{
		return -1;
	}

	List *ghiseu = ghisee[numar_ghiseu];

	ListNode *persoana = ghiseu->primul;

	while(persoana != NULL)
	{
		if(persoana->prioritate == prioritate 
			&& strcmp(persoana->nume,nume) == 0)
		{
			 return count;

		}
		count++;		
		persoana = persoana->urmator;
		
	}	
	return -1;
}

char* gaseste_persoana_din_fata(List **ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {
	
	List *ghiseu = ghisee[numar_ghiseu];
	ListNode *persoana = ghiseu->primul;

	while(persoana != NULL)
	{
		if(persoana->prioritate == prioritate 
			&& strcmp(persoana->nume,nume) == 0)
		{
		
			if(persoana->anterior != NULL)
			{
				return persoana->anterior->nume;
			}
			else
			{
				return "Nu exista.";


			}
				
		}
		persoana = persoana->urmator;

	} 
	return "Nu exista.";
}

void afiseaza_rand_ghiseu(FILE *f, List **ghisee, int numar_ghiseu) {

	if(numar_ghiseu >= total_ghisee)
	{
		return ;
	}
	List* ghiseu = ghisee[numar_ghiseu];
	ListNode* persoana = ghiseu->primul;
	
	int persoane_in_asteptare = 0;
	while(persoana != NULL)
	{
		persoane_in_asteptare = 1;
		fprintf(f,"%s-%d;",persoana->nume,persoana->prioritate);
		persoana = persoana->urmator;	
	}
		
	if (persoane_in_asteptare == 0)
	{
		fprintf(f,"Ghiseul %d nu are nicio persoana in asteptare.",numar_ghiseu);
	}

	fprintf(f, "\n");
}

#endif

//---Puisor Oana-Daniela-grupa 311CD
