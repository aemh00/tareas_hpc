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
    //while (i==0) {i = rand() % n;}
    int j = rand() % n;
    // verificar que no haya lobos en casilla
    while(mat[i*n + j] == 2){
            i = rand() % n;
            j = rand() % n;
        }

    mat[i*n + j] = 1;
}

void ubicar_manada_det(int *mat, int n, vector<pair<int,int>> &man){
    // dividir el dominio para k lobos
    // considerar particion 2D, factorizar k en dos valores 'similares'.
    // usar 'techo' y 'piso' de raiz de k para hacer particiones.
    //
    // primero: k sub matrices
    // segundo: n*n/k casillas en cada sub matriz
    // tercero: encontrar las dimensiones que mejor dividan la matriz de n*n en k partes de n*n/k casillas c/u
     
    int lobos = man.size();
    double sq_l;
    sq_l = sqrt(lobos*1.0);

    double dim = n/sq_l;
    //double num = n*n*1.0/lobos*1.0;
    double entero,decimal;
    decimal = modf(dim,&entero);
    //int resto = n%sq_l;
    int k = 0;
    printf("lobos=%i\n sqrt(lobos) = %lf\n dim = %lf\n",lobos,sq_l,dim);
    if (decimal == 0.0){ // entonces a es entero
	    printf("caso 1\n");
	    int cuadritos = n/dim;
	    printf("cuadritos = %i\n",cuadritos);
	    for (int i = 0; i < n; i+=dim){
		    for (int j = 0; j < n; j+=dim){
			    // hay que saber donde poner lo L
			    printf("pone loh loboh\n");
			    mat[i*n + j] = 2;
		    	    man[k] = {i,j};
			    k++;

		    }
		    // calcular coordenadas dentro de cada cuadricula
		    // cuadricula dim*dim
		    // if k=0 -> i,j=0,0
		    // else: j+dim
	    }

    }
    else {
	    //caso 2
	    // buscar potencias anteriores y siguientes para a y b
	    // a = potencia_anterior(a);
	    // b = potencia_siguiente(a);
	    int c = 2;
	    while (dim>c){ c*=2;}
	    int b = c;
	    while (c>dim){ c/=2;}
	    int a = c;
	    printf("a= %i, b= %i\n",a,b);
	    for (int i = 0; i < n; i+=a){
		    for (int j = 0; j < n; j+=b){
			    // hay que saber donde poner lo L
			    printf("pone loh loboh\n");
			    mat[i*n + j] = 2;
		    	    man[k] = {i,j};
			    k++;

		    }
		    
	    }


   
    }

    //int i = 0;
    //int j = 0;

   // if ((n*n)%lobos == 0){
	    //int macrofilas = k;
//	    int cxl = n*n/lobos;
//	    for(int k=0; k<lobos; ++k){            
		    // a cada lobo le asignamos n/k filas;
//		    j = k*cxl;
		    // revisar condiciones de borde
//		    mat[i*n + j] = 2;
//		    man[k] = {i,j};
  //          }
    //}
  
   // else {
    //     asigno el numero entero de filas a c/lobo, excepto a uno que queda con +1 fila
    //	   int fxl = entero;
//	   int sobran = n*n%lobos;
//	   int n_sobran = 0;
//	   for(int k=0; k<lobos; ++k){            
		    // a cada lobo le asignamos fxl filas;
//		    printf("sobran %i\n",n_sobran);
//		    if (k==0){
//			j = k*fxl;
//		    }
//		    else if (n_sobran<sobran){
//		    	j = k*fxl+n_sobran;
//		    	n_sobran++;
//		    }
//		    else{
//		        j = k*fxl+n_sobran;
//		    }
//		    mat[i*n + j] = 2;
//		    man[k] = {i,j};
  //          }
//
   // }
    //##################################################
    // 2D
    //for (int k = 0; k<lobos; k++){
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
    //}

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
