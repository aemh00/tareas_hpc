#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <cmath>
#include <random>
#include <ctime>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <algorithm>

using namespace std;

#define SIZE_COUNT 257
#define MIN 2147483648	  // 2^31
#define MAX 1099511628000 // 2^40

#define PRINT 0
#define TEST 0

#ifndef uchar // 1 byte
#define uchar unsigned char
#endif

uint REPET = 1000000;

// Structure with all globals parameters program
typedef struct {
	char prefixResult[300];	// Prefix name of results files in the experiments
	ulong *A;
	ulong *PATT;
	uint n;

	bool NORMAL;	// flag probability: 1 = NORMAL, 0 = UNIFORM
	uint stddev;	// Standard deviation for normal distribution, media = n/2

	uint *ALow;		// almacena los 4 bytes menos significativos de A
	int COUNT[SIZE_COUNT];
} ParProg;

void genArrays(ParProg *par);
void runBS(ParProg *par);
void runBSOpt(ParProg *par);
void runBSCOUNT(ParProg *par);
int binarySearch(ulong *A, int l, int r, ulong x);
int binarySearchOpt(ulong *A, uint n, ulong x);
int binarySearchCOUNT(uint *X, int *COUNT, uint n, ulong x);
void testSearches(ParProg *par);

void printBitsUlong(ulong x);
void separateUlong(ulong x, uchar &highX, uint &lowX);

// ./bSearch 200 ./here 1 80
// ./bSearch 20000000 ./here 1 80000
int main(int argc, char** argv){
	ParProg *par = new ParProg();

	if(argc < 4){
		cout << "Execution Error! call: ./bsearch <n> <prefixResult> <NORMAL_flag> [<stddev>]" << endl;
		exit(EXIT_FAILURE);
	}
	par->n = atoi(argv[1]);
	strcpy(par->prefixResult, "");
	strcpy(par->prefixResult, argv[2]);
	par->NORMAL = atoi(argv[3]);
	if (par->NORMAL)
		par->stddev = atoi(argv[4]);

	cout << "Parameters..." << endl;
	cout << "n = " << par->n << endl;
	cout << "prefixResult = " << par->prefixResult << endl;
	cout << "NORMAL flag = " << par->NORMAL << endl;
	if (par->NORMAL)
		cout << "stddev = " << par->stddev << endl;

	/*ulong x = pow(2, 38) - 5;
	cout << "x = " << endl;
	printBitsUlong(x);
	uchar c;
	uint y;
	separateUlong(x, c, y);
	cout << endl << "c = " << (uint)c;
	cout << "y = " << endl;
	printBitsUlong((ulong)y);
	cout << endl;
	exit(0);*/

	genArrays(par);
	cout << " A[n] = " << par->A[par->n-1] << endl;
	if (TEST){
		cout << " Testing..." << endl;
		testSearches(par);
		cout << " Test OK !! " << endl;
	}

	runBS(par);
	runBSOpt(par);
	runBSCOUNT(par);

	cout << "################## " << endl;
	return 0;
}


// generate X nondecreasing array, PATT array for experiments and sample array for bSearch
void genArrays(ParProg *par){
	ulong i;
	long int num;
	mt19937_64 gen(random_device{}());

	if (par->NORMAL){
		par->A = new ulong[par->n];
		default_random_engine generator;
		normal_distribution<double> distribution(par->n/2, par->stddev);	// (mean, stddev)

		for (i=0; i<par->n; i++){
			num = distribution(generator);
	    	while (num<0)
	    		num = distribution(generator);
	    	par->A[i] = num;
	    }
	}else{
		par->A = new ulong[par->n];
		for (i=0; i<par->n; i++){
			par->A[i] = MIN + (gen()%(MAX-MIN));
		}
	}
	sort(par->A, par->A+par->n);

	// create 	ALow, COUNT[256]
	for (i=0; i<SIZE_COUNT; i++)
		par->COUNT[i]=-1;
	par->COUNT[SIZE_COUNT-1] = par->n-1;

	ulong mask = 0x00000000FFFFFFFF;
	int c;
	par->ALow = new uint[par->n];
	for (i=0; i<par->n; i++){
		par->ALow[i] = par->A[i] & mask;
		c = (par->A[i]) >> 32;
		//cout << " " << c;
		if(par->COUNT[c]==-1)
			par->COUNT[c] = i;
	}
	//cout << endl;

	cout << "size for A: " << par->n*sizeof(ulong) << " bytes = " << par->n*sizeof(ulong)/(float)1048576 << " MiB" << endl;
	cout << "size for ALow: " << par->n*sizeof(uint) << " bytes = " << par->n*sizeof(uint)/(float)1048576 << " MiB" << endl;
	cout << "size for COUNT: " << SIZE_COUNT*sizeof(int) << " bytes = " << SIZE_COUNT*sizeof(int)/(float)1048576 << " MiB" << endl;

	// patterns for experiments
	par->PATT = new ulong[REPET];
	for (i=0; i<REPET; i++)
		if(rand()%2)
			par->PATT[i] = par->A[rand()%par->n];	// this is in A
		else
			par->PATT[i] = gen()%MAX;

	// create sample scan structures...
	if (PRINT){
		cout << "A[] = ";
		for (i=0; i<par->n; i++)
			cout << par->A[i] << " ";
		cout << endl;

		/*cout << "PATT[] =" << endl;
		for (i=0; i<REPET; i++)
			cout << par->PATT[i] << " ";
		cout << endl;*/

		cout << "COUNT[0.." << SIZE_COUNT-1 << "] = ";
		for (i=0; i<SIZE_COUNT; i++)
			cout << par->COUNT[i] << " ";
		cout << endl;
	}
}

