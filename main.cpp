#include <iostream>
#include <cstdio>
#include <omp.h>
#include <math.h>

void printarray(int *a, int n, const char *msg){
    if( n <= 320) {
        printf("%s:\n", msg);
        for(int i=0; i<n; ++i){
            printf("%i  ", i);
        }
        printf("\n");
        for(int i=0; i<n; ++i){
            printf("%i  ", a[i]);
        }
        printf("\n");
        //printf("\n");
    }
}

// analisis problema: subir la cota minima lo mas posible (Omega(f(n)))
// analisis algoritmo: bajar la cota superior de tiempo (O(f(n))
// pointer jumping (algoritmo)
double pointer_jumping_seq(int *p, int n){
	// tareita
	// secuencial
	double ts1 = omp_get_wtime();
	int i = 0;
	while(p[i] != n-1){
		if (p[i+1] == n-1){
			p[i] = n-1;
			i = 0;	
		}
		i++;

	}
	p[0] = n-1;
	double ts2 = omp_get_wtime();
	double ts = ts2-ts1;
	return ts;
}
double pointer_jumping_seq2(int *p, int n){
	// tareita
	// secuencial
	//double log2n = log(n)/log(2);
	double ts1 = omp_get_wtime();
	int i = 0;
	// p[0] es el elemento 1, pero contiene lo que apunta
	// ej p[0]=4
	// p[4]=4
	// elem  a b c d e f g h  
	// punt  b c d e f g h h
	
	// it 1  c d e f g h h h
	// it 2  e f g h h h h h
	// it 3  h h h h h h h h
	for (int k = 0; k< log2(n); ++k){
		//printf("k = %i\n",k);
		while(p[i] != p[ p[i] ]){
			p[i] = p[ p[i] ];
			i++;
		}
		i=0;
	}
	double ts2 = omp_get_wtime();
	double ts = ts2-ts1;
	return ts;
}

double pointer_jumping_par(int *q, int n, int nt){

	// paralelo
	double tp1 = omp_get_wtime();
	#pragma omp parallel shared(q)
	{
		int c = 0;
		int tid = omp_get_thread_num();
		int segment = (n + nt -1)/nt;
		int start = segment*tid;
		int end = start + segment;
		printf("seg %i, st %i, end %i\n",segment, start,end);
		//#pragma omp parallel for shared(q)
		//      [   t1        [ ]           [ ]           [ ]         ]
		//      [ thread 1 ]  [ thread 2 ]  [ thread 3  ] [ thread 4  ] 
		// i     0   1   2     3   4   5     6   7   8     9   10  11 
		
		// el    1   2   3     4   5   6     7   8   9     10  11  12 
		
		// p0    2   3   4     5   6   7     8   9   10    11  12  12
		// p1    3   3   4     6   6   7     9   9   10    12  12  12
		for(int k=start; k<end && k<n; ++k){
			int i=0;
			while(q[i] != q[ q[i] ] && i!=end){
				//printf("--> %i\n",k);
				q[i] = q[ q[i] ];
				i++;
				//printf("--> %i\n",tid);
				//printf("thread %i  it %i  k %i\n", tid, c++,k);

			}
			#pragma omp barrier
			
		}
  	}	
	double tp2 = omp_get_wtime();
	double tp = tp2-tp1;
	return tp;
}


int main(int argc, char **argv){
	if(argc != 3){
		fprintf(stderr, "./prog n nt\n");
		exit(EXIT_FAILURE);
	}
	int n = atoi(argv[1]);
	int nt = atoi(argv[2]);
	int *p = new int[n];
	for(int i=0; i<n-1; ++i){
		p[i] = i+1;
	}
	int *q = new int[n];
	for(int i=0; i<n-1; ++i){
		q[i] = i+1;
	}

	p[n-1] = n-1;
	q[n-1] = n-1;
	printarray(p, n, "arreglo original P");
	printarray(q, n, "arreglo original Q");

	//printarray(p, n, "arreglo original");
	omp_set_num_threads(nt);
	double tseq = pointer_jumping_seq2(p, n);
	printarray(p, n, "solucion P");
	
	double tpar = pointer_jumping_par(q, n,nt);
	printarray(q, n, "solucion Q");

	printf("tseq = %f\ntpar = %f\n",tseq,tpar);
	return 0;
}
