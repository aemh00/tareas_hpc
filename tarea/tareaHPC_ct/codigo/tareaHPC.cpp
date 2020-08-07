#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <random>
#include <bits/random.h>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include "include/BasicCDS.h"

using namespace std;
using namespace cds;

#define PRINT 0
#define TEST 0

uint bM; // bits for MAX	
uint REPET = 5;
// Structure with all globals parameters program
typedef struct {
	char prefixResult[300];	// Prefix name of results files in the experiments

	ulong *A;
	ulong *X;
	ulong n;
	ulong MAX;

	ulong sizeA, sizeX;
	ulong nWX;		// number of Words for X[]
} ParProg;

void runQSAB(ParProg *par);
void genArrays(ParProg *par);
int partition_A(ulong *A, ulong l, ulong r);
void quickSort_A(ulong *A, ulong l, ulong r);
int partition_B(ulong *X, ulong l, ulong r, uint bM);
void quickSort_B(ulong *X, ulong l, ulong r, uint bM);
void swap64(ulong *A, uint posX, uint posY, uint bM);

int main(int argc, char** argv){
	ParProg *par = new ParProg();
	//clock_t t1,t2,tA,tB;
	//float timeA, timeB;

	if(argc != 4){
		cout << "Execution Error! call: ./tareaHPC n MAX prefixResult" << endl;
		exit(EXIT_FAILURE);
	}
	par->n = atoi(argv[1]);
	par->MAX = atoi(argv[2]);
	strcpy(par->prefixResult, "");
	strcpy(par->prefixResult, argv[3]);

	/*
	cout << "Parameters..." << endl;
	cout << "n = " << par->n << endl;
	cout << "MAX = " << par->MAX << endl;
	*/

	//genArrays(par);
	/*
	cout << "################## " << endl;
	t1 = clock();
	quickSort_A(par->A, 0, par->n-1);
	tA = clock() - t1;
	t2 = clock();
	quickSort_B(par->X, 0, par->n-1, bM);
	tB = clock() - t2;
	timeA = ((float)tA/CLOCKS_PER_SEC)*1000.0;
	timeB = ((float)tB/CLOCKS_PER_SEC)*1000.0;
	*/
	runQSAB(par);

	ulong i;
	if(par->n <= 32){
		cout << "A[]* = ";
		for (i=0; i<par->n; i++)
			cout << par->A[i] << " ";
		cout << endl;

		ulong j;
		cout << "B[]* = ";
		for (i=j=0; i<par->n; i++, j+=bM){
				cout << getNum64(par->X, j, bM) << " ";
			}
		cout << endl;
	}
	//cout << "Tiempo A = "<< timeA << " ms" << endl;
	//cout << "Tiempo B = "<< timeB << " ms" << endl;

	return 0;
}

void runQSAB(ParProg *par){
	uint k;
	float tA=0.0;
	float tB=0.0;
	char aFile[400],bFile[400];
	clock_t t;

	cout << "_________________________________________________" << endl;
	cout << "  Executing " << REPET << " QuickSort on A[] and B[]" << endl;

	for (k=0; k<REPET; k++){
		// generar arreglos A y B
		genArrays(par);
		// t1
		t = clock();
		// ordenar arreglo A
		quickSort_A(par->A, 0, par->n-1);
		// t2
		tA += (float)(clock() - t)/CLOCKS_PER_SEC;
		// t1
		t = clock();
		// ordena B
		quickSort_B(par->X, 0, par->n-1,bM);
		// t2
		tB += (float)(clock() - t)/CLOCKS_PER_SEC;
	}
	// A
	cout << "QS-A Total CPU time of "<< REPET <<" executions: " << (tA*1000.0) << " Milliseconds" << endl;
	cout << "QS-A Average CPU time per execution: " << (tA*1000000.0)/REPET << " Microseconds" << endl;

	strcpy(aFile, par->prefixResult);
	strcat(aFile, "QSA");
	cout << "Resume File: " << aFile << endl;

	FILE *fp = fopen(aFile, "a+" );
	// [n] [REPET] [avg bs-time/exec]
	fprintf(fp, "%ld %d %f\n", par->n, REPET, (tA*1000000.0)/REPET);
	fclose(fp);
	
	//B
	cout << "QS-B Total CPU time of "<< REPET <<" executions: " << (tB*1000.0) << " Milliseconds" << endl;
	cout << "QS-B Average CPU time per execution: " << (tB*1000000.0)/REPET << " Microseconds" << endl;
	
	strcpy(bFile, par->prefixResult);
	strcat(bFile, "QSB");
	cout << "Resume File: " << bFile << endl;

	FILE *fq = fopen(bFile, "a+" );
	// [n] [REPET] [avg bs-time/exec]
	fprintf(fq, "%ld %d %f\n", par->n, REPET, (tB*1000000.0)/REPET);
	fclose(fq);

}

