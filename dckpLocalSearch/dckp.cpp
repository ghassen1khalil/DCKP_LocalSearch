#include "dckp.h"

void recherche_locale(solution *sol, problem *pb, int *r)
{
	solution *bs;
	int i, j, bi, bj, k;
	bs=build_empty_sol(pb);
	copy_sol(sol,bs,pb);
	*r=0;
	do
	{
		(*r)++;
		//printf("iter %d\n",r);
		bi=bj=-1;
		for(i=0;i<pb->n-1;i++)
		{
			for(j=i+1;j<pb->n;j++)
			{
				if(sol->x[i]==1 && sol->x[j]==0 
					&& sol->V - pb->v[i] + pb->v[j] <= pb->C 
					&& sol->P - pb->p[i] + pb->p[j] > bs->P)
				{
					for(k=0;k<pb->n;k++)
						if(k!=i && sol->x[k]==1 && pb->e[j][k]==1)
							break;
					if(k==pb->n)
					{
						bs->V = sol->V - pb->v[i] + pb->v[j];
						bs->P = sol->P - pb->p[i] + pb->p[j];
						bi = i;
						bj = j;
						goto fin;
					}
				}
				else if(sol->x[i]==0 && sol->x[j]==1 
					&& sol->V - pb->v[j] + pb->v[i] <= pb->C 
					&& sol->P - pb->p[j] + pb->p[i] > bs->P)
				{
					for(k=0;k<pb->n;k++)
						if(k!=j && sol->x[k]==1 && pb->e[i][k]==1)
							break;
					if(k==pb->n)
					{
						bs->V = sol->V - pb->v[j] + pb->v[i];
						bs->P = sol->P - pb->p[j] + pb->p[i];
						bi = i;
						bj = j;
						goto fin;
					}
				}
			}
		}
fin:
		if(bi>=0)
		{
			if(sol->x[bi]==1 && sol->x[bj]==0)
			{
				bs->x[bi]=sol->x[bi]=0;
				bs->x[bj]=sol->x[bj]=1;
			}
			else
			{
				bs->x[bi]=sol->x[bi]=1;
				bs->x[bj]=sol->x[bj]=0;
			}
			sol->P=bs->P;
			sol->V=bs->V;
			//copy_sol(bs,sol,pb);
		}

	}while(bi>=0);

	/*if(*r>1)
		copy_sol(bs,sol,pb);
		*/
	desallouer_sol(bs);
}

solution * build_init_sol(problem *pb)
{
	int i,j,k;
	int *t1;//tableau contenant les indices des objets triés dans l'ordre croissant de leurs volumes
	int *t2;//tableau contenant les indices des objets triés dans l'ordre décroissant de leurs profits
	int *t3;//tableau contenant les indices des objets triés dans l'ordre croissant de leurs volumes/profits
	solution *s;
	s=build_empty_sol(pb);

	//Remplissage de t1
	t1=(int*)malloc(pb->n*sizeof(int));
	for(i=0;i<pb->n;i++)
		t1[i]=i;
	for(i=0;i<pb->n-1;i++)
		for(j=i+1;j<pb->n;j++)
			if(pb->v[t1[i]]>pb->v[t1[j]])
			{
				k=t1[i];
				t1[i]=t1[j];
				t1[j]=k;
			}
	/*for(i=0;i<pb->n;i++)
		printf("%d\t",t1[i]);
	printf("\n\n");*/

	//Remplissage de t2
	t2=(int*)malloc(pb->n*sizeof(int));
	for(i=0;i<pb->n;i++)
		t2[i]=i;
	for(i=0;i<pb->n-1;i++)
		for(j=i+1;j<pb->n;j++)
			if(pb->p[t2[i]]<pb->p[t2[j]])
			{
				k=t2[i];
				t2[i]=t2[j];
				t2[j]=k;
			}
	/*for(i=0;i<pb->n;i++)
		printf("%d\t",t2[i]);
	printf("\n\n");*/

	//Remplissage de t3
	t3=(int*)malloc(pb->n*sizeof(int));
	for(i=0;i<pb->n;i++)
		t3[i]=i;
	for(i=0;i<pb->n-1;i++)
		for(j=i+1;j<pb->n;j++)
			if(pb->v[t3[i]]/pb->p[t3[i]]>pb->v[t3[j]]/pb->p[t3[j]])
			{
				k=t3[i];
				t3[i]=t3[j];
				t3[j]=k;
			}
	/*for(i=0;i<pb->n;i++)
		printf("%d\t",t3[i]);
	printf("\n\n");*/

	//Construction d'une solution
	i=0;
	while(i <pb->n && s->V+pb->v[t1[i]]<=pb->C)
	{
		for(j=0;j<pb->n;j++)
		{
			if(s->x[j]==1 && pb->e[t1[i]][j]==1)
				break;
		}
		if(j==pb->n)
		{
			s->x[t1[i]]=1;
			s->V+=pb->v[t1[i]];
			s->P+=pb->p[t1[i]];
		}
		i++;
	}
	return s;
}