void runBS(ParProg *par){
	uint k;
	float avgTime;
	char aFile[400];
	clock_t t;

	cout << "_________________________________________________" << endl;
	cout << "  Executing " << REPET << " Binary Search on A[] " << endl;

	t = clock();
	for (k=0; k<REPET; k++)
		binarySearch(par->A, 0, par->n-1, par->PATT[k]);
	t = clock() - t;
	avgTime = (float)t/CLOCKS_PER_SEC;
	cout << "Total CPU time per execution: " << (avgTime*1000.0) << " Milliseconds" << endl;
	cout << "Average CPU time per execution: " << (avgTime*1000000.0)/REPET << " Microseconds" << endl;

	strcpy(aFile, par->prefixResult);
	strcat(aFile, "bSearch");
	cout << "Resume File: " << aFile << endl;

	FILE *fp = fopen(aFile, "a+" );
	if (par->NORMAL){
		// [n] [REPET] [avg bs-time/exec] [media] [stddev]
		fprintf(fp, "%d %d %f %d %d\n", par->n, REPET, (avgTime*1000000.0)/REPET, par->n/2, par->stddev);
	}else{
		// [n] [REPET] [avg bs-time/exec]
		fprintf(fp, "%d %d %f\n", par->n, REPET, (avgTime*1000000.0)/REPET);
	}	fclose(fp);
}

void runBSOpt(ParProg *par){
	uint k;
	float avgTime;
	char aFile[400];
	clock_t t;

	cout << "_________________________________________________" << endl;
	cout << "  Executing " << REPET << " Binary Search Opt* on A[] " << endl;

	t = clock();
	for (k=0; k<REPET; k++)
		binarySearchOpt(par->A, par->n, par->PATT[k]);
	t = clock() - t;
	avgTime = (float)t/CLOCKS_PER_SEC;
	cout << "Total CPU time per execution: " << (avgTime*1000.0) << " Milliseconds" << endl;
	cout << "Average CPU time per execution: " << (avgTime*1000000.0)/REPET << " Microseconds" << endl;

	strcpy(aFile, par->prefixResult);
	strcat(aFile, "bSearchOpt");
	cout << "Resume File: " << aFile << endl;

	FILE *fp = fopen(aFile, "a+" );
	if (par->NORMAL){
		// [n] [REPET] [avg bs-time/exec] [esperanza] [varianza]
		fprintf(fp, "%d %d %f %d %d\n", par->n, REPET, (avgTime*1000000.0)/REPET, par->n/2, par->stddev);
	}else{
		// [n] [REPET] [avg bs-time/exec]
		fprintf(fp, "%d %d %f\n", par->n, REPET, (avgTime*1000000.0)/REPET);
	}	fclose(fp);
}


void runBSCOUNT(ParProg *par){
	uint k;
	float avgTime;
	char aFile[400];
	clock_t t;

	cout << "_________________________________________________" << endl;
	cout << "  Executing " << REPET << " Binary Search COUNT on A[] " << endl;

	t = clock();
	for (k=0; k<REPET; k++)
		binarySearchCOUNT(par->ALow, par->COUNT, par->n, par->PATT[k]);
	t = clock() - t;
	avgTime = (float)t/CLOCKS_PER_SEC;
	cout << "Total CPU time per execution: " << (avgTime*1000.0) << " Milliseconds" << endl;
	cout << "Average CPU time per execution: " << (avgTime*1000000.0)/REPET << " Microseconds" << endl;

	strcpy(aFile, par->prefixResult);
	strcat(aFile, "bSearchCOUNT");
	cout << "Resume File: " << aFile << endl;

	FILE *fp = fopen(aFile, "a+" );
	if (par->NORMAL){
		// [n] [REPET] [avg bs-time/exec] [esperanza] [varianza]
		fprintf(fp, "%d %d %f %d %d\n", par->n, REPET, (avgTime*1000000.0)/REPET, par->n/2, par->stddev);
	}else{
		// [n] [REPET] [avg bs-time/exec]
		fprintf(fp, "%d %d %f\n", par->n, REPET, (avgTime*1000000.0)/REPET);
	}	fclose(fp);
}


