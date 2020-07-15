#ifndef TOOLS_H
#define TOOLS_H
using namespace std;
const char sym[3] = {'.', '@', 'L'};
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

void ubicar_manada_det(int *mat, int n, vector<pair<int,int>> &man, int &a,int &b){
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
    //printf("lobos=%i\n sqrt(lobos) = %lf\n dim = %lf\n",lobos,sq_l,dim);
    if (decimal == 0.0){ // entonces a es entero
	    //printf("caso 1\n");
	    int cuadritos = n/dim;
	    //printf("cuadritos = %i\n",cuadritos);
	    for (int i = 0; i < n; i+=dim){
		    for (int j = 0; j < n; j+=dim){
			    // hay que saber donde poner lo L
			    //printf("pone loh loboh\n");
			    mat[i*n + j] = 2;
		    	    man[k] = {i,j};
			    k++;

		    }
		    // calcular coordenadas dentro de cada cuadricula
		    // cuadricula dim*dim
		    // if k=0 -> i,j=0,0
		    // else: j+dim
	    }
	    a = (int) dim;
	    b = (int) dim;

    }
    else {
	    //caso 2
	    // buscar potencias anteriores y siguientes para a y b
	    // a = potencia_anterior(a);
	    // b = potencia_siguiente(a);
	    int c = 2;
	    while (dim>c){ c*=2;}
	    b = c;
	    while (c>dim){ c/=2;}
	    a = c;
	    //printf("a= %i, b= %i\n",a,b);
	    for (int i = 0; i < n; i+=b){
		    for (int j = 0; j < n; j+=a){
			    // hay que saber donde poner lo L
			    //printf("pone loh loboh\n");
			    mat[i*n + j] = 2;
		    	    man[k] = {i,j};
			    k++;
		    }	    
	    }
	       
    }

    //int i = 0;
    //int j = 0;

 
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
        //int i = rand() % n;
        //int j = rand() % n;
	// para que no aplasten al lobito
        //while(mat[i*n + j] == 1){
        //    i = rand() % n;
        //    j = rand() % n;
        //}
        //mat[i*n + j] = 2;
        // inicializar la manada con (i,j)
        for(int k=0; k<man.size(); ++k){
            // cada lobo parte en distinto lugar
            int i = rand() % n;
            int j = rand() % n;
            while(mat[i*n + j] == 1){
                i = rand() % n;
                j = rand() % n;
            }
            mat[i*n + j] = 2;
            man[k] = {i,j};
        }
    }
}

// busqueda deterministica 
std::pair<int, int> busqueda_det(int *mat, int n, vector<pair<int,int>> &m, int a, int b){
    int perdido = 1;
    // while (no encontrado)
    int i,j;
    //printf("llegan a = %i, b = %i\n",a,b);
    while(perdido){
        // un paso de busqueda
        for(int k=0; k<m.size(); ++k){
            mat[m[k].first*n + m[k].second] = 0;
	    i = m[k].first;
	    j = m[k].second;
	    if (i%2 ==0){ //me muevo pa la derecha (j+1)
		    if (j%a < a-1){ j+=1; }
		    else{ i+=1; }
	    }
	    else{ // memuevo pa la izq (j-1)
		    if (j%a > 0){ j-=1; }
		    else if (i%b < b-1){ i+=1; }
	    }
	    // si el J tiene espacio pal lao en el sub bloque, entonces se mueve
            //j = (m[k].second + 1);
	    // si llego al final el J. Si el i tiene espacio para abajo, se mueve
	    //y el i aumenta a la siguiente fila.
            //i = (m[k].first + 1);
	    //preguntar si encontré el lobito
            if (mat[i*n + j] == 1){
		    // pillao
		    perdido = 0;
		    k = m.size();
	    }
	    else {
		    m[k] = {i,j};
		    mat[i*n + j] = 2;
	    }
        }
        #ifdef DEBUG
            print_mat(mat, n);
            getchar();
        #endif
    }
    return {i,j};

}

// busqueda no-deterministica
std::pair<int, int> busqueda_nodet(int *mat, int n, vector<pair<int,int>> &m){
    int perdido = 1;
    // while (no encontrado)
    int i,j;
    while(perdido){
        // un paso de busqueda
        for(int k=0; k<m.size(); ++k){
            mat[m[k].first*n + m[k].second] = 0;
            i = (m[k].first + ((rand() % 3) - 1)) % n;
            j = (m[k].second + ((rand() % 3) - 1)) % n;
	    //preguntar si encontré el lobito
            if (mat[i*n + j] == 1){
		    // pillao
		    perdido = 0;
		    k = m.size();
	    }
	    else {
		    m[k] = {i,j};
		    mat[m[k].first*n + m[k].second] = 2;
	    }
        }
        #ifdef DEBUG
            print_mat(mat, n);
            getchar();
        #endif
    }
    return {i,j};
}
#endif
