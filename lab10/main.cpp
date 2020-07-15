#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <ctime>
#include <math.h>
#include "tools.h"
using namespace std;
bool isPowerOfTwo(int x){
	return (x != 0) &&((x &(x-1)));
}

int main(int argc, char **argv){
    // (1) argumentos  ./prog n k m
    if(argc != 4){
        fprintf(stderr, "run as ./prog n=size k=wolves m=mode\n n potencia de dos\nk divisor de n & k potencia de 2\nmode : {'determinist':0,'random_walk':1}\n");
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    while (isPowerOfTwo(n)){
    	    printf("\nReingrese n\n n potencia de dos: ");
    	    scanf("%d",&n);
    }
    int k = atoi(argv[2]);
    while (isPowerOfTwo(k) || n*n%k != 0){ // OR -> ||
	    //lo pido denuevo
	    printf("\nReingrese k\nk divisor de n*n\n k>1 & k potencia de dos: ");
	    scanf("%d",&k);
    }
    int m = atoi(argv[3]);
    while (m != 0 && m !=1){ // AND -> &&
	    printf("\nReingrese m: [0,1] ");
	    scanf("%d",&m);
    }
    // (2) crear e iniciar dominio n x n
    int *mat = new int[n * n];
    init_mat_const(mat, n, 0);
    
    srand(time(NULL));

    // (3) ubicar manada
    vector<pair<int, int>> manada(k);
    if(m == 0){
        ubicar_manada_det(mat, n, manada);
    }
    else if(m == 1){
        ubicar_manada_nondet(mat, n, manada);
    }
    // (4) ubicar lobito cachorro 
    ubicar_lobito(mat, n);


    print_mat(mat, n);
    getchar();

    // (5) proceso de busqueda
    if(m ==0){
        //pair<int,int> p = busqueda_det(mat, n, manada);
    }
    else if(m == 1){
        pair<int,int> p = busqueda_nodet(mat, n, manada);
    }
    return EXIT_SUCCESS;
}