// binary search for x on X[]
int binarySearch(ulong *A, int l, int r, ulong x){
	if (l>r)
		return -1;

	int m = l + (r-l)/2; // m = (l+r)/2 = (l+r)>>1
	if (x==A[m])
		return m;
	
	if (x<A[m])
		return binarySearch(A, l, m-1, x);
	return binarySearch(A, m+1, r, x);
}

// binary search for x on X[]
int binarySearchOpt(ulong *A, uint n, ulong x){
	int l, r, m;
	l=0;
	r=n-1;

	while(l<=r){
		m = (l+r)>>1;
		if (x<A[m])
			r=m-1;
		else{
			if (x==A[m])
				return m;
			else
				l = m+1;
		}	
	}

	return -1;
}

//Terea
int binarySearchCOUNT(uint *X, int *COUNT, uint n, ulong x){
	int l, h = x >> 32;

	l = COUNT[h];
	//cout << "h = " << h << ", l = " << l << endl;
	if (l == -1)
		return -1;

	int r, m;
	ulong mask = 0x00000000FFFFFFFF;
	uint y = x & mask;

	for (m=h+1, r=COUNT[m]; r==-1; m++, r=COUNT[m]);
	//cout << "y = " << y << ", r = " << r << endl;
	while(l<=r){
		m = (l+r)>>1;
		if (y<X[m])
			r=m-1;
		else{
			if (y==X[m])
				return m;
			else
				l = m+1;
		}
	}

	return -1;
}

void testSearches(ParProg *par){
	int pos1, pos2, pos3, nOcc1, nOcc2, nOcc3;
	
	ulong x;
	nOcc1 = nOcc2 = nOcc3 = 0;
	for (uint k=0; k<REPET; k++){
		x = par->PATT[k];
		pos1 = binarySearch(par->A, 0, par->n-1, x);
		pos2 = binarySearchOpt(par->A, par->n, x);
		pos3 = binarySearchCOUNT(par->ALow, par->COUNT, par->n, x);
		if (pos1>-1) nOcc1++;
		if (pos2>-1) nOcc2++;
		if (pos3>-1) nOcc3++;
		if ( (pos1 == -1 && (pos2>=0 || pos3>=0))
		  || (pos2 == -1 && (pos1>=0 || pos3>=0))
		  || (pos3 == -1 && (pos1>=0 || pos2>=0)) ){
			cout << "ERROR. patt[" <<k<< "] = " << x << endl;
			cout << "pos1 = " << pos1 << endl;
			cout << "pos2 = " << pos2 << endl;
			cout << "pos3 = " << pos3 << endl;
			exit(1);
		} else
			if (par->A[pos1] != par->A[pos2] || par->A[pos1] != par->A[pos2]){
				cout << "ERROR. patt[" <<k<< "] = " << x << endl;
				cout << "pos1 = " << pos1 << endl;
				cout << "pos2 = " << pos2 << endl;
				cout << "pos3 = " << pos3 << endl;
				exit(1);
		}
	}
	cout << "nOcc1 = " << nOcc1 << ", nOcc2 = " << nOcc2 << ", nOcc3 = " << nOcc3 << endl;
}

// print W64 bits of unsigned long int x
void printBitsUlong(ulong x){
	uint cnt = 0;
	ulong mask = 0x8000000000000000;
	uint bitsW = 64;

	for(cnt=0;cnt<bitsW;++cnt){
		putchar(((x & mask) == 0) ? '0' : '1');
		mask >>= 1;
	}
}

// With 5 bytes we can create  2^40 = 1099511628000 diferent numbers
// x has 8 bytes (from left to right), write the byte number 4 in highX
// and the last 4 bytes  of x in lowX
void separateUlong(ulong x, uchar &highX, uint &lowX){
	ulong mask = 0x00000000FFFFFFFF;

	highX = x >> 32;
	lowX = x & mask;
}