void copy_sol(solution *s1, solution *s2, problem *pb)
{
	int i;
	for(i=0;i<pb->n;i++)
		s2->x[i]=s1->x[i];
	s2->P=s1->P;
	s2->V=s1->V;
}

solution * build_empty_sol(problem *pb)
{
	int i;
	solution *s;
	s=(solution*)malloc(sizeof(solution));
	s->x=(int*)malloc(pb->n*sizeof(int));
	for(i=0;i<pb->n;i++)
		s->x[i]=0;
	s->P=s->V=0;
	return s;	
}

problem *chargement_prob(char* fichier)
{
	int i,j,k;
    problem *dckp;
    FILE *fp;
    errno_t err;

	if((err=fopen_s(&fp,fichier,"r"))!=0)
		printf("Erreur d'ouverture du fichier %s...\n", fichier);

	dckp=(problem*)malloc(sizeof(problem));
	
	
	fscanf_s(fp, "%d %d", &dckp->n,&dckp->m);

	dckp->C=0;	
	dckp->v= (int*)malloc(dckp->n * sizeof(int));
	dckp->p= (int*)malloc(dckp->n * sizeof(int));
	dckp->e= (int**)malloc(dckp->n * sizeof(int*));
	for(i=0;i<dckp->n;i++)
		dckp->e[i]=(int*)malloc(dckp->n * sizeof(int));
	for(i=0;i<dckp->n;i++)
		for(j=0;j<dckp->n;j++)
			dckp->e[i][j]=0;

	for(j=0; j<dckp->n; j++) 
		fscanf_s(fp,"%d", &(dckp->p[j]));
		
	//printf("%d %d\n",dckp->n,dckp->m);
	fscanf_s(fp, "%d", &dckp->C);
	
    for(j=0; j<dckp->n; j++)
		fscanf_s(fp, "%d ", &dckp->v[j]);

	for(i=0; i<dckp->m; i++) 
	{
        fscanf_s(fp,"%d", &j);
		fscanf_s(fp,"%d", &k);
		dckp->e[j][k]=dckp->e[k][j]=1;
	}
	  
    fclose(fp);   
    return dckp;
}

void display_sol(solution *s, problem *pb)
{
	int i;	
	//printf("n=%d\n",pb->n);
	//printf("Les objets pris dans le sac...\n");
	/*for(i=0;i<pb->n;i++)
		if(s->x[i]==1)
			printf("%d-",i);
	printf("\n");*/
	
	printf("P=%d\tV=%d\tC=%d\n\n",s->P,s->V,pb->C);
}

void desallouer_prob(problem *kp)
{
	free(kp->v);
	free(kp->p);
	for(int i=0;i<kp->n;i++)
		free(kp->e[i]);
	free(kp->e);
	free(kp);
}

void desallouer_sol(solution *s)
{
	free(s->x);
	free(s);
}