// generate X nondecreasing array, PATT array for experiments and sample array for bSearch
void genArrays(ParProg *par){
	ulong i, j, k;

	par->A = new ulong[par->n];
	par->sizeA = par->n*sizeof(ulong);
	for (i=0; i<par->n; i++)
		par->A[i] = rand()%par->MAX;

	// **************************************************************************
	// create X[] array...
	bM = 1+log2(par->MAX);
	par->nWX = (par->n*bM)/(sizeof(ulong)*8);
	if ((par->n*bM)%(sizeof(ulong)*8)>0)
		par->nWX++;

	par->X = new ulong[par->nWX];
	par->sizeX = par->nWX*sizeof(ulong);

	//cout << "bM = " << bM << endl;
    	//cout << "nWX = " << par->nWX << endl;
	cout << "size for A[] = " << par->sizeA/(1024.0*1024.0) << " MiB" << endl;
	cout << "size for B[] = " << par->sizeX/(1024.0*1024.0) << " MiB" << endl;

	// store values from A[] to X[] (calling the method setNum64())...
	for (i=j=0; i<par->n; i++, j+=bM)
		setNum64(par->X, j, bM, par->A[i]);

	if (PRINT){
		cout << "A[] = ";
		for (i=0; i<par->n; i++)
			cout << par->A[i] << " ";
		cout << endl;

		cout << "B[] = ";
		//read values from X[]...
		for (i=j=0; i<par->n; i++, j+=bM){
			cout << getNum64(par->X, j, bM) << " ";
		}
		cout << endl;

		// print bits using printBitsUlong()...
	}

	if(TEST){
		// check all the position A[i] == X[i]...
		for (i=j=0; i<par->n; i++, j+=bM){
			k = getNum64(par->X, j, bM);
			if (k!=par->A[i]){
				cout << "ERROR. A[" <<i<< "] = " << par->A[i] << " != X[i] = " << k << endl;
				exit(-1);
			}
		}

		cout << "Test OK !!" << endl;
	}
}

void swap64(ulong *A, uint posX, uint posY, uint bM){
	uint auxX = getNum64(A, posX*bM, bM);
	uint auxY = getNum64(A, posY*bM, bM);
	setNum64(A, posY*bM, bM, auxX);
	setNum64(A, posX*bM, bM, auxY);
}

// realiza la particion de A[l..r] retornando la posision de la particion t,
// dejando A tal que todo elemento en A[l..t-1] < A[t] <= A[t+1...r]
int partition_A(ulong *A, ulong l, ulong r){
	uint i, pv, p;
	p = l;
	pv = A[p];
	for(i=l; i<=r; i++){
		if (A[i] < pv){
			swap(A[i], A[p+1]);
			p++;
		}
	}
	swap(A[l], A[p]);

	return p;
}

// ordena los elementos de A con el algoritmo quickSort
void quickSort_A(ulong *A, ulong l, ulong r){
	if (l<r){
		uint p = partition_A(A, l, r);
		if (p){
			quickSort_A(A, l, p-1); // de la mitad hacia la izquierda
			quickSort_A(A, p+1, r); // de la mitad hacia la derecha 
		}
   	}
}

// ordena los elementos de X con el algoritmo quickSort
int partition_B(ulong *X, ulong l, ulong r, uint bM){
	uint i, pv, p, k;
	p = l;
	pv = getNum64(X, p*bM, bM);
	for(i=l; i<=r; i++){
		k = getNum64(X, i*bM, bM);
		if (k < pv){
			swap64(X, i, p+1,bM);
			p++;
		}	
	}
	swap64(X, l, p,bM);
	return p;
}

void quickSort_B(ulong *X, ulong l, ulong r, uint bM){
	if (l<r){
		uint p = partition_B(X, l, r, bM);
		if (p){
			quickSort_B(X, l, p-1, bM);
			quickSort_B(X, p+1, r, bM);
		}
	}
}


/*for (i=j=0; i<par->n; i++, j+=bM){
			cout << getNum64(par->X, j, bM) << " ";
		}

for (i=j=0; i<par->n; i++, j+=bM)
		setNum64(X, j, bM, X[i]);*/
