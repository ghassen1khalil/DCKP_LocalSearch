#include <iostream>
#include <fstream>
#include "dckp.h"
using namespace std;
void main( int argc,char *argv[])
{	
	int i,j,k,obj_in,iter;
	long start, end;
	char instance[50];
	problem* dckp;
	solution *sol;
	double objval;
	errno_t err;
	FILE *ff,*fw;//Flux d'ouverture des fichiers
	if((err=fopen_s(&ff,"dckp_instances_list.txt","r"))!=0)
		printf("Erreur d'ouverture du fichier dckp_instances_list.txt...\n");
	
	///////
	if((err=fopen_s(&fw,"results_RL.xls","w"))!=0)
		printf("Erreur d'ouverture du fichier solutions.xls...\n");
	else
		fprintf(fw,"INSTANCES\tOBJ_IN\tOBJ_RL\tITER\tCPU\tITEMS\n");
	fclose(fw);
	///////
		
	for(k=0;k<50;k++)
	{   
		printf("\n+-------------------------+-------------------------+-------------------------+\n");
		fscanf_s(ff,"%s",instance);
		puts(instance);
		
		dckp=chargement_prob(instance);
		start=clock();
		printf("\nSolution Initiale \n");
		sol=build_init_sol(dckp);
		obj_in=sol->P;
		display_sol(sol,dckp);

		printf("Apres Recherche Locale \n");
		recherche_locale(sol,dckp,&iter);
		display_sol(sol,dckp);
		end=clock();
		
		printf("ITER=%d\tCPU=%.3lf\n\n",iter,(double)(end-start)/1000);

		if((err=fopen_s(&fw,"results_RL.xls","a+"))!=0)
			printf("Erreur d'ouverture du fichier solutions.xls...\n");
		
		fprintf(fw,"%s\t%d\t%d\t%d\t%.3lf\t",instance,obj_in,sol->P,iter,(double)(end-start)/1000);
		for(i=0;i<dckp->n;i++)
			if(sol->x[i]==1)
				fprintf(fw,"%d\t",i);
		fprintf(fw,"\n");
		fclose(fw);

		desallouer_prob(dckp);
		//system("pause");
	}
	fclose(ff);
	
}