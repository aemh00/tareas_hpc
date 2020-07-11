#ifndef TOOLS_H
#define TOOLS_H
using namespace std;
const char sym[3] = {'*', '@', 'L'};
void init_mat_const(int *mat, int n, int c){
    #pragma omp parallel for
    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            long index = (long)i*n + (long)j;
            mat[index] = c;
        }
    }
}

void print_mat(int *mat, int n){
    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            long index = i*n + j;
            printf("%c ", sym[mat[index]]);
        }
        printf("\n");
    }
}
// ubicar cachorro
void ubicar_lobito(int *mat, int n){
    int i = rand() % n;
    while (i==0) {i = rand() % n;}
    int j = rand() % n;
    mat[i*n + j] = 1;
}

void ubicar_manada_det(int *mat, int n, vector<pair<int,int>> &man){
    // dividir el dominio para k lobos
    // considerar particion 2D, factorizar k en dos valores 'similares'.
    // usar 'techo' y 'piso' de raiz de n para hacer particiones.
    int i = 0;
    int j = 0;
    int lobos = man.size();
    double num = n*n*1.0/lobos*1.0;
    double entero, decimal;
    decimal = modf(num,&entero);

    if ((n*n)%lobos == 0){
	    //int macrofilas = k;
	    int cxl = n*n/lobos;
	    for(int k=0; k<lobos; ++k){            
		    // a cada lobo le asignamos n/k filas;
		    j = k*cxl;
		    // revisar condiciones de borde
		    mat[i*n + j] = 2;
		    man[k] = {i,j};
            }

    }
  
    else {
    //     asigno el numero entero de filas a c/lobo, excepto a uno que queda con +1 fila
    	   int fxl = entero;
	   int sobran = n*n%lobos;
	   int n_sobran = 0;
	   for(int k=0; k<lobos; ++k){            
		    // a cada lobo le asignamos fxl filas;
		    printf("sobran %i\n",n_sobran);
		    if (k==0){
			j = k*fxl;
		    }
		    else if (n_sobran<sobran){
		    	j = k*fxl+n_sobran;
		    	n_sobran++;
		    }
		    else{
		        j = k*fxl+n_sobran;
		    }
		    mat[i*n + j] = 2;
		    man[k] = {i,j};
            }

    }
    //##################################################
    // 2D
    for (int k = 0; k<lobos; k++){
	    //calcular indices 
	    //
	    //

//	    for (int i = 0; i<n; i++){
//		    if (i%2 == 0){
//			    // pa la derecha
//			    for (int j = 0; j<n; j++){
//				    //wiri
//			    }
//		    }
//		    else{
//			    // pa la izq
//			    for (int j = n; j>0; j--){
//				    //wiri
//			    }
//		    }
//	    
//	    } 
    }

}

void ubicar_manada_nondet(int *mat, int n, vector<pair<int,int>> &man){
    // pintar en la matriz
    if(man.size() > 0){
        // todos parten de un mismo lugar
        int i = rand() % n;
        int j = rand() % n;
	// para que no aplasten al lobito
        while(mat[i*n + j] == 1){
            i = rand() % n;
            j = rand() % n;
        }
        mat[i*n + j] = 2;
        // inicializar la manada con (i,j)
        for(int k=0; k<man.size(); ++k){
            // cada lobo parte en distinto lugar
            //int i = rand() % n;
            //int j = rand() % n;
            //while(mat[i*n + j] == 1){
            //    i = rand() % n;
            //    j = rand() % n;
            //}
            //mat[i*n + j] = 2;
            man[k] = {i,j};
        }
    }
}

// busqueda deterministica 
std::pair<int, int> busqueda_det(int *mat, int n, vector<pair<int,int>> &m){
	// wiri
}

// busqueda no-deterministica
std::pair<int, int> busqueda_nodet(int *mat, int n, vector<pair<int,int>> &m){
    int perdido = 1;
    // while (no encontrado)
    while(perdido){
        // un paso de busqueda
        for(int k=0; k<m.size(); ++k){
            mat[m[k].first*n + m[k].second] = 0;
            int i = (m[k].first + ((rand() % 3) - 1)) % n;
            int j = (m[k].second + ((rand() % 3) - 1)) % n;
            m[k] = {i,j};
            mat[m[k].first*n + m[k].second] = 2;
        }
        //#ifdef DEBUG
            print_mat(mat, n);
            getchar();
        //#endif
    }
    return {0,0};
}
#endif
