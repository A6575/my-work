/*
Estudiante: Astrid Alvarado
User CodeForce: A_6575
Ejercicio: Divan and a New Project (https://codeforces.com/problemset/problem/1614/B)
Veredicto: Accepted

Solucion
La idea consiste en generar un vector de pares, donde el primer elemento a_i
de cada par es las veces que Divan visita el edificio i, y el segundo elemento
corresponde al número del edificio i. Este vector se ordena usando radix sort,
y luego generamos un vector de coordenadas x_i tales que el tiempo T transcurrido
por Divan es el mínimo. Este vector de coordenadas se calcula escogiendo un punto
medio mid, el cual corresponderá a la coordenada donde la sede será colocada,
y luego vamos intercalando el resto de edificios entre derecha e izquierda del 
mismo, donde los primeros edificios en posicionar serán los más visitados.
*/

#include<iostream>
#include<vector>
#include<assert.h>
#include<cmath>

using namespace std;

// Implementación de counting sort y radix sort para ordenar vectores de pares.
// Se ordena basándose en el primer elemento de cada par, sin tomar en cuenta
// el segundo elemento.
vector<pair<long long, long long> > counting_sort(vector<pair<long long, long long> > &A, vector<pair<long long, long long> > &S) {
	//Variables de la función counting_sort
	long long m = A[0].first;
	int n = A.size();
	
	for(int i=1; i<n; i++){
		m = max(m, A[i].first);  //buscamos el máximo valor entre los primeros elementos de cada par
	}
	
	vector<int> acum(m+1, 0);  
	vector<pair<long long, long long> > R(n);

    // Generamos un vector acum que almacenará la cantidad de veces que se encuentra cada primer elemento de los pares en el vector
	for(int i=0; i<n; i++){
		acum[A[i].first]++;
	}
	
	// Cada elemento de acum se le sumará el anterior
	for(int i=1; i<m+1; i++){
		acum[i] += acum[i-1];
	}
	
	// Iteramos desde n-1 hasta 0 y en el vector acum nos posicionamos en el índice A[i].first, 
	// restamos 1 y al vector resultado le asignamos el valor correspondiente
	for(int i = n-1; i>=0; i--){
		acum[A[i].first]--;
		R[acum[A[i].first]].first = S[i].first;
		R[acum[A[i].first]].second = S[i].second;
	}
	
	return R;
}
// Implementamos radix sort para vectores de pares usando couting sort antes implementado
vector<pair<long long,long long> > radix_sort(vector<pair<long long, long long> > &A) {
	// Variables de la función radix sort
	int n = A.size(),
	    d = 1;
	long long m = A[0].first;
    vector<pair<long long, long long> > D(n),
                                        R(A);
    for(int i = 0; i<n; i++){
    	m = max(m, A[i].first);  // buscamos el máximo valor entre los primeros elementos de cada par
    }
    
    // Comparamos cada dígito de los primeros elementos de lo pares del vector y ordenamos
    while(m>=d){
    	for(int i = 0; i<n; i++){
    		D[i].first = (R[i].first / d) % 10;
    		D[i].second = (R[i].second / d) % 10;
    	}
    	R = counting_sort(D, R);  // Llamamos a counting sort con los vectores D y R
    	d *= 10;
    }
    return R;
}

// Función para imprimir los elementos de un vector
void print_vector(vector<long long> &A) {
	for (int i=0 ; i<A.size() ; i++)
		cout << A[i] << " ";
	cout << endl;
}

// Programa principal
int main(){
	// Variables del programa
	int t, n, r, l;
	long mid;
	
	cin >> t;
	// Precondición:
	// t está en el intervalo [1, 1000] 
	// assert(1<=t<=1000);
	while(t-->0){
		unsigned long long T = 0; // Tiempo mínimo T transcurrido 
		cin >> n;
		// Precondición:
		// n debe estar en el intervalo [1, 2*10^5]
		// assert(1<=n<=2*pow(10, 5));
		
	    vector<pair<long long, long long> > a(n); // Vector donde se almacena las cantidad de veces que Divan visita el edificio i
		vector<long long> x(n+1); // Vector de coordenadas
		
		// Se ingresan las veces que Divan visita el edificio i y se almacena como 
		// primer elemento del par, y en el segundo elemento el valor de i
		for(int i = 0; i<n; i++){
			cin >> a[i].first;
			// Precondición:
			// a_i debe estar en el intervalo [0, 10^6]
			// assert(0<= a[i].first <= pow(10, 6));
			
			a[i].second = i;
		}
		
		a = radix_sort(a); // Ordenamos el vector de forma ascendente
		mid = (n+1)/2; // Escogemos un punto medio en el cual ubicar la sede
		
		// Inicializamos las variables r y l con el valor mid.
		// Dichas variables nos permitirán iterar entre derecha
		// e izquierda y así poder ubicar el resto de los edificios
		r = mid; 
		l = mid;
		
		// A la primera coordenada se le asigna el punto medio
		x[0] = mid;
		
		
		/* Dado que el vector de pares está ordenado, tendremos que 
		   aquellos edificios que más visita Divan estará en la última
		   posición del vector, y los que menos visita en la primera 
		   posición. Como se quiere que los que más visitados estén cerca
		   de la sede, empezaremos iterando desde n-1 hasta 0.
		*/
		for(int i = n-1; i>=0; i--){
			/* Se busca iterar entre derecha e izquierda siempre, por lo que
			   tendremos los siguientes casos: si n es par, entonces todos 
			   los índices que sean impares terminarán en la derecha del punto
			   medio, dejando a los pares del lado izquierdo. En cambio, si n
			   es impar, ocurre el caso contrario, es decir, todos los índices
			   que sean pares quedarán del lado derecho del punto medio y los
			   impares del lado izquierdo.
			*/
			
			if(n%2 == 0){
				if(i%2 == 1){
					x[a[i].second + 1] = r + 1;
					r++;
					T += 2*a[i].first*(abs(x[0]-x[a[i].second + 1]));
				}else{
					x[a[i].second + 1] = l - 1;
					l--;
					T += 2*a[i].first*(abs(x[0]-x[a[i].second + 1]));
				}
				
			}else{
				if(i%2 == 0){
					x[a[i].second + 1] = r + 1;
					r++;
					T += 2*a[i].first*(abs(x[0]-x[a[i].second + 1]));
				}else{
					x[a[i].second + 1] = l - 1;
					l--;
					T += 2*a[i].first*(abs(x[0]-x[a[i].second + 1]));
				}
			}
		}
		cout<< T<< endl;  // T es la sumatoria desde i = 1 hasta n de 2*a_i*|x_0 - x_i|
		print_vector(x);
    }
    return 0;
}
