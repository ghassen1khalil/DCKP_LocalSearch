#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct
{

  int n;	/*nombre d'objets*/
  int m;    /*nombre de couples incompatibles*/
  int * p;  /*vecteur des profits*/
  int * v;  /*vecteur des volumes*/
  int C;    /*capacité du sac*/ 
  int **e;  /*matrice binaire de taille nxn indiquant l'incompatibilité*/
  
}problem;

typedef struct
{

  int *x;       /*vecteur binaire pour la solution*/
  int V;        /*volume de la solution*/
  int P;      /*profit total de la solution*/
  
}solution;

void allouer_prob(problem *kp);

void desallouer_prob(problem *kp);

problem *chargement_prob(char* fichier);

void copy_sol(solution *s1, solution *s2, problem *pb);

solution * build_empty_sol(problem *pb);

void desallouer_sol(solution *s);

solution * build_init_sol(problem *pb);

void recherche_locale(solution *sol, problem *pb, int *r);

void display_sol(solution *s, problem *pb);





